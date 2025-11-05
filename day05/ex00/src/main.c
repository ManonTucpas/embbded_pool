#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../include/uart.h"

/*
    ADC_POT ==> PC0 , ADC0 :
    PC0 ADC0 (ADC Input Channel 0)
    PCINT8 (Pin Change Interrupt 8)

    Le bit de réduction de puissance ADC, PRADC, dans « Réduction de la consommation d'énergie » à la page 51 doit être désactivé en
écrivant un zéro logique pour activer l'ADC.

 L'ADC est
activé en réglant le bit ADC Enable, ADEN dans ADCSRA. Les sélections de référence de tension et de canal d'entrée
ne prendront effet qu'une fois ADEN réglé

L'ADC génère un résultat de 10 bits qui est présenté dans les registres de données ADC, ADCH et ADCL. Par défaut,
le résultat est présenté aligné à droite, mais il peut être présenté aligné à gauche en activant le bit ADLAR dans
ADMUX.
Si le résultat est aligné à gauche et qu'une précision de 8 bits maximum est requise, il suffit de lire ADCH.[...] Lorsque ADCH est lu, l'accès de l'ADC aux registres ADCH et ADCL est réactivé.

After the conversion is complete (ADIF is high), the conversion result can be found in the ADC Result Registers
(ADCL, ADCH).
*/
void init_ADC()
{
    // Table 24-3. Voltage Reference Selections for ADC
    ADMUX |= (1 << REFS0); // AVCC with external capacitor at AREF pin

    ADMUX |= (1 << ADLAR); // pour aligner a gauche et definir 8bits conversion results

    // Table 24-4. Input Channel Selections
    //  On ne fait rien car MUX3..0 pour ADC0 est 0000

    // ADC Enable
    ADCSRA |= (1 << ADEN);

    // ADC Prescaler Selections
    // Input clock frequency doit etre entre 50kHz et 200kHz
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
        uart_print_hex(read_ADC());
        uart_printstr("\r\n");
        _delay_ms(20);
    }
    return (0);
}