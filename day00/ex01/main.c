#include <avr/io.h>
#include <util/delay.h>

/* Ref: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
    cf page 61

    - ATMega a plusieurs ports GPIO: PORTB (PB0 - PB7), PORTC (PC0 - PC6), PORTD (PD0 -PD7)
    - Chaque port a 3 registres
        1. DDRx donnne la direction : input or outpout
        2. PORTx defini l'etat:
            output: HIGH ou LOW
            input: active/desactive pullup
        3. PINx permet de lire l'etat du pin

 */
void setup()
{
    // Setup the LED PIN PB0 as OUTPUT
    // `1 << PB0` -> masque pour mettre le bit PB0 a 1
    // PB0 est en OUTPUT
    DDRB |= (1 << PB0);
}

int main(void)
{

    setup();

    while (1)
    {
        // PB0 a HIGH ==> LED allumee
        PORTB |= (1 << PB0);

        /* for blinking LED
         PORTB ^= (1 << PB0); // Reverse bit to blink
         _delay_ms(1000);
        */
    }

    return 0;
}