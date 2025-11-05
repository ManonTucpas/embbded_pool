#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../include/uart.h"
#include "../include/itoa.h"

#define CANAL_POT 0
#define CANAL_LDR 1
#define CANAL_NTC 2

uint16_t adc_value;

// La valeur maximum sera 1023 car 10bits
char buffer[5];

/*
    ADC_POT ==> PC0 , ADC0 :
    PC0 ADC0 (ADC Input Channel 0)
    PCINT8 (Pin Change Interrupt 8)

    ADC_NTC ==> PC2 ADC2
    ADC_LDR ==> PC1 ADC1

*/
void adc_init()
{
    // Table 24-3. Voltage Reference Selections for ADC
    ADMUX |= (1 << REFS0); // AVCC with external capacitor at AREF pin

    // Table 24-4. Input Channel Selections
    // ADMUX est le registre qui permet de selectionner le canal ADC sur lequel on veut ecouter
    ADMUX = (ADMUX & ~0x07); // on efface les bits de selection du canal (bits 0-2)
                             // ADMUX |=  CANAL_POT; // On met le numero du canal qu'on veut

    // ADC Enable, de base il est eteint pour que le micro controlleur consomme moins
    ADCSRA |= (1 << ADEN);

    // ADC Prescaler Selections
    // Input clock frequency: doit etre entre 50kHz et 200kHz
    // 16000000 / 128 = 125kHz
    ADCSRA |= (1 << ADPS1) | (1 << ADPS0) | (1 << ADPS2);

    // Faire une premiere conversion car la premiere est la plus lente
    ADCSRA |= (1 << ADSC);
}

uint16_t read_adc(uint8_t channel)
{
    ADMUX &= 0xF0;         // Effacer le canal qui etait, on remet les 3 bits a 0
    ADMUX |= channel;      // Definit le nouveau canal a lire
    ADCSRA |= (1 << ADSC); // Commence la conversion
    while (ADCSRA & (1 << ADSC))
        ; // Attend que la conversion soit prete

    uint16_t result;
    result = ADCL; //on le lit en premier
    result |= (ADCH << 8);
    return result;

    // registre 16 bits qui combine automatiquement ADCL et ADCH
    // return ADCW;
}

int main()
{
    uart_init();
    adc_init();
    while (1)
    {
        for (int i = 0; i < 3; i++)
        {
            adc_value = read_adc(i);
            itoa(adc_value, buffer, 10);
            uart_printstr(buffer);
            uart_printstr(", ");
        }
        uart_printstr("\n\r");

        _delay_ms(20);
    }
    return (0);
}