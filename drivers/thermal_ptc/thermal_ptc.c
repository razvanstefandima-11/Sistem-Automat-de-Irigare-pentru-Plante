#include "thermal_ptc.h"
#include "drivers/gpio/gpio.h"
#include "drivers/pwm/pwm.h"

/**
 * @file thermal_ptc.c
 * @brief Fire safety interlock and thermal hysteresis state machine implementation.
 */

// Hardware Pin Mappings on ATmega2560
#define PIN_PTC_RELAY  GPIO_PORTC, 7 /**< 12V 50W PTC Heater Relay (Pin 30) */
#define PIN_FAN_PWM    GPIO_PORTB, 4 /**< 12V PC Fan PWM Control (OC2A / Pin 10) */

#define RELAY_ACTIVE   GPIO_HIGH
#define RELAY_INACTIVE GPIO_LOW

static thermal_state_t state = THERMAL_STATE_OFF;
static uint32_t state_timer = 0;
static bool ptc_hardware_state = false;

static void set_fan_pwm(uint8_t duty_percent) {
    if (duty_percent > 100) duty_percent = 100;
    uint8_t raw_duty = (uint8_t)(((uint16_t)duty_percent * 255) / 100);
    PWM_SetDutyCycle(GPIO_PORTB, 4, raw_duty);
}

static void set_ptc_relay(bool enable) {
    // HARDWARE SAFETY INTERLOCK CHECK:
    // Under NO circumstance can PTC be energized unless fan is running at full capacity (100%)
    if (enable && (state != THERMAL_STATE_FAN_RAMP_UP && state != THERMAL_STATE_HEATING)) {
        GPIO_Write(PIN_PTC_RELAY, RELAY_INACTIVE);
        ptc_hardware_state = false;
        return;
    }

    ptc_hardware_state = enable;
    GPIO_Write(PIN_PTC_RELAY, enable ? RELAY_ACTIVE : RELAY_INACTIVE);
}

void ThermalPTC_Init(void) {
    // Configure PTC relay pin as output and ensure it is powered off
    GPIO_Init(PIN_PTC_RELAY, GPIO_OUTPUT);
    GPIO_Write(PIN_PTC_RELAY, RELAY_INACTIVE);
    ptc_hardware_state = false;

    // Configure PC Fan PWM output (25kHz standard PC fan frequency)
    PWM_Init(GPIO_PORTB, 4, 25000);
    set_fan_pwm(0);

    state = THERMAL_STATE_OFF;
    state_timer = 0;
}

void ThermalPTC_Update(float current_temp, uint32_t current_millis) {
    // Sensor failure sanity check: if temperature is wildly out of bounds, trigger fail-safe
    if (current_temp < -20.0f || current_temp > 70.0f) {
        ThermalPTC_EmergencyStop();
        return;
    }

    switch (state) {
        case THERMAL_STATE_OFF:
            // Temperature dropped below lower hysteresis band (22.0°C): start fan first!
            if (current_temp < THERMAL_TARGET_MIN_TEMP) {
                set_fan_pwm(100); // 100% PWM
                state = THERMAL_STATE_FAN_RAMP_UP;
                state_timer = current_millis;
            }
            break;

        case THERMAL_STATE_FAN_RAMP_UP:
            // Enforce mandatory 100ms air inertia delay before engaging 50W PTC
            if ((current_millis - state_timer) >= THERMAL_AIR_INERTIA_DELAY_MS) {
                set_ptc_relay(true);
                state = THERMAL_STATE_HEATING;
            }
            break;

        case THERMAL_STATE_HEATING:
            // Temperature reached upper hysteresis band (25.0°C): de-energize PTC, start cooldown
            if (current_temp >= THERMAL_TARGET_MAX_TEMP) {
                set_ptc_relay(false); // PTC turns off immediately
                state = THERMAL_STATE_COOLDOWN;
                state_timer = current_millis;
            }
            break;

        case THERMAL_STATE_COOLDOWN:
            // Fan continues running at 100% to dissipate residual heat from ceramic block
            if ((current_millis - state_timer) >= THERMAL_FAN_COOLDOWN_MS) {
                // Cooldown complete: shut down fan if temperature is still safe
                if (current_temp >= THERMAL_TARGET_MIN_TEMP) {
                    set_fan_pwm(0);
                    state = THERMAL_STATE_OFF;
                } else {
                    // Temperature dropped again during cooldown: re-engage heating
                    set_ptc_relay(true);
                    state = THERMAL_STATE_HEATING;
                }
            }
            break;
    }
}

void ThermalPTC_EmergencyStop(void) {
    set_ptc_relay(false);
    set_fan_pwm(100); // Keep fan running to vent any residual heat in an emergency
    state = THERMAL_STATE_OFF;
}

bool ThermalPTC_IsPTCActive(void) {
    return ptc_hardware_state;
}

thermal_state_t ThermalPTC_GetState(void) {
    return state;
}
