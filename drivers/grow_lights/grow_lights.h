#ifndef GROW_LIGHTS_H
#define GROW_LIGHTS_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file grow_lights.h
 * @brief Full-Spectrum Grow Lights Hardware PWM Driver for ATmega2560.
 * 
 * Generates flicker-free 1 kHz PWM via 16-bit Timer1 (OC1A / PB5 / Pin 11)
 * directly controlling power MOSFETs (IRLZ44N) for 12V LED fixtures.
 * 
 * Phenological Stage Profiles:
 *  - Seedling (Răsad): 50% PWM (Redus)
 *  - Vegetative Growth: 80% PWM (Intens)
 *  - Flowering / Fruit: 100% PWM (Maxim)
 *  - Inactive / Night: 0% PWM (Off)
 */

typedef enum {
    GROW_STAGE_OFF = 0,
    GROW_STAGE_SEEDLING = 1,   /**< 50% Intensity */
    GROW_STAGE_VEGETATIVE = 2, /**< 80% Intensity */
    GROW_STAGE_FLOWERING = 3   /**< 100% Intensity */
} grow_stage_t;

/**
 * @brief Configures Timer1 in Fast PWM Mode 14 (1 kHz) on pin PB5.
 */
void GrowLights_Init(void);

/**
 * @brief Sets raw duty cycle percentage (0 - 100%).
 * 
 * @param duty_percent Percentage from 0 to 100.
 */
void GrowLights_SetDuty(uint8_t duty_percent);

/**
 * @brief Sets illumination profile by phenological stage.
 * 
 * @param stage GROW_STAGE_SEEDLING, GROW_STAGE_VEGETATIVE, GROW_STAGE_FLOWERING, or GROW_STAGE_OFF.
 */
void GrowLights_SetStage(grow_stage_t stage);

/**
 * @brief Returns current duty cycle percentage.
 * 
 * @return uint8_t 
 */
uint8_t GrowLights_GetDuty(void);

#endif // GROW_LIGHTS_H
