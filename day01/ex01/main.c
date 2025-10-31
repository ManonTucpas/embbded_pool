#include <avr/io.h>

void setup()
{
    // Setup LED2 as Output
    DDRB |= (1 << PB1);
    // DDRB |= (1 << PB2);

    /*
        Dans le manuel: page 140
        TCCRIA: Timer/Counter1 Control Register A
        Compare OUTPUT mode et si on active COM1x0 -> on toggle le OUTPUT associe.
        PB1 <=> OC1A && PB2 <=> OC1B   cf schema ele
    */
    TCCR1A = (1 << COM1A0);
    // TCCR1A = (1 << COM1A0) | (1 << COM1B0);

    /*
        Registre TCCR1B (manuel: page 622):
            || ICN1 | ICES1 | - | WGM13 | WMGM12 | CS12 | CS11 | CS10 ||

        - CSxx ==> bit de selection du rapport de frequence
            CS12, CS11, et CS10, pour le timer1
        - WGMxx ==> registre qui permet de definir comment compter au niveau du timer,
            ici on veut CTC donc on active WGM12, remise a zero du timer quand le nombre de cycles defni est atteint
    */

    /*
        On veut diviser par 1024 pour avoir 15 000 cycles par seconde
        Pour diviser par 1024 on doit on doit mettre les bits CS12 ET CS10 a 1  | cf tableau page 143
        on a donc 15k cycles/sec
    */
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);

    /*
        On stocke la valeur a laquelle on veut qu'une interruption soit levee.
        Quand le timer aura atteint cette valeur le bit OCF1A, ainsi au'une interruption seront levees
            ==> On veut une interruption toutes les 0.5 sec donc 15 000 / 2
    */
    OCR1A = 7812;
}

int main()
{
    setup();

    while (1)
    {
    }
    return (0);
}