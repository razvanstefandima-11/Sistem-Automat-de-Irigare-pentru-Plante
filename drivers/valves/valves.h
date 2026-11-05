#ifndef VALVES_H
#define VALVES_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file valves.h
 * @brief Manifold Solenoid Valves Driver for ATmega2560 (C.A.M.P.I. Master).
 * 
 * Controls a GPIO array of 5 Normally Closed (NC) 12V DC solenoid valves
 * for independent hydraulic routing to each dock/slot.
 */

#define VALVES_NUM_SLOTS 5

/**
 * @brief Initializes all 5 solenoid valve GPIO pins as outputs and ensures they are closed.
 */
void Valves_Init(void);

/**
 * @brief Opens the solenoid valve for a specific slot.
 * 
 * @param slot_idx Index of slot (0 to VALVES_NUM_SLOTS - 1).
 */
void Valves_Open(uint8_t slot_idx);

/**
 * @brief Closes the solenoid valve for a specific slot.
 * 
 * @param slot_idx Index of slot (0 to VALVES_NUM_SLOTS - 1).
 */
void Valves_Close(uint8_t slot_idx);

/**
 * @brief Closes all 5 solenoid valves simultaneously.
 */
void Valves_CloseAll(void);

/**
 * @brief Queries the operational state of a valve.
 * 
 * @param slot_idx Index of slot (0 to VALVES_NUM_SLOTS - 1).
 * @return true if open (energized), false if closed.
 */
bool Valves_IsOpen(uint8_t slot_idx);

#endif // VALVES_H
