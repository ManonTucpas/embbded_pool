#ifndef UART_H
# define UART_H

# include <avr/io.h>
# include <util/delay.h>
# include <avr/interrupt.h>

# define BAUDRATE 115200
# define BAUD_PRESCALLER ((F_CPU / (BAUDRATE * 8UL)) - 1)


void uart_init();

void uart_tx(char c);

char uart_rx(void);

ISR(USART_RX_vect);


#endif