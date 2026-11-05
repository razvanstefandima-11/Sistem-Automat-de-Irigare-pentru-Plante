#include "pollinator.h"
#include "drivers/gpio/gpio.h"
#include "drivers/dht22/dht22.h"

/**
 * @file pollinator.c
 * @brief Haptic vibration motor driver implementation for Micro Tom buzz pollination.
 */

// Hardware Pin Mappings on ATmega2560 (Port A, Pins 1 to 5 / Pins 23 to 27 on Arduino Mega)
static const struct {
    gpio_port_t port;
    uint8_t pin;
} motor_pins[POLLINATOR_NUM_MOTORS] = {
    {GPIO_PORTA, 1}, // Motor 1 (Pin 23)
    {GPIO_PORTA, 2}, // Motor 2 (Pin 24)
    {GPIO_PORTA, 3}, // Motor 3 (Pin 25)
    {GPIO_PORTA, 4}, // Motor 4 (Pin 26)
    {GPIO_PORTA, 5}  // Motor 5 (Pin 27)
};

static uint32_t motor_start_times[POLLINATOR_NUM_MOTORS];
static uint16_t motor_durations[POLLINATOR_NUM_MOTORS];
static bool motor_active[POLLINATOR_NUM_MOTORS];

void Pollinator_Init(void) {
    for (uint8_t i = 0; i < POLLINATOR_NUM_MOTORS; i++) {
        GPIO_Init(motor_pins[i].port, motor_pins[i].pin, GPIO_OUTPUT);
        GPIO_Write(motor_pins[i].port, motor_pins[i].pin, GPIO_LOW);
        motor_active[i] = false;
        motor_start_times[i] = 0;
        motor_durations[i] = 0;
    }
}

bool Pollinator_TriggerSlot(uint8_t slot_idx, float ambient_humidity, uint16_t burst_duration_ms) {
    if (slot_idx >= POLLINATOR_NUM_MOTORS) return false;

    // Environmental conditioning (Section 4.2): optimal RH 50% - 70%
    if (!DHT22_IsHumidityOptimalForPollination(ambient_humidity)) {
        return false;
    }

    if (burst_duration_ms == 0) burst_duration_ms = POLLINATOR_DEFAULT_BURST_MS;

    motor_durations[slot_idx] = burst_duration_ms;
    motor_start_times[slot_idx] = 0; // Set on next update tick
    motor_active[slot_idx] = true;
    GPIO_Write(motor_pins[slot_idx].port, motor_pins[slot_idx].pin, GPIO_HIGH);

    return true;
}

bool Pollinator_TriggerAll(float ambient_humidity, uint16_t burst_duration_ms) {
    if (!DHT22_IsHumidityOptimalForPollination(ambient_humidity)) {
        return false;
    }

    bool started_any = false;
    for (uint8_t i = 0; i < POLLINATOR_NUM_MOTORS; i++) {
        if (Pollinator_TriggerSlot(i, ambient_humidity, burst_duration_ms)) {
            started_any = true;
        }
    }
    return started_any;
}

void Pollinator_Update(uint32_t current_millis) {
    for (uint8_t i = 0; i < POLLINATOR_NUM_MOTORS; i++) {
        if (motor_active[i]) {
            if (motor_start_times[i] == 0) {
                motor_start_times[i] = current_millis;
            } else if ((current_millis - motor_start_times[i]) >= motor_durations[i]) {
                // Burst duration completed: stop motor
                GPIO_Write(motor_pins[i].port, motor_pins[i].pin, GPIO_LOW);
                motor_active[i] = false;
                motor_start_times[i] = 0;
            }
        }
    }
}

void Pollinator_StopAll(void) {
    for (uint8_t i = 0; i < POLLINATOR_NUM_MOTORS; i++) {
        GPIO_Write(motor_pins[i].port, motor_pins[i].pin, GPIO_LOW);
        motor_active[i] = false;
        motor_start_times[i] = 0;
    }
}

bool Pollinator_IsActive(void) {
    for (uint8_t i = 0; i < POLLINATOR_NUM_MOTORS; i++) {
        if (motor_active[i]) return true;
    }
    return false;
}
