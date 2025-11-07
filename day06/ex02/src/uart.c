#include "../include/uart.h"

// Init UART - Manuel p.185
void uart_init()
{
    UCSR0A = (1 << U2X0);

    // set baud rate to 115200
    UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
    UBRR0L = (uint8_t)(BAUD_PRESCALLER);

    // Active réception, transmission  p.201
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) ;
    // Enable interrupt on data recieved
	// UCSR0B |= (1 << RXCIE0);
    
    UCSR0C = ((1 << UCSZ00) | (1 << UCSZ01));
}

// // Write a char the serial port
void uart_tx(char c)
{
    // Attend que le buffer de transmission soit vide
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

char uart_rx(void)
{
    /* Wait for data to be received */
    while(!(UCSR0A & (1<<RXC0)));

    /* Get and return received data from buffer */
    return (char)UDR0;
}

void uart_printstr(const char *str)
{
   unsigned char i = 0;

	while (*(str + i)) {
		uart_tx(*(str + i));
		i++;
	}
}

// À ajouter dans uart.cEn une phrase : On découpe un byte (8 bits) en deux morceaux de 4 bits pour obtenir les deux chiffres hexadécimaux ! ✂️
void uart_print_hex(uint8_t value)
{
    // On recupere les 4 bits de poids fort dans high, et les 4 bits de poids faible daans low
    // ca permet de recuperer deux chiffre hexa de 4bits
    uint8_t high = (value >> 4) & 0x0F;
    uint8_t low = value & 0x0F;
    
    // Convertit high en caractère hexa
    char high_char;
    if (high < 10) // chiffre entre 0-9
    {
        high_char = '0' + high;
    }
    else //lettre A-F
    {
        high_char = 'a' + (high - 10);
    }
    uart_tx(high_char);

    char low_char;
    if (low <10)
    {
        low_char = '0' + low;
    }
    else
    {
        low_char = 'a' + (low - 10);
    }
    uart_tx(low_char);
  
}




