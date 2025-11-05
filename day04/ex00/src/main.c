#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t flag = 0;

/**
 * Une interrpution sera lie au fait de presser le bouton
 * Lorsque cette interruption est detectee, il faut changer l'etat de la LED
 */

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
    // Quand on appuie on passe sur un circuit ferme : on pase de HIGH a LOW -> Falling edge
    EICRA |= (1 << ISC01); // The falling edge of INT0 generates an interrupt request. Table13-2

    // EIMSK – External Interrupt Mask Register p.81
    EIMSK |= (1 << INT0); // active l'interruption INT0
}

// Interupt vector pour SW1 is EXT_INT0 - cf shema elec & p.67
ISR(INT0_vect)
{
    flag = 1;
}

int main()
{
    setup();
    sei();
    while (1)
    {
        if (flag == 1)
        {
            // On desactive l'interruption
            EIMSK &= ~(1 << INT0);

            // Inverse l'etat de la LED
            PORTB ^= (1 << PB0);

            _delay_ms(200);
            // EIFR – External Interrupt Flag Register
            EIFR |= (1 << INTF0); // Efface le flag d'interuption, evite les interruptions parasites
            EIMSK |= (1 << INT0); // On re-active l'interruption INT0
            flag = 0;
        }
    }
    return (0);
}
