#ifndef TIMER2_H
#define TIMER2_H

#include <stdint.h>
#include <avr/io.h>

/**
 * @brief Initialize Timer2 for Fast PWM mode.
 * 
 * Configures Timer2 for Fast PWM (Mode 3 or Mode 7).
 * For Mode 3: TOP = 0xFF (Fixed frequency).
 * For Mode 7: TOP = OCR2A (Variable frequency, but OC2A is toggle only or disabled).
 * 
 * We will use Mode 3 (Fast PWM, TOP=0xFF) for standard 8-bit PWM.
 * Frequency = F_CPU / (Prescaler * 256)
 * 
 * @param prescaler Prescaler value (1, 8, 32, 64, 128, 256, 1024)
 */
void Timer2_FastPWM_Init(uint16_t prescaler);

/**
 * @brief Set the duty cycle for Channel A (Pin D11 / PB3).
 * 
 * @param duty val (0 to 255)
 */
void Timer2_SetDutyCycleA(uint8_t duty);

/**
 * @brief Set the duty cycle for Channel B (Pin D3 / PD3).
 * 
 * @param duty val (0 to 255)
 */
void Timer2_SetDutyCycleB(uint8_t duty);

/**
 * @brief Stop Timer2.
 */
void Timer2_Stop(void);

#endif // TIMER2_H