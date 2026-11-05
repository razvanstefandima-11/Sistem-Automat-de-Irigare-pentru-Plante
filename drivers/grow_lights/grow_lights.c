#include "grow_lights.h"
#include "drivers/gpio/gpio.h"
#include "drivers/pwm/pwm.h"

/**
 * @file grow_lights.c
 * @brief Full-Spectrum Grow Lights PWM driver implementation.
 */

// Hardware PWM pin on ATmega2560: Timer1 OC1A (PB5 / Pin 11)
#define GROW_LIGHTS_PORT GPIO_PORTB
#define GROW_LIGHTS_PIN  5

static uint8_t current_duty_pct = 0;

void GrowLights_Init(void) {
    // Initialize Timer1 Fast PWM at 1000 Hz
    PWM_Init(GROW_LIGHTS_PORT, GROW_LIGHTS_PIN, GROW_LIGHTS_PWM_FREQ_HZ);
    GrowLights_TurnOff();
}

void GrowLights_SetDuty(uint8_t duty_percent) {
    if (duty_percent > 100) duty_percent = 100;
    current_duty_pct = duty_percent;

    // Convert 0-100% to 0-255 duty cycle for PWM driver
    uint8_t raw_duty = (uint8_t)(((uint16_t)duty_percent * 255UL) / 100UL);
    PWM_SetDutyCycle(GROW_LIGHTS_PORT, GROW_LIGHTS_PIN, raw_duty);
}

void GrowLights_SetStage(phenological_stage_t stage) {
    switch (stage) {
        case STAGE_SEEDLING:
            GrowLights_SetDuty(GROW_LIGHTS_DUTY_SEEDLING);
            break;
        case STAGE_VEGETATIVE:
            GrowLights_SetDuty(GROW_LIGHTS_DUTY_VEGETATIVE);
            break;
        case STAGE_FLOWERING:
            GrowLights_SetDuty(GROW_LIGHTS_DUTY_FLOWERING);
            break;
    }
}

void GrowLights_TurnOff(void) {
    GrowLights_SetDuty(GROW_LIGHTS_DUTY_OFF);
}

uint8_t GrowLights_GetDuty(void) {
    return current_duty_pct;
}
