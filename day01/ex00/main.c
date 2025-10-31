#include <avr/io.h>

void setup()
{
    // Setup LED2 as Output
    DDRB |= (1 << PB1); // BIT 2

    // Setup register for timer
    // Put the bit WGM10 to 0 (in the TCCR1A register)
    // Put the bit WGM11 to 0 (in the TCCR1A register)
}

void delay_500_ms()
{
    /*  Notre microcontrolleur execute 16 000 000 cycles par seconde
        On veut faire executer a notre controleur 8 000 000 de cycles == 0,5 secondes
        pour simuler une pause
    */
    for (uint32_t i = 0; i < (F_CPU / 64); i++)
    {
        /*
            - boucle for = ~8 cycles
            - instanciation = ~8 cycles
            - comparaison = ~8 cycles
            - incrementation = ~8 cycles
                == ~32 cycles
                32 * (F_CPU / 64) = 8 000 000
        */
    }
}

int main()
{
    setup();

    while (1)
    {
        PORTB ^= (1 << PB1); // Reverse bit for LED blinking
        delay_500_ms();
    }
    return (0);
}