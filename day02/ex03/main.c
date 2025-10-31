
// https://hekilledmywire.wordpress.com/2011/01/05/using-the-usartserial-tutorial-part-2/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUDRATE 115200
#define BAUD_PRESCALLER ((F_CPU / (BAUDRATE * 8UL)) - 1)

// Init UART - Manuel p.185
void uart_init()
{
    UCSR0A = (1 << U2X0);

    UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
    UBRR0L = (uint8_t)(BAUD_PRESCALLER);

    // Active réception, transmission et interruption RX, p.201
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) |(1 << RXCIE0);
    UCSR0C = ((1 << UCSZ00) | (1 << UCSZ01));
    
    // ========== sei() : Set Enable Interrupts ==========
    // Sans sei(), AUCUNE interruption ne peut se déclencher
    // C'est l'interrupteur général de toutes les interruptions
    sei();
}

// Write a char the serial port
void uart_tx(char c)
{
    // Attend que le buffer de transmission soit vide
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

// ISR : Interrupt Service Routine
//  p.74 table 12-6
ISR(USART_RX_vect)
{
    // On recupere le char recu
    char c = UDR0;
     if (c >= 'a' && c <= 'z')
        c -= 32;
    else if (c >= 'A' && c <= 'Z')
        c += 32;
    // On l'affiche
    uart_tx(c);
}

int main()
{
    uart_init();
    while (1)
    {
    }
    return (0);
}