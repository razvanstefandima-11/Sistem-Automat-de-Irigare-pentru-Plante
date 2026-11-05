#ifndef SERVO_ROOF_H
#define SERVO_ROOF_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file servo_roof.h
 * @brief 16-bit Timer3 Hardware PWM Driver for MG996R Shading Roof Servos.
 * 
 * Generates high-precision 50Hz (20ms period) PWM signals using Timer3 on ATmega2560.
 * Controls two high-torque MG996R servos for deployable thermal shading roof:
 *  - Servo 1: PE3 (OC3A / Pin 5)
 *  - Servo 2: PE4 (OC3B / Pin 2)
 * 
 * Safety Rule:
 *  - Deploys shading automatically when local temperature exceeds 32.0°C to prevent pollen sterilization.
 */

#define SERVO_ROOF_COUNT 2
#define SERVO_SHADING_TEMP_THRESHOLD 32.0f /**< 32°C pollen protection trigger */

// Servo Pulse Calibration (Timer3 with Prescaler 8 -> 0.5us per tick)
#define SERVO_TICK_MIN 2000U /**< 1000us (0 degrees / Retracted) */
#define SERVO_TICK_MAX 4000U /**< 2000us (180 degrees / Deployed) */

/**
 * @brief Configures Timer3 in 50Hz Fast PWM Mode 14 for dual servo control.
 */
void ServoRoof_Init(void);

/**
 * @brief Sets servo position in degrees (0 to 180).
 * 
 * @param servo_idx 0 (Servo 1) or 1 (Servo 2).
 * @param angle_deg Angle in degrees (0 to 180).
 */
void ServoRoof_SetAngle(uint8_t servo_idx, uint8_t angle_deg);

/**
 * @brief Deploys (closes) or retracts (opens) the thermal shading roof.
 * 
 * @param deploy true to close roof (shade active), false to open roof.
 */
void ServoRoof_SetShading(bool deploy);

/**
 * @brief Evaluates ambient temperature and automatically triggers shading if > 32°C.
 * 
 * @param temperature Ambient temperature in °C.
 */
void ServoRoof_CheckThermalProtection(float temperature);

/**
 * @brief Checks if the shading roof is currently deployed.
 * 
 * @return true if deployed (shaded), false if open.
 */
bool ServoRoof_IsShaded(void);

#endif // SERVO_ROOF_H
