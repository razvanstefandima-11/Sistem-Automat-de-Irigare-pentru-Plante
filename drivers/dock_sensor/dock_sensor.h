#ifndef DOCK_SENSOR_H
#define DOCK_SENSOR_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file dock_sensor.h
 * @brief Plug & Play Magnetic Dock Detection Driver for C.A.M.P.I.
 * 
 * Monitors 5 modular pot docks using SS49E / A3144 Hall-effect sensors.
 * Implements a mandatory 5-second insertion validation timer before confirming
 * that a pot dock has been seated and locked mechanically.
 */

#define DOCK_NUM_SLOTS 5
#define DOCK_VALIDATION_DELAY_MS 5000UL /**< 5-second debounce/validation delay */

typedef enum {
    DOCK_STATUS_EMPTY = 0,      /**< No pot detected in dock */
    DOCK_STATUS_INSERTING = 1,  /**< Magnet detected, 5-second validation timer running */
    DOCK_STATUS_CONFIRMED = 2   /**< Pot confirmed seated and validated */
} dock_status_t;

/**
 * @brief Initializes the Hall sensor GPIO inputs with internal pull-ups.
 */
void DockSensor_Init(void);

/**
 * @brief Non-blocking periodic evaluation of dock sensors and 5-second validation timers.
 * 
 * @param current_millis Current timestamp in ms (from Millis()).
 */
void DockSensor_Update(uint32_t current_millis);

/**
 * @brief Gets the status of a specific dock slot.
 * 
 * @param slot_idx Index of dock slot (0 to 4).
 * @return dock_status_t 
 */
dock_status_t DockSensor_GetStatus(uint8_t slot_idx);

/**
 * @brief Checks if a new dock has just completed its 5-second validation.
 * Resets the flag upon reading (useful for triggering acoustic buzzer beep).
 * 
 * @param slot_idx Index of dock slot (0 to 4).
 * @return true if validated on this cycle, false otherwise.
 */
bool DockSensor_HasJustValidated(uint8_t slot_idx);

/**
 * @brief Checks if a dock is confirmed populated and ready for irrigation.
 * 
 * @param slot_idx Index of dock slot (0 to 4).
 * @return true if confirmed seated, false otherwise.
 */
bool DockSensor_IsDockReady(uint8_t slot_idx);

#endif // DOCK_SENSOR_H
