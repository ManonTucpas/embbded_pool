#include "../include/rgb.h"

// Initializes the timers
void init_rgb()
{
    TCCR2A = 0;
    TCCR2B = 0;
    TCCR0A = 0;
    TCCR0B = 0;

    // Timer0 controle PD5 & PD6 LEDs - p.117
    // TCCR0A – Timer/Counter Control Register A & TCCR0B – Timer/Counter Control Register B
    // PD6(OC0A) => OCR0A – Output Compare Register A
    // PD5(OC0B) => OCR0B – Output Compare Register B
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1); // PWM mode: Clear on compare match for OC0A & OC0B - p.114
    TCCR0A |= (1 << WGM01) | (1 << WGM00);   // Fast PWM - p.115
    TCCR0B |= (1 << CS00) | (1 << CS01);     // Prescaler /64 - p.117

    // Timer2 controle PD3
    // PD3(OC2B) => TCCR2B – Timer/Counter Control Register B
    TCCR2A |= (1 << COM2B1);               // PWM mode: Clear on compare match for OC2B - p.164
    TCCR2A |= (1 << WGM20) | (1 << WGM21); // fast PWM, non inverted
    TCCR2B |= (1 << CS22);                 // Prescaler /64 - p.165
}

// Set the LED's color
void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    // OCRnx sont sur 8 bits donc la valeur ira de 0 a 255
    // En Fast PWM TOP = 255 | BOTTOM = 0
    OCR0B = r;
    OCR0A = g;
    OCR2B = b;
}