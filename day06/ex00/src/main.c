#include <avr/io.h>
#include <util/twi.h>
#include "../include/uart.h"

#define TW_READ 1
#define TW_WRITE 0
#define I2C_FREQ_HZ 100000UL
#define AHT20_ADDR 0x38 // Adresse I2C du capteur AHT20 (selon datasheet p.8)

// lib twi.h
// https://www.nongnu.org/avr-libc/user-manual/group__util__twi.html

// AHT20 Datasheet
// https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf

// I2C temp sensor PINS
// PC4(ADC4/SDA)  => Data I/O
// PC5(ADC5/SCL)  => Clock

void i2c_init(void)
{
    // 1. Prescaler = 1 (TWPS1:0 = 00)
    TWSR &= ~((1 << TWPS0) | (1 << TWPS1));

    // 2. Calcul et configuration de TWBR pour obtenir 100kHz
    // Formule: SCL = F_CPU / (16 + 2*TWBR*prescaler)
    // Donc: TWBR = ((F_CPU/I2C_FREQ_HZ) - 16) / 2
    TWBR = (uint8_t)(((F_CPU / I2C_FREQ_HZ) - 16) / 2);

    // 3. Activer le module TWI (I2C)
    TWCR = (1 << TWEN);
}

void i2c_start()
{
    // 1. Envoyer la condition START
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    // 2. Attendre que le START soit envoyé (TWINT devient 1)
    while (!(TWCR & (1 << TWINT)))
        ;

    // 3. Charger l'adresse du slave + bit R/W dans TWDR
    // Pour écrire: (AHT20_ADDR << 1) | TW_WRITE
    // Pour lire: (AHT20_ADDR << 1) | TW_READ
    TWDR = (AHT20_ADDR << 1) | TW_WRITE;

    // 4. Envoyer l'adresse
    TWCR = (1 << TWINT) | (1 << TWEN);

    // 5. Attendre que l'adresse soit envoyée
    while (!(TWCR & (1 << TWINT)))
        ;

    // 6. Retourner le status (optionnel pour débuggage)
    uint8_t status = TWSR & 0xF8;
    uart_print_hex(status); // 0x18SLA+W transmis, ACK reçu

    // On peut l'envoyer via UART pour vérifier
}

void i2c_stop(void)
{
    // STOP
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}
int main()
{
    uart_init();
    i2c_init();
    i2c_start();
    i2c_stop();
    while (1)
    {
    }
    return (0);
}
