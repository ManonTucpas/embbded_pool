
//https://hekilledmywire.wordpress.com/2011/01/05/using-the-usartserial-tutorial-part-2/

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

    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = ((1 << UCSZ00) | (1 << UCSZ01));
}

// Write a char the serial port
void uart_tx(char c)
{
    // Attend que le buffer de transmission soit vide
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = c;
}

// Receive a char from serial port
char uart_rx(void)
{
    /* Wait for data to be received */
    while(!(UCSR0A & (1<<RXC0)));

    /* Get and return received data from buffer */
    return UDR0;
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
//  p.74 table 12-6
ISR(TIMER1_COMPA_vect)
{
    // On recupere le char recu
   char c = uart_rx();
   // On l'affiche
   uart_tx(c);
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