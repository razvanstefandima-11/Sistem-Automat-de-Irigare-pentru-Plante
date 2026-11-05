#ifndef DOSING_SYSTEM_H
#define DOSING_SYSTEM_H

#include <stdint.h>
#include <stdbool.h>
#include "drivers/gpio/gpio.h"

/**
 * @file dosing_system.h
 * @brief Precision Time-Proportional Dosing & Flush/Purge Subsystem for C.A.M.P.I.
 * 
 * Implements dynamic in-substrate nutrient dosing directly into Rockwool cubes.
 * - Pump A: Clean Water (dilution & manifold purge/flush)
 * - Pump B: NPK Concentrate
 * - 5x Solenoid Valves: 12V NC manifold distribution
 * 
 * Dosing Sequence:
 * 1. Open Target Slot Valve
 * 2. Run Pump B (Nutrient Concentrate) for calculated duration
 * 3. Run Pump A (Clean Water) for calculated duration (purges manifold & pushes mix into substrate)
 * 4. Close Slot Valve
 */

#define DOSING_NUM_VALVES 5

typedef enum {
    DOSING_IDLE = 0,
    DOSING_OPENING_VALVE,
    DOSING_RUNNING_NUTRIENTS,
    DOSING_RUNNING_WATER_FLUSH,
    DOSING_CLOSING_VALVE
} dosing_state_t;

typedef enum {
    STAGE_SEEDLING = 0,   /**< 90% Water / 10% Nutrients */
    STAGE_VEGETATIVE = 1, /**< 60% Water / 40% Nutrients */
    STAGE_FLOWERING = 2   /**< 30% Water / 70% Nutrients */
} phenological_stage_t;

/**
 * @brief Configures the GPIO pins for Pump A, Pump B, and the 5 solenoid valves.
 */
void Dosing_Init(void);

/**
 * @brief Starts an asynchronous dosing cycle for a target pot slot.
 * 
 * @param slot_idx Index of pot slot (0 to DOSING_NUM_VALVES - 1).
 * @param total_duration_ms Total active pumping duration in milliseconds.
 * @param water_percent Percentage of time allocated to clean water (e.g. 90, 60, 30).
 * @return true if cycle successfully scheduled, false if system busy or locked out by Dry-Run.
 */
bool Dosing_Start(uint8_t slot_idx, uint16_t total_duration_ms, uint8_t water_percent);

/**
 * @brief Starts a dosing cycle configured automatically by phenological stage.
 * 
 * @param slot_idx Index of pot slot (0 to DOSING_NUM_VALVES - 1).
 * @param total_duration_ms Base duration.
 * @param stage STAGE_SEEDLING, STAGE_VEGETATIVE, or STAGE_FLOWERING.
 * @return true if started, false if busy or locked out.
 */
bool Dosing_StartStageDosing(uint8_t slot_idx, uint16_t total_duration_ms, phenological_stage_t stage);

/**
 * @brief Executes a dedicated Clean Water Purge & Flush cycle across all or selected valves.
 * Cleans the common manifold to prevent NPK salt crystallization.
 * 
 * @param slot_idx Target slot to flush.
 * @param flush_duration_ms Flush duration in ms.
 * @return true if started, false otherwise.
 */
bool Dosing_StartPurgeAndFlush(uint8_t slot_idx, uint16_t flush_duration_ms);

/**
 * @brief Non-blocking state machine update. Must be called periodically from the main loop.
 * 
 * @param current_millis Current system time in ms (from Millis()).
 */
void Dosing_Update(uint32_t current_millis);

/**
 * @brief Immediately de-energizes both pumps and closes all solenoid valves.
 */
void Dosing_EmergencyStop(void);

/**
 * @brief Checks if a dosing cycle is currently underway.
 * 
 * @return true if active, false if idle.
 */
bool Dosing_IsBusy(void);

#endif // DOSING_SYSTEM_H
