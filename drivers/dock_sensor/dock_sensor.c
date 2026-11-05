#include "dock_sensor.h"
#include "drivers/gpio/gpio.h"

/**
 * @file dock_sensor.c
 * @brief Hall sensor dock detection implementation with 5-second validation.
 */

// Hardware Pin Mappings: Docks 0-4 on PORTL (Pins 49, 48, 47, 46, 45 on Arduino Mega)
static const struct {
    gpio_port_t port;
    uint8_t pin;
} dock_pins[DOCK_NUM_SLOTS] = {
    {GPIO_PORTL, 0}, // Dock 0 / Slot 1 (Pin 49)
    {GPIO_PORTL, 1}, // Dock 1 / Slot 2 (Pin 48)
    {GPIO_PORTL, 2}, // Dock 2 / Slot 3 (Pin 47)
    {GPIO_PORTL, 3}, // Dock 3 / Slot 4 (Pin 46)
    {GPIO_PORTL, 4}  // Dock 4 / Slot 5 (Pin 45)
};

typedef struct {
    dock_status_t status;
    uint32_t insertion_start_ms;
    bool just_validated_event;
} dock_channel_t;

static dock_channel_t docks[DOCK_NUM_SLOTS];

void DockSensor_Init(void) {
    for (uint8_t i = 0; i < DOCK_NUM_SLOTS; i++) {
        GPIO_Init(dock_pins[i].port, dock_pins[i].pin, GPIO_INPUT_PULLUP);
        docks[i].status = DOCK_STATUS_EMPTY;
        docks[i].insertion_start_ms = 0;
        docks[i].just_validated_event = false;
    }
}

void DockSensor_Update(uint32_t current_millis) {
    for (uint8_t i = 0; i < DOCK_NUM_SLOTS; i++) {
        // A3144 Hall Switch: Output goes LOW when magnetic south pole is detected
        bool magnet_present = (GPIO_Read(dock_pins[i].port, dock_pins[i].pin) == GPIO_LOW);

        switch (docks[i].status) {
            case DOCK_STATUS_EMPTY:
                if (magnet_present) {
                    docks[i].status = DOCK_STATUS_INSERTING;
                    docks[i].insertion_start_ms = current_millis;
                }
                break;

            case DOCK_STATUS_INSERTING:
                if (!magnet_present) {
                    // Drawer pulled out or bounced before 5 seconds elapsed
                    docks[i].status = DOCK_STATUS_EMPTY;
                } else if ((current_millis - docks[i].insertion_start_ms) >= DOCK_VALIDATION_DELAY_MS) {
                    // Validated after continuous 5 seconds of steady magnetic lock
                    docks[i].status = DOCK_STATUS_CONFIRMED;
                    docks[i].just_validated_event = true;
                }
                break;

            case DOCK_STATUS_CONFIRMED:
                if (!magnet_present) {
                    // Pot removed from dock
                    docks[i].status = DOCK_STATUS_EMPTY;
                    docks[i].insertion_start_ms = 0;
                }
                break;
        }
    }
}

dock_status_t DockSensor_GetStatus(uint8_t slot_idx) {
    if (slot_idx >= DOCK_NUM_SLOTS) return DOCK_STATUS_EMPTY;
    return docks[slot_idx].status;
}

bool DockSensor_HasJustValidated(uint8_t slot_idx) {
    if (slot_idx >= DOCK_NUM_SLOTS) return false;
    bool evt = docks[slot_idx].just_validated_event;
    docks[slot_idx].just_validated_event = false;
    return evt;
}

bool DockSensor_IsDockReady(uint8_t slot_idx) {
    if (slot_idx >= DOCK_NUM_SLOTS) return false;
    return (docks[slot_idx].status == DOCK_STATUS_CONFIRMED);
}
