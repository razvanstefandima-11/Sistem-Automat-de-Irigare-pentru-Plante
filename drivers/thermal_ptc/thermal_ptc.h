#ifndef THERMAL_PTC_H
#define THERMAL_PTC_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file thermal_ptc.h
 * @brief Active Microclimate Thermal Control & Fire Safety Interlock for C.A.M.P.I.
 * 
 * Hardware:
 *  - 12V PC Fan (PWM airflow control)
 *  - 50W 12V Ceramic PTC Heating Element (Optocoupled Relay / MOSFET)
 * 
 * Safety Interlock:
 *  - Fan is driven to 100% PWM BEFORE heater activation.
 *  - A mandatory 100ms mechanical air inertia delay is enforced before PTC energizes.
 *  - PTC can NEVER be active without fan confirmed at 100%.
 * 
 * Temperature Control:
 *  - Hysteresis band: 22.0°C (Turn ON) to 25.0°C (Turn OFF).
 *  - Post-heating Cooldown: Fan runs for 15s after PTC de-energizes to dissipate residual heat.
 */

#define THERMAL_TARGET_MIN_TEMP   22.0f /**< Heater engagement threshold in °C */
#define THERMAL_TARGET_MAX_TEMP   25.0f /**< Heater disengagement threshold in °C */
#define THERMAL_SHADING_TEMP      32.0f /**< Maximum threshold triggering shade panels in °C */

#define THERMAL_AIR_INERTIA_DELAY_MS 100UL   /**< 100ms fan run-up before PTC turns on */
#define THERMAL_FAN_COOLDOWN_MS      15000UL /**< 15-second residual heat dissipation run */

typedef enum {
    THERMAL_STATE_OFF = 0,
    THERMAL_STATE_FAN_RAMP_UP,  /**< Fan running 100%, waiting 100ms air inertia */
    THERMAL_STATE_HEATING,      /**< Fan 100% + PTC Active */
    THERMAL_STATE_COOLDOWN      /**< PTC OFF, Fan continuing to dissipate heat */
} thermal_state_t;

/**
 * @brief Initializes GPIO and PWM peripherals for the PC Fan and PTC heater.
 */
void ThermalPTC_Init(void);

/**
 * @brief Periodically evaluates ambient temperature and executes the thermal state machine.
 * 
 * @param current_temp Current temperature reading from DHT22 in °C.
 * @param current_millis Current system time in ms (from Millis()).
 */
void ThermalPTC_Update(float current_temp, uint32_t current_millis);

/**
 * @brief Immediate fail-safe emergency stop. De-energizes PTC instantly.
 */
void ThermalPTC_EmergencyStop(void);

/**
 * @brief Checks if heating element is currently energized.
 * 
 * @return true if PTC is ON, false otherwise.
 */
bool ThermalPTC_IsPTCActive(void);

/**
 * @brief Returns current state of the thermal controller.
 * 
 * @return thermal_state_t 
 */
thermal_state_t ThermalPTC_GetState(void);

#endif // THERMAL_PTC_H
