#include "servo_roof.h"
#include <avr/io.h>

/**
 * @file servo_roof.c
 * @brief Timer3 50Hz Fast PWM implementation for MG996R dual servos.
 */

// 16MHz / (Prescaler 8) = 2,000,000 Hz (0.5us per tick)
// 20ms period (50Hz) = 40,000 ticks
#define TIMER3_TOP_50HZ 40000U

static bool shading_deployed = false;

void ServoRoof_Init(void) {
    // 1. Configure Timer3 for Fast PWM Mode 14 (TOP = ICR3)
    // Non-inverting on OC3A (PE3) and OC3B (PE4)
    TCCR3A = (1 << COM3A1) | (1 << COM3B1) | (1 << WGM31);

    // WGM33 = 1, WGM32 = 1, Prescaler = 8 (CS31 = 1)
    TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS31);

    // 2. Set TOP to 40,000 ticks for exact 20.0ms period (50 Hz)
    ICR3 = TIMER3_TOP_50HZ;

    // 3. Set PE3 (Pin 5) and PE4 (Pin 2) as OUTPUT
    DDRE |= (1 << DDE3) | (1 << DDE4);

    // 4. Start in retracted position (0 degrees / 1.0ms pulse)
    ServoRoof_SetShading(false);
}

void ServoRoof_SetAngle(uint8_t servo_idx, uint8_t angle_deg) {
    if (angle_deg > 180) angle_deg = 180;

    // Linear mapping: 0 deg -> SERVO_TICK_MIN (2000 ticks), 180 deg -> SERVO_TICK_MAX (4000 ticks)
    uint32_t ticks = SERVO_TICK_MIN + (((uint32_t)angle_deg * (SERVO_TICK_MAX - SERVO_TICK_MIN)) / 180UL);

    if (servo_idx == 0) {
        OCR3A = (uint16_t)ticks; // Servo 1 (PE3)
    } else if (servo_idx == 1) {
        OCR3B = (uint16_t)ticks; // Servo 2 (PE4)
    }
}

void ServoRoof_SetShading(bool deploy) {
    shading_deployed = deploy;
    if (deploy) {
        // Deploy shading panels (180 degrees)
        ServoRoof_SetAngle(0, 180);
        ServoRoof_SetAngle(1, 180);
    } else {
        // Retract panels (0 degrees / sunlight open)
        ServoRoof_SetAngle(0, 0);
        ServoRoof_SetAngle(1, 0);
    }
}

void ServoRoof_CheckThermalProtection(float temperature) {
    // PDF Section 4.2: Automatically deploy shade if temperature > 32°C
    if (temperature > SERVO_SHADING_TEMP_THRESHOLD) {
        if (!shading_deployed) {
            ServoRoof_SetShading(true);
        }
    } else if (temperature < (SERVO_SHADING_TEMP_THRESHOLD - 2.0f)) {
        // 2°C hysteresis to prevent motor jitter around 32°C
        if (shading_deployed) {
            ServoRoof_SetShading(false);
        }
    }
}

bool ServoRoof_IsShaded(void) {
    return shading_deployed;
}
