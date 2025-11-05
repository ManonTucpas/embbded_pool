#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../include/uart.h"
#include "../include/itoa.h"

#define TEMP_CHANNEL 8
uint16_t temperature;
char buffer[5];

void setup()
{
    uart_init();

    ADMUX |= (1 << REFS0) | (1 << REFS1); // 1.1 volt p.257

    ADMUX &= 0xF0;         // Effacer le canal qui etait, on remet les 3 bits a 0
    ADMUX |= TEMP_CHANNEL; // enable ADC8 internal temperature sensor

    ADCSRA |= (1 << ADEN); // enable ADC

    ADCSRA |= (1 << ADPS1) | (1 << ADPS0) | (1 << ADPS2); // input clock frequency

}

int read_temp()
{
    // lance une conversion
    ADCSRA |= (1 << ADSC);

    // wait for completion
    while (ADCSRA & (1 << ADSC))
        ;
      
    // Lis dans le bon ordre avec uint16_t
    uint16_t adc = ADCL;
    adc |= (ADCH << 8);
    
    // Affiche la valeur ADC en décimal
    char buf[10];
    itoa(adc, buf, 10);
    uart_printstr("ADC brut: ");
    uart_printstr(buf);
    uart_printstr("\n\r");
    
    return  (adc * 100L - 32431) / 122;
}

int main()
{
    setup();
    while (1)
    {
        temperature = read_temp();

        itoa(temperature, buffer, 10);
        uart_printstr(buffer);

            _delay_ms(2000);
    }
}