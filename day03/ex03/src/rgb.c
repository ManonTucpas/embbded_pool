#include "../include/rgb.h"

// Initializes the timers
void init_rgb()
{

    // Timer0 -> PD6(OC0A), PD5(OC0B) : 
    TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00) | (1 << COM0B1); //Fast PWM non inversé,
    TCCR0B |=  (1 << CS00)  | (1 << CS01); // prescaler /64

    // Timer0 -> PD6(OC0A), PD5(OC0B) : Fast PWM non inversé, prescaler /64
    TCCR2A |= (1 << COM2B1) | (1 << WGM20) |(1 << WGM21); // fast PWM, non inverted
    TCCR2B |= (1 << CS20) | (1 << CS21); // prescaler / 64
}

// Set the LED's color
void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    OCR0B = r;
    OCR0A = g;
    OCR2B = b;
}