#include "climate_control.h"
#include "drivers/gpio/gpio.h"

/**
 * @file climate_control.c
 * @brief Fire safety interlock and climate control implementation.
 */

// Hardware Pin Mappings on ATmega2560
#define PIN_PTC_RELAY  GPIO_PORTC, 7 /**< 12V 50W PTC Heater Relay (Pin 30) */
#define PIN_FAN_SWITCH GPIO_PORTB, 4 /**< 12V PC Fan Control (Pin 10) */

#define RELAY_ACTIVE   GPIO_HIGH
#define RELAY_INACTIVE GPIO_LOW

static climate_state_t state = CLIMATE_OFF;
static uint32_t state_timer = 0;
static bool ptc_active = false;

static void set_fan(bool enable) {
    GPIO_Write(PIN_FAN_SWITCH, enable ? GPIO_HIGH : GPIO_LOW);
}

static void set_ptc(bool enable) {
    // CRITICAL SAFETY INTERLOCK:
    // Never allow PTC to turn on if state is not explicitly in FAN_STARTUP or HEATING
    if (enable && (state != CLIMATE_FAN_STARTUP && state != CLIMATE_HEATING)) {
        GPIO_Write(PIN_PTC_RELAY, RELAY_INACTIVE);
        ptc_active = false;
        return;
    }
    ptc_active = enable;
    GPIO_Write(PIN_PTC_RELAY, enable ? RELAY_ACTIVE : RELAY_INACTIVE);
}

void ClimateControl_Init(void) {
    GPIO_Init(PIN_PTC_RELAY, GPIO_OUTPUT);
    GPIO_Init(PIN_FAN_SWITCH, GPIO_OUTPUT);

    GPIO_Write(PIN_PTC_RELAY, RELAY_INACTIVE);
    GPIO_Write(PIN_FAN_SWITCH, GPIO_LOW);

    ptc_active = false;
    state = CLIMATE_OFF;
    state_timer = 0;
}

void ClimateControl_Update(float current_temp, uint32_t current_millis) {
    // Sensor failure sanity check (Fail-Safe)
    if (current_temp < -20.0f || current_temp > 70.0f) {
        ClimateControl_EmergencyStop();
        return;
    }

    switch (state) {
        case CLIMATE_OFF:
            // Temperature dropped below 22.0°C: start fan first!
            if (current_temp < CLIMATE_MIN_TARGET_TEMP) {
                set_fan(true);
                state = CLIMATE_FAN_STARTUP;
                state_timer = current_millis;
            }
            break;

        case CLIMATE_FAN_STARTUP:
            // Wait for 100ms air inertia delay before energizing PTC
            if ((current_millis - state_timer) >= CLIMATE_AIR_INERTIA_MS) {
                set_ptc(true);
                state = CLIMATE_HEATING;
            }
            break;

        case CLIMATE_HEATING:
            // Temperature reached 25.0°C: turn off heater, keep fan for cooldown
            if (current_temp >= CLIMATE_MAX_TARGET_TEMP) {
                set_ptc(false);
                state = CLIMATE_COOLDOWN;
                state_timer = current_millis;
            }
            break;

        case CLIMATE_COOLDOWN:
            // Fan continues running for 15 seconds to safely dissipate heat
            if ((current_millis - state_timer) >= CLIMATE_COOLDOWN_MS) {
                if (current_temp >= CLIMATE_MIN_TARGET_TEMP) {
                    set_fan(false);
                    state = CLIMATE_OFF;
                } else {
                    // Temperature dropped again: resume heating
                    set_ptc(true);
                    state = CLIMATE_HEATING;
                }
            }
            break;
    }
}

void ClimateControl_EmergencyStop(void) {
    set_ptc(false);
    set_fan(true); // Vent remaining heat
    state = CLIMATE_OFF;
}

bool ClimateControl_IsHeatingActive(void) {
    return ptc_active;
}

climate_state_t ClimateControl_GetState(void) {
    return state;
}
