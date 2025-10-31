#include <avr/io.h>
#include <util/delay.h>
#include "../include/rgb.h"

#define LED_R PD5
#define LED_B PD3
#define LED_G PD6

void setup()
{
    // Setup LEDs as Output
    DDRD |= (1 << LED_B) | (1 << LED_G) | (1 << LED_R);
}

void wheel(uint8_t pos)
{
    pos = 255 - pos;
    if (pos < 85)
    {
        set_rgb(255 - pos * 3, 0, pos * 3);
    }
    else if (pos < 170)
    {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    }
    else
    {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}

int main()
{
    setup();
    init_rgb();

    while (1)
    {
        for (int i = 0; i < 255; i++)
        {
            wheel(i);
           _delay_ms(20); // Permet de voir le rainbow
        }
    }
    return (0);
}