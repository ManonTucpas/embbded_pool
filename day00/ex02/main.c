#include <avr/io.h>

void setup()
{
    // Setup LED as Output
    DDRB |= (1 << PB0);
    // Setup button PD2 as input
    DDRD &= ~(1 << PD2);
    // Active pull up interne sur PD2
    PORTD |= (1 << PD2);
}

int main()
{
    setup();

    while (1)
    {
        // On lit l'etat du boutton sur PD2 
        if ((PIND & (1 << PD2)))  // button relache
        {
            PORTB &= ~(1 << PB0); // eteint LED, on met le bit PB0 a 0
        }
        else // button presse
        {
            PORTB |= (1 << PB0); // allumre LED => on met le bit PB0 a 1
        }
    }
    return (0);
}