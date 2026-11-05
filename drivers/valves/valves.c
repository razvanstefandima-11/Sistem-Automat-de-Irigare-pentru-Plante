#include "valves.h"
#include "drivers/gpio/gpio.h"

/**
 * @file valves.c
 * @brief Solenoid valve control implementation.
 */

// Hardware Pin Mappings on ATmega2560 (Port C)
static const struct {
    gpio_port_t port;
    uint8_t pin;
} valve_hardware_pins[VALVES_NUM_SLOTS] = {
    {GPIO_PORTC, 2}, /**< Solenoid Valve 1 (Pin 35) */
    {GPIO_PORTC, 3}, /**< Solenoid Valve 2 (Pin 34) */
    {GPIO_PORTC, 4}, /**< Solenoid Valve 3 (Pin 33) */
    {GPIO_PORTC, 5}, /**< Solenoid Valve 4 (Pin 32) */
    {GPIO_PORTC, 6}  /**< Solenoid Valve 5 (Pin 31) */
};

#define VALVE_ENERGIZED   GPIO_HIGH
#define VALVE_DEENERGIZED GPIO_LOW

static bool valve_states[VALVES_NUM_SLOTS] = {false, false, false, false, false};

void Valves_Init(void) {
    for (uint8_t i = 0; i < VALVES_NUM_SLOTS; i++) {
        GPIO_Init(valve_hardware_pins[i].port, valve_hardware_pins[i].pin, GPIO_OUTPUT);
        GPIO_Write(valve_hardware_pins[i].port, valve_hardware_pins[i].pin, VALVE_DEENERGIZED);
        valve_states[i] = false;
    }
}

void Valves_Open(uint8_t slot_idx) {
    if (slot_idx >= VALVES_NUM_SLOTS) return;
    GPIO_Write(valve_hardware_pins[slot_idx].port, valve_hardware_pins[slot_idx].pin, VALVE_ENERGIZED);
    valve_states[slot_idx] = true;
}

void Valves_Close(uint8_t slot_idx) {
    if (slot_idx >= VALVES_NUM_SLOTS) return;
    GPIO_Write(valve_hardware_pins[slot_idx].port, valve_hardware_pins[slot_idx].pin, VALVE_DEENERGIZED);
    valve_states[slot_idx] = false;
}

void Valves_CloseAll(void) {
    for (uint8_t i = 0; i < VALVES_NUM_SLOTS; i++) {
        Valves_Close(i);
    }
}

bool Valves_IsOpen(uint8_t slot_idx) {
    if (slot_idx >= VALVES_NUM_SLOTS) return false;
    return valve_states[slot_idx];
}
