#include <avr/io.h>

/*
    DUTY CYCLE :
    Phenomene periodique a deux etats:
    rapport entre la duree de l'etat actif et la periode
    10% Duty cycle => 10% de la periode sera ON | 90% OFF
*/ 


void setup()
{
    // Setup LED2 as Output
    // Si necessaire: cf explications ex01
    DDRB |= (1 << PB1);
    
    // cf p142 tableau 16-4
    // COM1A1 a 1: mode clear on compare match
    // Quand timer atteint la valeur OCR1A, la sortie OC1A passe a 0 ==> LED to LOW
    // WGM13=1, WGM12=1, WGM11=1, WGM10=0 => Mode 14: Fast PWM avec TOP=ICR1
    // Le timer compte de 0 à ICR1, puis revient INSTANTANÉMENT à 0 (pas d'aller-retour)
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
  
  
    TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS12) | (1 << CS10);
    // Definit la valeur maximale du compteur, frequence
    ICR1 = 15625;

    // Definit quand le signal passe HIGH a LOW, duty cycle
    OCR1A = 1562;
}

int main()
{
    setup();
    while (1) {}
    return (0);
}