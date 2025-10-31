#include <avr/io.h>
#include <util/delay.h>

// Store the state of the push button
unsigned char currentStateSW1 = 1; // État actuel (1 = relâché)
unsigned char prevStateSW1 = 1;    // etat precredent

unsigned char currentStateSW2 = 1; // État actuel (1 = relâché)
unsigned char prevStateSW2 = 1;    // etat precredent

int counter = 0;

void setup()
{
    // Setup LED1 as Output
    DDRB |= (1 << PB0); // BIT 1
    // Setup LED2 as Output
    DDRB |= (1 << PB1); // BIT 2
    // Setup LED3 as Output
    DDRB |= (1 << PB2); // BIT 3
    // Setup LED4 as Output
    DDRB |= (1 << PB4); // BIT

    // Setup SW1 button PD2 as input
    DDRD &= ~(1 << PD2);
    // Setup SW2 button PD4 as input
    DDRD &= ~(1 << PD4);
    // Active SW1 pull up interne sur PD2
    PORTD |= (1 << PD2);
    // Active SW2 pull up interne sur PD4
    PORTD |= (1 << PD4);
}

void display(int counter)
{
    if (counter & 1)
        PORTB |= (1 << PB0);
    if (counter & 2)
        PORTB |= (1 << PB1);
    if (counter & 4)
        PORTB |= (1 << PB2);
    if (counter & 8)
        PORTB |= (1 << PB4);
}

void shutoff_led()
{
    PORTB &= ~(1 << PB0);
    PORTB &= ~(1 << PB1);
    PORTB &= ~(1 << PB2);
    PORTB &= ~(1 << PB4);
}

int main()
{
    setup();

    while (1)
    {
        // On lit l'etat actuel du bouton sur PD2
        currentStateSW1 = (PIND & (1 << PD2)) ? 1 : 0;
        currentStateSW2 = (PIND & (1 << PD4)) ? 1 : 0;

        if ((prevStateSW1 == 1 && currentStateSW1 == 0) ||
            (prevStateSW2 == 1 && currentStateSW2 == 0)) // bouton pressee
        {
            _delay_ms(50);
            if (!(PIND & (1 << PD2))) // bouton pressee
            {
                counter++;
            }
            if (!(PIND & (1 << PD4)))
            {
                if (counter > 0)
                    counter--;
            }
            _delay_ms(50);
            // LED Handle
            display(counter);
        }
        _delay_ms(50);
        prevStateSW1 = currentStateSW1;
        prevStateSW2 = currentStateSW2;
        // Set LED OFF
        shutoff_led();
    }
    return (0);
}