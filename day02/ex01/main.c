
//https://hekilledmywire.wordpress.com/2011/01/05/using-the-usartserial-tutorial-part-2/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUDRATE 115200
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

// // Write a char the serial port
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

void uart_printstr(const char *str)
{
    while (*str != 0x00)
    {
        uart_tx(*str);
        str++;
    }
}

// Initialisation du Timer1 pour interruption toutes les 2 secondes
void timer1_init()
{
    // Mode CTC : le timer se réinitialise quand il atteint OCR1A
    TCCR1B |= (1 << WGM12);

    // Prescaler 1024 (CS12 = 1, CS10 = 1)
    TCCR1B |= (1 << CS12) | (1 << CS10);

    // Calcul pour 2 secondes :
    // F_timer = 16 MHz / 1024 = 15625 Hz
    // OCR1A = 15625 × 2s = 31250
    OCR1A = 31250;

    // Active l'interruption de comparaison A du Timer1, p.144
    TIMSK1 |= (1 << OCIE1A);

    // ========== sei() : Set Enable Interrupts ==========
    // Sans sei(), AUCUNE interruption ne peut se déclencher
    // C'est l'interrupteur général de toutes les interruptions
    sei();
}

// ISR : Interrupt Service Routine
// Fonction spéciale appelée automatiquement par le hardware
// quand l'interruption TIMER1_COMPA se déclenche
// TIMER1_COMPA_vect : vecteur d'interruption du Timer1 Compare Match A
// Cette fonction s'exécute toutes les 2 secondes (selon OCR1A)
ISR(TIMER1_COMPA_vect)
{
    uart_printstr("Hello World!\n");
}

int main()
{

    uart_init();
    timer1_init();
    while (1)
    {
    }
    return (0);
}