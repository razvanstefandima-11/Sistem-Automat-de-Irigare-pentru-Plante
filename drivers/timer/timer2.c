#include "timer2.h"

/**
 * @file timer2.c
 * @brief 8-bit Timer2 PWM implementation adapted for ATmega2560 (C.A.M.P.I. Master).
 */

void Timer2_FastPWM_Init(uint16_t prescaler) {
    // 1. Set Fast PWM Mode 3 (WGM22:0 = 011)
    // TOP is 0xFF (255), Update of OCR2x at BOTTOM, TOV2 Flag Set on MAX
    
    // TCCR2A: COM2A1, COM2B1 (Non-inverting), WGM21 (1), WGM20 (1)
    TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    
    // TCCR2B: WGM22 (0), CS2x (Prescaler)
    uint8_t cs_bits = 0;
    switch (prescaler) {
        case 1:    cs_bits = (1 << CS20); break;
        case 8:    cs_bits = (1 << CS21); break;
        case 32:   cs_bits = (1 << CS21) | (1 << CS20); break;
        case 64:   cs_bits = (1 << CS22); break;
        case 128:  cs_bits = (1 << CS22) | (1 << CS20); break;
        case 256:  cs_bits = (1 << CS22) | (1 << CS21); break;
        case 1024: cs_bits = (1 << CS22) | (1 << CS21) | (1 << CS20); break;
        default:   cs_bits = (1 << CS22) | (1 << CS21) | (1 << CS20); break;
    }
    
    TCCR2B = cs_bits;

    // Initialize Duty Cycles to 0
    OCR2A = 0;
    OCR2B = 0;

    // Configure Output Pins:
#if defined(__AVR_ATmega2560__)
    // On ATmega2560: OC2A is PB4 (Arduino Mega Pin 10), OC2B is PH6 (Arduino Mega Pin 9)
    DDRB |= (1 << DDB4);
    DDRH |= (1 << DDH6);
#else
    // ATmega328P / Host unit tests: OC2A is PB3 (Pin 11), OC2B is PD3 (Pin 3)
    DDRB |= (1 << DDB3);
    DDRD |= (1 << DDD3);
#endif
}

void Timer2_SetDutyCycleA(uint8_t duty) {
    OCR2A = duty;
}

void Timer2_SetDutyCycleB(uint8_t duty) {
    OCR2B = duty;
}

void Timer2_Stop(void) {
    TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
}
