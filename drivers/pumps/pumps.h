#ifndef PUMPS_H
#define PUMPS_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file pumps.h
 * @brief Time-Proportional Dosing Pumps Driver for ATmega2560 (C.A.M.P.I. Master).
 * 
 * Controls:
 *  - Pump A: Clean Water (dilution & manifold purge/flush)
 *  - Pump B: Nutrient Concentrate (NPK)
 * 
 * Enforces Dry-Run safety interlock via float_switch driver.
 */

/**
 * @brief Initializes GPIO pins for Pump A and Pump B and ensures pumps are de-energized.
 */
void Pumps_Init(void);

/**
 * @brief Controls Pump A (Clean Water / Flush).
 * Inhibited if Dry-Run condition is active.
 * 
 * @param enable true to turn ON, false to turn OFF.
 * @return true if state applied, false if blocked by safety interlock.
 */
bool Pumps_SetWater(bool enable);

/**
 * @brief Controls Pump B (NPK Nutrient Concentrate).
 * Inhibited if Dry-Run condition is active.
 * 
 * @param enable true to turn ON, false to turn OFF.
 * @return true if state applied, false if blocked by safety interlock.
 */
bool Pumps_SetNutrients(bool enable);

/**
 * @brief Immediately turns off both pumps.
 */
void Pumps_StopAll(void);

/**
 * @brief Checks if Pump A is currently running.
 * 
 * @return true if running, false if stopped.
 */
bool Pumps_IsWaterRunning(void);

/**
 * @brief Checks if Pump B is currently running.
 * 
 * @return true if running, false if stopped.
 */
bool Pumps_IsNutrientsRunning(void);

#endif // PUMPS_H
