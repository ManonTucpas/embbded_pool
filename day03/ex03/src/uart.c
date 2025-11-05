#include "../include/uart.h"

// Init UART - Manuel p.185
void uart_init()
{
    UCSR0A = (1 << U2X0);

    UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
    UBRR0L = (uint8_t)(BAUD_PRESCALLER);

    // Active réception, transmission et interruption RX, p.201
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) |(1 << RXCIE0);
    UCSR0C = ((1 << UCSZ00) | (1 << UCSZ01));

}
// // Write a char the serial port
void uart_tx(char c)
{
    // Attend que le buffer de transmission soit vide
    while (!(UCSR0A & (1 << UDRE0)))
    UDR0 = c;
}

char uart_rx(void)
{
    /* Wait for data to be received */
    while(!(UCSR0A & (1<<RXC0)));

    /* Get and return received data from buffer */
    return UDR0;
}

void uart_printstr(const char *str)
{
    while (*str != 0x00)
    {
        uart_tx(*str);
        str++;
    }
}



