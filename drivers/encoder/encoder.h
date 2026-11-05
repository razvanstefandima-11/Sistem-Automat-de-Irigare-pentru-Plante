#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file encoder.h
 * @brief Quadrature Rotary Encoder & Pushbutton Driver for ATmega2560 (C.A.M.P.I. HMI).
 * 
 * Uses hardware External Interrupts for responsive menu navigation in the local OLED FSM.
 * 
 * Hardware Mapping on ATmega2560:
 *  - Channel A (CLK): PE4 (INT4 / Pin 2) or PD0 (INT0)
 *  - Channel B (DT):  PH3 (Pin 6)
 *  - Push Button (SW): PH4 (Pin 7)
 */

/**
 * @brief Configures external interrupts, GPIO inputs with internal pull-ups, and state tracking.
 */
void Encoder_Init(void);

/**
 * @brief Returns and resets the relative rotational step delta (+1 clockwise, -1 counter-clockwise).
 * 
 * @return int16_t Net delta since last call.
 */
int16_t Encoder_GetDelta(void);

/**
 * @brief Returns the absolute rotational position counter.
 * 
 * @return int16_t Position count.
 */
int16_t Encoder_GetPosition(void);

/**
 * @brief Sets the absolute position counter.
 * 
 * @param pos Position to set.
 */
void Encoder_SetPosition(int16_t pos);

/**
 * @brief Checks if the encoder pushbutton is currently held down.
 * 
 * @return true if pressed, false otherwise.
 */
bool Encoder_IsButtonPressed(void);

/**
 * @brief Checks if a debounced button press & release click event occurred.
 * Resets the click flag upon reading.
 * 
 * @return true if button was clicked, false otherwise.
 */
bool Encoder_HasBeenClicked(void);

/**
 * @brief Non-blocking button debouncing update (call periodically in main loop).
 * 
 * @param current_millis Current timestamp from Millis().
 */
void Encoder_Update(uint32_t current_millis);

#endif // ENCODER_H
