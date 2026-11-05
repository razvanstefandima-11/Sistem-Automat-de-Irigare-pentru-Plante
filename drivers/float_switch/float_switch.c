#include "float_switch.h"

/**
 * @file float_switch.c
 * @brief Anti-wave debounced float switch implementation for C.A.M.P.I.
 */

typedef struct {
    gpio_port_t port;
    uint8_t pin;
    gpio_state_t raw_state;
    tank_level_state_t confirmed_state;
    uint32_t state_change_timestamp;
    bool in_transition;
} float_sensor_channel_t;

static float_sensor_channel_t tanks[FLOAT_TANK_COUNT];

void FloatSwitch_Init(gpio_port_t water_port, uint8_t water_pin,
                      gpio_port_t nutr_port, uint8_t nutr_pin) {
    tanks[FLOAT_TANK_WATER].port = water_port;
    tanks[FLOAT_TANK_WATER].pin  = water_pin;
    tanks[FLOAT_TANK_WATER].raw_state = GPIO_HIGH;
    tanks[FLOAT_TANK_WATER].confirmed_state = TANK_LEVEL_OK;
    tanks[FLOAT_TANK_WATER].state_change_timestamp = 0;
    tanks[FLOAT_TANK_WATER].in_transition = false;

    tanks[FLOAT_TANK_NUTRIENTS].port = nutr_port;
    tanks[FLOAT_TANK_NUTRIENTS].pin  = nutr_pin;
    tanks[FLOAT_TANK_NUTRIENTS].raw_state = GPIO_HIGH;
    tanks[FLOAT_TANK_NUTRIENTS].confirmed_state = TANK_LEVEL_OK;
    tanks[FLOAT_TANK_NUTRIENTS].state_change_timestamp = 0;
    tanks[FLOAT_TANK_NUTRIENTS].in_transition = false;

    // Configure pins as INPUT with internal PULL-UP enabled
    GPIO_Init(water_port, water_pin, GPIO_INPUT_PULLUP);
    GPIO_Init(nutr_port, nutr_pin, GPIO_INPUT_PULLUP);
}

void FloatSwitch_Update(uint32_t current_millis) {
    for (uint8_t i = 0; i < FLOAT_TANK_COUNT; i++) {
        gpio_state_t reading = GPIO_Read(tanks[i].port, tanks[i].pin);

        // Switch to GND: when float is down (empty), contact closes to GND -> GPIO_LOW
        // When float is floating (full), contact opens -> GPIO_HIGH via internal pull-up
        tank_level_state_t current_state = (reading == GPIO_LOW) ? TANK_LEVEL_EMPTY : TANK_LEVEL_OK;

        if (current_state != tanks[i].confirmed_state) {
            if (!tanks[i].in_transition) {
                tanks[i].in_transition = true;
                tanks[i].state_change_timestamp = current_millis;
            } else {
                // Check if candidate state has remained steady for the 2-second anti-wave window
                if ((current_millis - tanks[i].state_change_timestamp) >= FLOAT_SWITCH_ANTI_WAVE_MS) {
                    tanks[i].confirmed_state = current_state;
                    tanks[i].in_transition = false;
                }
            }
        } else {
            // Noise / transient bounce returned to confirmed state: reset transition timer
            tanks[i].in_transition = false;
        }
    }
}

tank_level_state_t FloatSwitch_GetState(float_tank_id_t tank) {
    if (tank >= FLOAT_TANK_COUNT) return TANK_LEVEL_EMPTY;
    return tanks[tank].confirmed_state;
}

bool FloatSwitch_IsDryRunLocked(void) {
    // Lockout pumps if either clean water or nutrients reservoir is confirmed empty
    return (tanks[FLOAT_TANK_WATER].confirmed_state == TANK_LEVEL_EMPTY ||
            tanks[FLOAT_TANK_NUTRIENTS].confirmed_state == TANK_LEVEL_EMPTY);
}
