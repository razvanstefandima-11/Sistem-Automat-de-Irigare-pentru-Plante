#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file buzzer.h
 * @brief Alarm & Acoustic Feedback Buzzer Driver for ATmega2560 (C.A.M.P.I. Master).
 * 
 * Hardware Pin: PH5 (Pin 8 on Arduino Mega).
 * Used for:
 *  - Dock insertion acoustic validation (100ms chirp upon 5s valid lock).
 *  - Critical fault alarm patterns (Dry-run empty tank, overheating).
 */

/**
 * @brief Initializes the buzzer GPIO pin.
 */
void Buzzer_Init(void);

/**
 * @brief Immediately turns the buzzer ON continuously.
 */
void Buzzer_On(void);

/**
 * @brief Immediately turns the buzzer OFF.
 */
void Buzzer_Off(void);

/**
 * @brief Toggles the buzzer output state.
 */
void Buzzer_Toggle(void);

/**
 * @brief Triggers a non-blocking single acoustic beep.
 * 
 * @param duration_ms Beep duration in milliseconds.
 */
void Buzzer_Beep(uint16_t duration_ms);

/**
 * @brief Enables or disables intermittent repetitive alarm beeping.
 * 
 * @param enable true to start alarm pattern, false to silence.
 */
void Buzzer_SetAlarm(bool enable);

/**
 * @brief Non-blocking state machine update for beeps and alarms.
 * 
 * @param current_millis Current timestamp from Millis().
 */
void Buzzer_Update(uint32_t current_millis);

#endif // BUZZER_H
