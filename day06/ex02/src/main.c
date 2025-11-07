#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include "../include/uart.h"

#define TW_READ 1
#define TW_WRITE 0
#define I2C_FREQ_HZ 100000UL
#define AHT20_ADDR 0x38

#define AVG_COUNT 3
float t_buf[AVG_COUNT];
float h_buf[AVG_COUNT];
uint8_t idx = 0;
uint8_t filled = 0;

void add_measure(float t, float h) {
    t_buf[idx] = t;
    h_buf[idx] = h;

    idx = (idx + 1) % AVG_COUNT;
    if (filled < AVG_COUNT)
        filled++;
}

float average(float *buf) {
    float s = 0;
    for (int i = 0; i < filled; i++)
        s += buf[i];
    return s / filled;
}

float round2(float v) {
    return (float)((int)(v * 100 + 0.5)) / 100.0;
}


/*
    lib twi.h
    https://www.nongnu.org/avr-libc/user-manual/group__util__twi.html

    AHT20 Datasheet
    https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf

    I2C temp sensor PINS
    PC4(ADC4/SDA)  => Data I/O
    PC5(ADC5/SCL)  => Clock

    La datasheet donne les formules officielles p.8

    Humidité brute (20 bits)
*/

void i2c_init(void)
{
    TWSR &= ~((1 << TWPS0) | (1 << TWPS1));
    TWBR = (uint8_t)(((F_CPU / I2C_FREQ_HZ) - 16) / 2);
    TWCR = (1 << TWEN);
}

void i2c_start(void)
{
    // Envoyer START
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    
    // Envoyer adresse + WRITE
    TWDR = (AHT20_ADDR << 1) | TW_WRITE;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    _delay_ms(40);

}

void i2c_stop(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void i2c_write(unsigned char data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

void i2c_read(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
}

void print_hex_value(char c)
{
    char hex[] = "0123456789ABCDEF";
    uart_tx(hex[(c >> 4) & 0x0F]);
    uart_tx(hex[c & 0x0F]);
}

void aht20_trigger_measurement(void)
{
    i2c_start();
    i2c_write(0xAC);
    i2c_write(0x33);
    i2c_write(0x00);
    i2c_stop();
    _delay_ms(80);
}

void aht20_read_data(void)
{
    uint8_t data[7];

    // START + READ
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    TWDR = (AHT20_ADDR << 1) | TW_READ;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    for (int i = 0; i < 6; i++) {
        i2c_read();
        data[i] = TWDR;
    }

    // dernier byte NACK
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    data[6] = TWDR;

    i2c_stop();

    uint32_t Hraw = ((uint32_t)data[1] << 12) |
                    ((uint32_t)data[2] << 4) |
                    ((uint32_t)data[3] >> 4);

    uint32_t Traw = ((uint32_t)(data[3] & 0x0F) << 16) |
                    ((uint32_t)data[4] << 8) |
                    ((uint32_t)data[5]);

    // Signal transformation p.9
    float humidity = (Hraw * 100.0) / 1048576.0;
    float temp = (Traw * 200.0 / 1048576.0) - 50.0;

    add_measure(temp, humidity);

    float t_avg = round2(average(t_buf));
    float h_avg = round2(average(h_buf));

    char t_str[10], h_str[10];
    dtostrf(t_avg, 4, 2, t_str);
    dtostrf(h_avg, 4, 2, h_str);

    uart_printstr("Temperature: ");
    uart_printstr(t_str);
    uart_printstr("C, Humidity: ");
    uart_printstr(h_str);
    uart_printstr("%\r\n");
}


int main(void)
{
    uart_init();
    i2c_init();
    
    _delay_ms(100);
    
    while (1)
    {
        aht20_trigger_measurement();
        aht20_read_data();
        _delay_ms(1000);
    }
    
    return 0;
}