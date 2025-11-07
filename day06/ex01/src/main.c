#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include "../include/uart.h"


// lib twi.h
// https://www.nongnu.org/avr-libc/user-manual/group__util__twi.html

// AHT20 Datasheet
// https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf

// I2C temp sensor PINS
// PC4(ADC4/SDA)  => Data I/O
// PC5(ADC5/SCL)  => Clock


#define TW_READ 1
#define TW_WRITE 0
#define I2C_FREQ_HZ 100000UL
#define AHT20_ADDR 0x38

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
    
    // START en mode READ
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    
    TWDR = (AHT20_ADDR << 1) | TW_READ;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    
    // Lire 6 bytes avec ACK
    for (int i = 0; i < 6; i++)
    {
        i2c_read();
        data[i] = TWDR;
    }
    
    // Dernier byte avec NACK
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    data[6] = TWDR;
    
    i2c_stop();
    
    // Afficher avec print_hex_value()
    for (int i = 0; i < 7; i++)
    {
        print_hex_value(data[i]);
        if (i < 6)
            uart_tx(' ');
    }
    uart_tx('\r');
    uart_tx('\n');
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