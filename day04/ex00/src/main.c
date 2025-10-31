#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void setup()
{
    // LED D1
    DDRB |= (1 << PB0);

    // Setup button PD2 as input
    DDRD &= ~(1 << PD2);
    // Active pull up interne sur PD2
    PORTD |= (1 << PD2);

    // Setup interrupt SW1 <=> INT0, external interrupt
    // EICRA – External Interrupt Control Register A p.80
    EICRA |= (1 << ISC01) | (1 << ISC00); // The rising edge of INT0 generates an interrupt request. Table13-2

    // EIMSK – External Interrupt Mask Register p.81
    EIMSK |= (1 << INT0); // active l'interruption INT0
    
    // EIFR – External Interrupt Flag Register
    EIFR |= (1 << INTF0); // Effacce le flag d'interuption, evite les interruptions parasites
}

// Interupt vector pour SW1 is EXT_INT0 - cf shema elec & p.67
ISR(INT0_vect)
{
    // Inverse l'etat de la LED
    PORTB ^= (1 << PB0);
    _delay_ms(20);

}

int main()
{
    setup();
    sei();
    while (1)
    {
    }
    return (0);
}
