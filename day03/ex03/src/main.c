#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../include/rgb.h"
#include "../include/uart.h"
#include "../include/hexa.h"

#define LED_R PD5
#define LED_B PD3
#define LED_G PD6

// Variables globales
volatile char buffer[6];
volatile uint8_t idx = 0;
enum
{
    WAIT_HASH,
    READ_HEX
} state = WAIT_HASH;

void setup()
{
    // Setup LEDs as Output
    DDRD |= (1 << LED_B) | (1 << LED_G) | (1 << LED_R);
}

// ISR : Interrupt Service Routine
//  p.74 table 12-6
ISR(USART_RX_vect)
{
    char c = UDR0; // lire vite le registre

    if (state == WAIT_HASH) // on attend de tomber sur '#'
    {
        if (c == '#')
        {
            idx = 0;
            state = READ_HEX;
        }
        return;
    }
    buffer[idx++] = c; // on rempli le buffer

    if (idx == 6)
    {
        uint8_t R = pair_to_byte(buffer[0], buffer[1]);
        uint8_t G = pair_to_byte(buffer[2], buffer[3]);
        uint8_t B = pair_to_byte(buffer[4], buffer[5]);
        set_rgb(R, G, B);
        state = WAIT_HASH;

    }
}

int main()
{
    setup();
    init_rgb();
    uart_init();
    sei();
    while (1)
    {
    }
    return (0);
}