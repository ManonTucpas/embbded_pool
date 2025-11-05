#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*
    TIMER 1 - Contole la luminisote de LED2 ==> OC1A
    Dans le manuel: page 140
    TCCRIA: Timer/Counter1 Control Register A

*/
void init_timer1()
{
    // Fast PWM - Clear OC1A on Compare Match
    TCCR1A |= (1 << WGM11) | (1 << COM1A1);

    // Prescaler / 8 => 2 000 000 cycles/sec | cf tableau page 143
    // Frequence PWM => 2000000 / ICR1 = ~10 000 Hz : assez eleve pour que l'oeil ne voit pas le clignotement
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11);

    /*
        On stocke la valeur a laquelle on veut qu'une interruption soit levee.
        Quand le timer aura atteint cette valeur le bit OCF1A, ainsi qu'une interruption seront levees
    */
    // TOP pour ~7.8 kHz
    ICR1 = 200;

    // Duty au départ (0%): OCR1A = 0
    OCR1A = 0;
}

/*
    Timer0 - Génère une interruption toutes les 5 ms pour modifier le duty cycle
*/
void init_timer0()
{
    // Setup timer & duty cycle
    TCCR0A |= (1 << WGM01); // Mode CTC : WGM01 = 1

    // Choix du prescaler / 1024 → CS02=1, CS01=0, CS00=1
    // 15 625 cycles dans une seconde
    // 15625 / 200  = 78 =>  0.005s
    TCCR0B |= (1 << CS02) | (1 << CS00);

    // Valeur TOP = 78 pour environ 5 ms
    OCR0A = 78;

    // Active interruption Compare Match A
    TIMSK0 = (1 << OCIE0A);
}

void setup()
{
    // Init timers
    init_timer1();
    init_timer0();

    // Setup LED2 as Output
    DDRB |= (1 << PB1);
}

// Interupt vector pour timer0
// Le timer cree des interruptions toutes les 5 ms, afin de changer la valeur de la LED
// 1 cycle complet : 50ms to top, 50 ms top to bottom
ISR(TIMER0_COMPA_vect)
{
    static int sens = 1;
    static int duty = 0;

    duty = duty + sens;

    // On inverse le sens pour aller de 0 a 100 & de 100 a 0
    if (duty >= 100 || duty <= 0)
        sens = -sens;

    OCR1A = (duty * ICR1) / 100; // ajuste la luminosite
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