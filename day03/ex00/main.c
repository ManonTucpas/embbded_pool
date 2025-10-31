#include <avr/io.h>
#include <util/delay.h>

#define LED_R PD5
#define LED_B PD3
#define LED_G PD6

/**
 * PD3 BLUE
 * INT1 (External Interrupt 1 Input)
    OC2B (Timer/Counter2 Output Compare Match B Output)
    PCINT19 (Pin Change Interrupt 19)

    PD5 RED
    T1 (Timer/Counter 1 External Counter Input)
    OC0B (Timer/Counter0 Output Compare Match B Output)
    PCINT21 (Pin Change Interrupt 21)

    PD6 GREEN
    AIN0 (Analog Comparator Positive Input)
    OC0A (Timer/Counter0 Output Compare Match A Output)
    PCINT22 (Pin Change Interrupt 22

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
        PORTD |= (1 << LED_R);
        _delay_ms(1000);
        PORTD &= ~(1 << LED_R);
        PORTD |= (1 << LED_G);
        _delay_ms(1000);
        PORTD &= ~(1 << LED_G);
        PORTD |= (1 << LED_B);
        _delay_ms(1000);
        PORTD &= ~(1 << LED_B);
    }
    return (0);
}