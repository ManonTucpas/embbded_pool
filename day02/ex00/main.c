
//https://hekilledmywire.wordpress.com/2011/01/05/using-the-usartserial-tutorial-part-2/

#include <avr/io.h>
#include <util/delay.h>

#define BAUDRATE 115200
// Calcul du prescaler pour le registre UBRR en mode double speed (U2X0 = 1)
// Formule : UBRR = (F_CPU / (8 * BAUDRATE)) - 1
// Avec F_CPU = 16MHz : UBRR = 16 (erreur de 0%)
#define BAUD_PRESCALLER ((F_CPU / (BAUDRATE * 8UL)) - 1)

// Init UART - Manuel p.185
void uart_init()
{
    // Active le mode double speed (U2X0) dans le registre UCSR0A
    // Ce mode divise par 8 au lieu de 16, réduisant l'erreur de baud rate à 0%
    UCSR0A = (1 << U2X0);
     
    // Configure le baud rate en deux parties (registre 16 bits)
    UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
    UBRR0L = (uint8_t)(BAUD_PRESCALLER);

    /// Active le récepteur (RXEN0) et l'émetteur (TXEN0) dans UCSR0B
    // RXEN0 = Receiver Enable (permet de recevoir des données)
    // TXEN0 = Transmitter Enable (permet d'envoyer des données)
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Configure le format de trame dans UCSR0C : 8N1
    // UCSZ00 et UCSZ01 à 1 = 8 bits de données
    // Pas de bit de parité (par défaut)
    // 1 bit de stop (par défaut)
    // Configuration : 8 data bits, No parity, 1 stop bit
    UCSR0C = ((1 << UCSZ00) | (1 << UCSZ01));
}

// Write a char the serial port
void uart_tx(char c)
{
    // Attend que le buffer de transmission soit vide
    // UDRE0 (USART Data Register Empty) = 1 quand le registre UDR0 est prêt
    // La boucle bloque tant que UDRE0 = 0 (buffer plein)
    // Le bit UDRE0 dans UCSR0A indique si on peut écrire dans UDR0
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    
    // Écrit le caractère dans le registre de données USART
    // Dès qu'on écrit dans UDR0, le hardware UART envoie automatiquement
    // le caractère sur la ligne TX (transmission)
    UDR0 = c;
}

int main()
{
    uart_init();
    while (1)
    {
        uart_tx('Z');
        _delay_ms(1000);
    }
    return (0);
}