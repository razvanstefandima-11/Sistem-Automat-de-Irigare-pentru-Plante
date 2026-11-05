#include "grow_lights.h"
#include <avr/io.h>

/**
 * @file grow_lights.c
 * @brief 16-bit Timer1 direct register PWM implementation for LED fixtures.
 */

// Timer1 TOP value for 1 kHz PWM at F_CPU = 16MHz and Prescaler = 8:
// Frequency = 16,000,000 / (8 * TOP) = 1,000 Hz -> TOP = 2000
#define GROW_LIGHTS_TIMER1_TOP 2000U

static uint8_t current_duty = 0;

void GrowLights_Init(void) {
    // 1. Configure Timer1 for Fast PWM Mode 14 (TOP = ICR1)
    // Non-inverting on OC1A (PB5): COM1A1 = 1, COM1A0 = 0
    TCCR1A = (1 << COM1A1) | (1 << WGM11);

    // WGM13 = 1, WGM12 = 1, Prescaler = 8 (CS11 = 1)
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);

    // 2. Set TOP to 2000 (1000 Hz)
    ICR1 = GROW_LIGHTS_TIMER1_TOP;

    // 3. Initial Duty Cycle = 0 (OFF)
    OCR1A = 0;

    // 4. Set PB5 (OC1A / Arduino Mega Pin 11) as OUTPUT
    DDRB |= (1 << DDB5);

    current_duty = 0;
}

void GrowLights_SetDuty(uint8_t duty_percent) {
    if (duty_percent > 100) duty_percent = 100;
    current_duty = duty_percent;

    // Map 0-100% to 0-GROW_LIGHTS_TIMER1_TOP
    uint32_t val = ((uint32_t)duty_percent * GROW_LIGHTS_TIMER1_TOP) / 100UL;
    OCR1A = (uint16_t)val;
}

void GrowLights_SetStage(grow_stage_t stage) {
    switch (stage) {
        case GROW_STAGE_OFF:
            GrowLights_SetDuty(0);
            break;
        case GROW_STAGE_SEEDLING:
            GrowLights_SetDuty(50); // 50% Redus
            break;
        case GROW_STAGE_VEGETATIVE:
            GrowLights_SetDuty(80); // 80% Intens
            break;
        case GROW_STAGE_FLOWERING:
            GrowLights_SetDuty(100); // 100% Maxim
            break;
    }
}

uint8_t GrowLights_GetDuty(void) {
    return current_duty;
}
