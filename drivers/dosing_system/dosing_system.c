#include "dosing_system.h"
#include "drivers/float_switch/float_switch.h"

/**
 * @file dosing_system.c
 * @brief Time-proportional dosing and manifold flush state machine implementation.
 */

// Hardware relay pin mappings on ATmega2560 (Port C)
#define PIN_PUMP_A_WATER   GPIO_PORTC, 0 /**< Clean Water Pump A (Pin 37) */
#define PIN_PUMP_B_NUTR    GPIO_PORTC, 1 /**< Nutrient Pump B (Pin 36) */

static const struct {
    gpio_port_t port;
    uint8_t pin;
} valve_pins[DOSING_NUM_VALVES] = {
    {GPIO_PORTC, 2}, /**< Valve 0 (Pin 35) */
    {GPIO_PORTC, 3}, /**< Valve 1 (Pin 34) */
    {GPIO_PORTC, 4}, /**< Valve 2 (Pin 33) */
    {GPIO_PORTC, 5}, /**< Valve 3 (Pin 32) */
    {GPIO_PORTC, 6}  /**< Valve 4 (Pin 31) */
};

#define RELAY_ON  GPIO_HIGH
#define RELAY_OFF GPIO_LOW

// State Machine Context
static dosing_state_t current_state = DOSING_IDLE;
static uint8_t active_slot = 0;
static uint32_t state_start_ms = 0;
static uint16_t time_nutrients_ms = 0;
static uint16_t time_water_flush_ms = 0;

static void set_pump_a(bool enable) {
    GPIO_Write(PIN_PUMP_A_WATER, enable ? RELAY_ON : RELAY_OFF);
}

static void set_pump_b(bool enable) {
    GPIO_Write(PIN_PUMP_B_NUTR, enable ? RELAY_ON : RELAY_OFF);
}

static void set_valve(uint8_t slot, bool open) {
    if (slot < DOSING_NUM_VALVES) {
        GPIO_Write(valve_pins[slot].port, valve_pins[slot].pin, open ? RELAY_ON : RELAY_OFF);
    }
}

static void close_all_valves(void) {
    for (uint8_t i = 0; i < DOSING_NUM_VALVES; i++) {
        set_valve(i, false);
    }
}

void Dosing_Init(void) {
    // Configure Pump GPIOs
    GPIO_Init(PIN_PUMP_A_WATER, GPIO_OUTPUT);
    GPIO_Init(PIN_PUMP_B_NUTR, GPIO_OUTPUT);
    set_pump_a(false);
    set_pump_b(false);

    // Configure Valve GPIOs
    for (uint8_t i = 0; i < DOSING_NUM_VALVES; i++) {
        GPIO_Init(valve_pins[i].port, valve_pins[i].pin, GPIO_OUTPUT);
        set_valve(i, false);
    }

    current_state = DOSING_IDLE;
}

bool Dosing_Start(uint8_t slot_idx, uint16_t total_duration_ms, uint8_t water_percent) {
    if (slot_idx >= DOSING_NUM_VALVES || current_state != DOSING_IDLE) {
        return false;
    }

    // Safety Interlock: Prohibit pump activation if Dry-Run is engaged
    if (FloatSwitch_IsDryRunLocked()) {
        return false;
    }

    if (water_percent > 100) water_percent = 100;

    active_slot = slot_idx;
    time_water_flush_ms = (uint16_t)(((uint32_t)total_duration_ms * water_percent) / 100UL);
    time_nutrients_ms   = total_duration_ms - time_water_flush_ms;

    // Begin sequence: open target valve first
    set_valve(active_slot, true);
    current_state = DOSING_OPENING_VALVE;
    state_start_ms = 0; // Updated on next update tick

    return true;
}

bool Dosing_StartStageDosing(uint8_t slot_idx, uint16_t total_duration_ms, phenological_stage_t stage) {
    uint8_t water_pct = 90;
    switch (stage) {
        case STAGE_SEEDLING:   water_pct = 90; break; // 90% Water / 10% Nutrients
        case STAGE_VEGETATIVE: water_pct = 60; break; // 60% Water / 40% Nutrients
        case STAGE_FLOWERING:  water_pct = 30; break; // 30% Water / 70% Nutrients
    }
    return Dosing_Start(slot_idx, total_duration_ms, water_pct);
}

bool Dosing_StartPurgeAndFlush(uint8_t slot_idx, uint16_t flush_duration_ms) {
    // 100% Water flush
    return Dosing_Start(slot_idx, flush_duration_ms, 100);
}

void Dosing_Update(uint32_t current_millis) {
    // Hardware Safety Check: Abort if dry-run tripped during dosing
    if (current_state != DOSING_IDLE && FloatSwitch_IsDryRunLocked()) {
        Dosing_EmergencyStop();
        return;
    }

    switch (current_state) {
        case DOSING_IDLE:
            break;

        case DOSING_OPENING_VALVE:
            // Allow 100ms for solenoid to mechanically seat before activating pressure
            if (state_start_ms == 0) state_start_ms = current_millis;
            if ((current_millis - state_start_ms) >= 100) {
                if (time_nutrients_ms > 0) {
                    set_pump_b(true); // Start Nutrient Concentrate Pump
                    current_state = DOSING_RUNNING_NUTRIENTS;
                    state_start_ms = current_millis;
                } else {
                    // Skip to water flush directly
                    set_pump_a(true);
                    current_state = DOSING_RUNNING_WATER_FLUSH;
                    state_start_ms = current_millis;
                }
            }
            break;

        case DOSING_RUNNING_NUTRIENTS:
            if ((current_millis - state_start_ms) >= time_nutrients_ms) {
                set_pump_b(false); // Stop Pump B

                // Immediate switch to Clean Water (Purge & Flush)
                set_pump_a(true);  // Start Pump A
                current_state = DOSING_RUNNING_WATER_FLUSH;
                state_start_ms = current_millis;
            }
            break;

        case DOSING_RUNNING_WATER_FLUSH:
            if ((current_millis - state_start_ms) >= time_water_flush_ms) {
                set_pump_a(false); // Stop Pump A
                current_state = DOSING_CLOSING_VALVE;
                state_start_ms = current_millis;
            }
            break;

        case DOSING_CLOSING_VALVE:
            // Settle pressure before closing valve (150ms)
            if ((current_millis - state_start_ms) >= 150) {
                set_valve(active_slot, false);
                current_state = DOSING_IDLE;
            }
            break;
    }
}

void Dosing_EmergencyStop(void) {
    set_pump_a(false);
    set_pump_b(false);
    close_all_valves();
    current_state = DOSING_IDLE;
}

bool Dosing_IsBusy(void) {
    return (current_state != DOSING_IDLE);
}
