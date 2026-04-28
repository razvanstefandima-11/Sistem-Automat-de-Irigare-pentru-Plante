#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include "drivers/gpio/gpio.h"

/**
 * @file pwm.h
 * @brief Generic Pulse Width Modulation (PWM) Driver.
 * 
 * Accurately controls the PWM duty cycle for supported pins.
 * Automatically manages underlying Timers (Timer1, Timer2).
 */

/**
 * @brief Initialize PWM on a specific pin.
 * 
 * Configures the associated timer for the given pin. 
 * Note: Pins sharing a timer must use compatible frequencies if underlying hardware dictates it.
 * 
 * @param port GPIO Port (GPIO_PORTB, GPIO_PORTD usually).
 * @param pin Pin number (0-7).
 * @param frequency_hz Desired PWM frequency in Hz.
 */
void PWM_Init(uint8_t port, uint8_t pin, uint32_t frequency_hz);

/**
 * @brief Set the duty cycle for a specific pin.
 * 
 * @param port GPIO Port.
 * @param pin Pin number.
 * @param duty Duty cycle (0-255). 0 = 0%, 255 = 100%.
 */
void PWM_SetDutyCycle(uint8_t port, uint8_t pin, uint8_t duty);

/**
 * @brief Stop PWM on a specific pin.
 * 
 * Disables the PWM output for that pin.
 * 
 * @param port GPIO Port.
 * @param pin Pin number.
 */
void PWM_Stop(uint8_t port, uint8_t pin);

#endif // PWM_H