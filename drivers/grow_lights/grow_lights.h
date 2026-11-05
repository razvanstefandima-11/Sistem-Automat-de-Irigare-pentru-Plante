#ifndef GROW_LIGHTS_H
#define GROW_LIGHTS_H

#include <stdint.h>
#include <stdbool.h>
#include "drivers/dosing_system/dosing_system.h" // For phenological_stage_t

/**
 * @file grow_lights.h
 * @brief Full-Spectrum Grow Lights PWM Driver for ATmega2560 (C.A.M.P.I. Master).
 * 
 * Controls 12V Full-Spectrum LED bars via power MOSFET (IRLZ44N) using hardware PWM.
 * 
 * Stage Profiles (Section 6 of C.A.M.P.I. Specifications):
 *  - 1. Răsad (Seedling):            50% PWM (Redus)
 *  - 2. Creștere Vegetativă:         80% PWM (Intens)
 *  - 3. Înflorire / Fruct:          100% PWM (Maxim)
 *  - Night / Inactive:                0% PWM (Off)
 */

#define GROW_LIGHTS_PWM_FREQ_HZ 1000UL /**< 1 kHz flicker-free LED PWM */

#define GROW_LIGHTS_DUTY_OFF         0
#define GROW_LIGHTS_DUTY_SEEDLING   50
#define GROW_LIGHTS_DUTY_VEGETATIVE 80
#define GROW_LIGHTS_DUTY_FLOWERING 100

/**
 * @brief Initializes the hardware PWM peripheral for Grow Lights.
 */
void GrowLights_Init(void);

/**
 * @brief Sets the raw PWM duty cycle percentage (0 - 100%).
 * 
 * @param duty_percent Percentage from 0% (OFF) to 100% (Full intensity).
 */
void GrowLights_SetDuty(uint8_t duty_percent);

/**
 * @brief Automatically sets the recommended photoperiod intensity for a given phenological stage.
 * 
 * @param stage STAGE_SEEDLING (50%), STAGE_VEGETATIVE (80%), or STAGE_FLOWERING (100%).
 */
void GrowLights_SetStage(phenological_stage_t stage);

/**
 * @brief Turns off the grow lights.
 */
void GrowLights_TurnOff(void);

/**
 * @brief Gets the current duty cycle percentage.
 * 
 * @return uint8_t Current duty cycle (0 - 100).
 */
uint8_t GrowLights_GetDuty(void);

#endif // GROW_LIGHTS_H
