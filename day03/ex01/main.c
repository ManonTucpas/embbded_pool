#include <avr/io.h>
#include <util/delay.h>

#define LED_R PD5
#define LED_B PD3
#define LED_G PD6

#define ON_RED PORTD |= (1 << LED_R)
#define ON_BLUE PORTD |= (1 << LED_B)
#define ON_GREEN PORTD |= (1 << LED_G)

#define OFF_GREEN PORTD &= ~(1 << LED_G)
#define OFF_BLUE PORTD &= ~(1 << LED_B)
#define OFF_RED PORTD &= ~(1 << LED_R)
/**
    yellow: R & G
    cyan : G & B
    magenta : R & B
    white R & B & G
 */
void setup()
{
    // Setup LEDs as Output
    DDRD = (1 << LED_B) | (1 << LED_G) | (1 << LED_R);
}

int main()
{
    setup();
    while (1)
    {
        // Red
        ON_RED;
        _delay_ms(1000);
        OFF_RED;
        // Green
        ON_GREEN;
        _delay_ms(1000);
        OFF_GREEN;
        // Blue
        ON_BLUE;
        _delay_ms(1000);
        OFF_BLUE;
        // Yellow
        ON_RED;
        ON_GREEN;
        _delay_ms(1000);
        OFF_RED;

        // Cyan
        ON_BLUE;
        _delay_ms(1000);
        OFF_GREEN;
        // Magenta
        ON_RED;
        _delay_ms(1000);
        // White
        ON_GREEN;
        _delay_ms(1000);
        OFF_BLUE;
        OFF_GREEN;
        OFF_RED;
    }
    return (0);
}