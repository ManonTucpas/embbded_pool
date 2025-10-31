#include <avr/io.h>
#include <util/delay.h>

#define OCR1A_BASE 1562
// Store the state of the push button
unsigned char currentStateSW1 = 1; // État actuel (1 = relâché)
unsigned char prevStateSW1 = 1;    // etat precredent

unsigned char currentStateSW2 = 1; // État actuel (1 = relâché)
unsigned char prevStateSW2 = 1;    // etat precredent

// On commence a 10%
uint8_t dutyCyclePercent = 1;

void setup()
{
    // Setup LED2 as Output
    DDRB |= (1 << PB1); // BIT 2

    // Setup SW1 button PD2 as input
    DDRD &= ~(1 << PD2);
    // Setup SW2 button PD4 as input
    DDRD &= ~(1 << PD4);
    // Active SW1 pull up interne sur PD2
    PORTD |= (1 << PD2);
    // Active SW2 pull up interne sur PD4
    PORTD |= (1 << PD4);

    // Setup timer & duty cycle
    TCCR1A = (1 << COM1A1) | (1 << WGM11);

    TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS12) | (1 << CS10);
    // Definit la valeur maximale du compteur, frequence
    ICR1 = 15625;
    // Definit quand le signal passe HIGH a LOW, duty cycle
    OCR1A = 1562;
}

int main()
{
    setup();

    while (1)
    {
        // On lit l'etat actuel des boutons
        currentStateSW1 = (PIND & (1 << PD2)) ? 1 : 0;
        currentStateSW2 = (PIND & (1 << PD4)) ? 1 : 0;

        if ((prevStateSW1 == 1 && currentStateSW1 == 0) ||
            (prevStateSW2 == 1 && currentStateSW2 == 0)) // boutons pressees
        {
            if (!(PIND & (1 << PD2))) // SW1 pressee
            {
                if (dutyCyclePercent <= 10)
                {
                    OCR1A = OCR1A_BASE * dutyCyclePercent;
                    dutyCyclePercent++;
                    
                }
            }
            if (!(PIND & (1 << PD4))) // SW2 pressee
            {
                if (dutyCyclePercent >= 1)
                {
                    OCR1A = OCR1A_BASE * dutyCyclePercent;
                    dutyCyclePercent--;
                }
            }
        }
        _delay_ms(50);
        prevStateSW1 = currentStateSW1;
        prevStateSW2 = currentStateSW2;
    }
    return (0);
}