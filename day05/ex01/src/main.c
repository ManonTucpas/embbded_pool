#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../include/uart.h"

#define CANAL_POT 0
#define CANAL_LDR 1
#define CANAL_NTC 2

/*
    ADC_POT ==> PC0 , ADC0 :
    PC0 ADC0 (ADC Input Channel 0)
    PCINT8 (Pin Change Interrupt 8)

    ADC_NTC ==> PC2 ADC2
    ADC_LDR ==> PC1 ADC1

*/
void init_ADC()
{
    // Table 24-3. Voltage Reference Selections for ADC
    ADMUX |= (1 << REFS0); // AVCC with external capacitor at AREF pin

    ADMUX |= (1 << ADLAR); // pour aligner a gauche et definir 8bits

    // Table 24-4. Input Channel Selections
    // ADMUX est le registre qui permet de selectionner le canal ADC sur lequel on veut ecouter
    ADMUX = (ADMUX & ~0x07); // on efface les bits de selection du canal (bits 0-2)
   // ADMUX |=  CANAL_POT; // On met le numero du canal qu'on veut
   
   
    // ADC Enable
    ADCSRA |= (1 << ADEN);

    // ADC Prescaler Selections
    // Input clock frequency: doit etre entre 50kHz et 200kHz
    // 16000000 / 128 = 125kHz
    ADCSRA |= (1 << ADPS1) | (1 << ADPS0) | (1 << ADPS2); // division factor 8
}

uint8_t read_ADC()
{
    // ADC Start Conversion,quand la conversion est terminee le bit revient a 0
    ADCSRA |= (1 << ADSC);

    // Attend la fin de la conversion (ADSC revient à 0)
    while (ADCSRA & (1 << ADSC))
        ;

    // Retourne la valeur 8 bits (registre ADCH car ADLAR = 1)
    return ADCH;
}

int main()
{
    uart_init();
    init_ADC();
    while (1)
    {
        // On set le canal a ADC0 -> ADC_POT
        ADMUX = (ADMUX & ~0x07) | CANAL_POT;
        uart_print_hex(read_ADC());
        uart_printstr(", ");
        
        // On change le canal pour ADC1 -> ADC_LDR   
        ADMUX = (ADMUX & ~0x07) | CANAL_LDR;
        uart_print_hex(read_ADC());
        uart_printstr(", ");
        
        // On change le canal pour ADC2 -> ADC_NTC
        ADMUX = (ADMUX & ~0x07) | CANAL_NTC;
        uart_print_hex(read_ADC());
        uart_printstr("\n\r");
         
        _delay_ms(20);
    }
    return (0);
}