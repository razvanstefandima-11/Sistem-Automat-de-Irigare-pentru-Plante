#ifndef FLOAT_SWITCH_H
#define FLOAT_SWITCH_H

#include <stdint.h>
#include <stdbool.h>
#include "drivers/gpio/gpio.h"

/**
 * @file float_switch.h
 * @brief Side-Mount Magnetic Float Switch Driver with Anti-Wave Filtering for ATmega2560.
 * 
 * Monitors Clean Water Tank and Nutrient Concentrate Tank levels.
 * Uses microcontroller internal Pull-Up resistors connected to GND.
 * Implements a 2-second anti-wave software filter to prevent false triggers
 * due to fluid sloshing, ensuring safe Dry-Run pump protection.
 */

#define FLOAT_SWITCH_ANTI_WAVE_MS 2000UL /**< 2-second debouncing filter */

typedef enum {
    FLOAT_TANK_WATER = 0,     /**< Clean Water Reservoir */
    FLOAT_TANK_NUTRIENTS = 1, /**< NPK Concentrate Reservoir */
    FLOAT_TANK_COUNT = 2
} float_tank_id_t;

typedef enum {
    TANK_LEVEL_OK = 0,        /**< Fluid level adequate */
    TANK_LEVEL_EMPTY = 1      /**< Fluid level below minimum threshold (Empty) */
} tank_level_state_t;

/**
 * @brief Initializes the float switches with internal Pull-Up resistors.
 * 
 * @param water_port GPIO port for water float switch.
 * @param water_pin Pin number for water float switch.
 * @param nutr_port GPIO port for nutrients float switch.
 * @param nutr_pin Pin number for nutrients float switch.
 */
void FloatSwitch_Init(gpio_port_t water_port, uint8_t water_pin,
                      gpio_port_t nutr_port, uint8_t nutr_pin);

/**
 * @brief Updates anti-wave software debouncing filters.
 * Must be called periodically from the main loop.
 * 
 * @param current_millis Current system time in milliseconds (from Millis()).
 */
void FloatSwitch_Update(uint32_t current_millis);

/**
 * @brief Returns the debounced state of a specific reservoir.
 * 
 * @param tank FLOAT_TANK_WATER or FLOAT_TANK_NUTRIENTS.
 * @return tank_level_state_t TANK_LEVEL_OK or TANK_LEVEL_EMPTY.
 */
tank_level_state_t FloatSwitch_GetState(float_tank_id_t tank);

/**
 * @brief Checks if a Dry-Run lockout condition is currently active.
 * If either tank is confirmed empty after 2 seconds of anti-wave filtering,
 * returns true to immediately inhibit pump activation.
 * 
 * @return true if pumps must be locked out, false if safe to operate.
 */
bool FloatSwitch_IsDryRunLocked(void);

#endif // FLOAT_SWITCH_H
