#include "timer1.h"

void Timer1_FastPWM_Init(uint16_t prescaler, uint16_t top) {
    // 1. Set Fast PWM Mode 14 (WGM13:0 = 1110)
    // TOP is ICR1, Update of OCR1x at BOTTOM, TOV1 Flag Set on TOP
    
    // TCCR1A: COM1A1, COM1B1 (Non-inverting), WGM11 (1)
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
    
    // TCCR1B: WGM13 (1), WGM12 (1), CS1x (Prescaler)
    TCCR1B = (1 << WGM13) | (1 << WGM12);

    // Set TOP value
    ICR1 = top;

    // Set Prescaler
    // 001 = /1, 010 = /8, 011 = /64, 100 = /256, 101 = /1024
    switch (prescaler) {
        case 1: TCCR1B |= (1 << CS10); break;
        case 8: TCCR1B |= (1 << CS11); break;
        case 64: TCCR1B |= (1 << CS11) | (1 << CS10); break;
        case 256: TCCR1B |= (1 << CS12); break;
        case 1024: TCCR1B |= (1 << CS12) | (1 << CS10); break;
        default: TCCR1B |= (1 << CS10); // Default to no prescaling
    }

    // Initialize Duty Cycles to 0
    OCR1A = 0;
    OCR1B = 0;

    // Enable Timer1 Outputs on PB1 (OC1A/D9) and PB2 (OC1B/D10)
    DDRB |= (1 << DDB1) | (1 << DDB2);
}

void Timer1_SetDutyCycleA(uint16_t duty) {
    if (duty > ICR1) duty = ICR1; // Clamp to TOP
    OCR1A = duty;
}

void Timer1_SetDutyCycleB(uint16_t duty) {
    if (duty > ICR1) duty = ICR1; // Clamp to TOP
    OCR1B = duty;
}

void Timer1_Stop(void) {
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}