#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../include/rgb.h"
#include "../include/uart.h"
#include "../include/hex.h"

#define LED_R PD5
#define LED_B PD3
#define LED_G PD6

#define BUF_LEN 7
char buf[BUF_LEN];
uint8_t pos = 0;
volatile char c;

void setup()
{
    // Setup LEDs as Output
    DDRD |= (1 << LED_B) | (1 << LED_G) | (1 << LED_R);
    init_rgb();
    uart_init();
}

// ISR : Interrupt Service Routine
//  p.74 table 12-6
ISR(USART_RX_vect)
{
    c = UDR0;

    UDR0 = c;

    if (c == '\r')
        UDR0 = '\n';

    if (c == '\r')
    { // screen envoie \r
        if (pos == 7 && buf[0] == '#')
        {

            // Verif if hexa valeur
            for (int i = 1; i <= 6; i++)
            {
                if (!is_hex(buf[i]))
                {
                    pos = 0;
                    return;
                }
            }
            uint8_t r = pair_to_byte(buf[1], buf[2]);
            uint8_t g = pair_to_byte(buf[3], buf[4]);
            uint8_t b = pair_to_byte(buf[5], buf[6]);
            set_rgb(r, g, b);
        }
        pos = 0;
        return;
    }

    if (pos < BUF_LEN)
    {
        buf[pos++] = c;
    }
    else
        pos = 0;
}

int main()
{
    setup();
    sei();

    while (1)
    {
    }
    return (0);
}