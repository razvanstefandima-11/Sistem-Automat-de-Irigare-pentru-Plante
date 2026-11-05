#ifndef CLIMATE_CONTROL_H
#define CLIMATE_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file climate_control.h
 * @brief Active Microclimate Thermal Control & Fire Safety Interlock Driver for C.A.M.P.I.
 * 
 * Hardware:
 *  - 12V PC Fan (airflow tunnel)
 *  - 50W 12V Ceramic PTC Heating Element (Relay / MOSFET)
 * 
 * Safety Interlock:
 *  - The fan is ALWAYS started at full capacity (100%) before heating element activation.
 *  - A mandatory 100ms air inertia mechanical delay is enforced before PTC energizes.
 *  - PTC can NEVER be active without the fan running.
 * 
 * Thermal Hysteresis:
 *  - Heating triggers below 22.0°C.
 *  - Heating disengages at 25.0°C.
 *  - Post-heating cooldown keeps the fan running for 15 seconds to safely vent residual heat.
 */

#define CLIMATE_MIN_TARGET_TEMP   22.0f /**< Heater engagement threshold in °C */
#define CLIMATE_MAX_TARGET_TEMP   25.0f /**< Heater disengagement threshold in °C */
#define CLIMATE_AIR_INERTIA_MS    100UL /**< 100ms fan run-up delay */
#define CLIMATE_COOLDOWN_MS       15000UL /**< 15-second residual heat dissipation */

typedef enum {
    CLIMATE_OFF = 0,
    CLIMATE_FAN_STARTUP, /**< Fan running, waiting 100ms mechanical inertia */
    CLIMATE_HEATING,     /**< Fan running + PTC active */
    CLIMATE_COOLDOWN     /**< PTC off, Fan running to vent heat */
} climate_state_t;

/**
 * @brief Initializes GPIO and PWM pins for the PC Fan and PTC heater relay.
 */
void ClimateControl_Init(void);

/**
 * @brief Non-blocking periodic climate control update.
 * 
 * @param current_temp Temperature reading from DHT22 in °C.
 * @param current_millis Current timestamp in ms (from Millis()).
 */
void ClimateControl_Update(float current_temp, uint32_t current_millis);

/**
 * @brief Immediate fail-safe emergency stop. De-energizes PTC instantly.
 */
void ClimateControl_EmergencyStop(void);

/**
 * @brief Checks if heating element is currently energized.
 * 
 * @return true if PTC is ON, false otherwise.
 */
bool ClimateControl_IsHeatingActive(void);

/**
 * @brief Returns current state of climate control.
 * 
 * @return climate_state_t 
 */
climate_state_t ClimateControl_GetState(void);

#endif // CLIMATE_CONTROL_H
