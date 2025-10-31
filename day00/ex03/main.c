#include <avr/io.h>
#include <util/delay.h>


// Store the state of the push button
unsigned char currentState = 1; // État actuel (1 = relâché)
unsigned char prevState = 1; // etat precredent 

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

    while (1){
    
        // On lit l'etat actuel du bouton sur PD2
        currentState = (PIND & (1 << PD2)) ? 1 : 0;
        
       
        if (prevState == 1 && currentState == 0) // bouton pressee
        {
            _delay_ms(50);
            if (!(PIND & (1 << PD2))) // bouton pressee
            {
                PORTB ^= (1 << PB0); 
            }
        }
        prevState = currentState;
    }
    return (0);
}