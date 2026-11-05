#include "pumps.h"
#include "drivers/gpio/gpio.h"
#include "drivers/float_switch/float_switch.h"

/**
 * @file pumps.c
 * @brief Pump control implementation for C.A.M.P.I.
 */

// Hardware Pin Mappings on ATmega2560
#define PIN_PUMP_A_WATER  GPIO_PORTC, 0 /**< Relay Channel 1: Clean Water (Pin 37) */
#define PIN_PUMP_B_NUTR   GPIO_PORTC, 1 /**< Relay Channel 2: Nutrients (Pin 36) */

#define RELAY_ACTIVE   GPIO_HIGH
#define RELAY_INACTIVE GPIO_LOW

static bool pump_water_state = false;
static bool pump_nutr_state = false;

void Pumps_Init(void) {
    GPIO_Init(PIN_PUMP_A_WATER, GPIO_OUTPUT);
    GPIO_Init(PIN_PUMP_B_NUTR, GPIO_OUTPUT);
    Pumps_StopAll();
}

bool Pumps_SetWater(bool enable) {
    if (enable) {
        // Safety Interlock: Prohibit pump operation if tank is confirmed dry
        if (FloatSwitch_IsDryRunLocked()) {
            Pumps_SetWater(false);
            return false;
        }
        GPIO_Write(PIN_PUMP_A_WATER, RELAY_ACTIVE);
        pump_water_state = true;
    } else {
        GPIO_Write(PIN_PUMP_A_WATER, RELAY_INACTIVE);
        pump_water_state = false;
    }
    return true;
}

bool Pumps_SetNutrients(bool enable) {
    if (enable) {
        // Safety Interlock: Prohibit pump operation if tank is confirmed dry
        if (FloatSwitch_IsDryRunLocked()) {
            Pumps_SetNutrients(false);
            return false;
        }
        GPIO_Write(PIN_PUMP_B_NUTR, RELAY_ACTIVE);
        pump_nutr_state = true;
    } else {
        GPIO_Write(PIN_PUMP_B_NUTR, RELAY_INACTIVE);
        pump_nutr_state = false;
    }
    return true;
}

void Pumps_StopAll(void) {
    GPIO_Write(PIN_PUMP_A_WATER, RELAY_INACTIVE);
    GPIO_Write(PIN_PUMP_B_NUTR, RELAY_INACTIVE);
    pump_water_state = false;
    pump_nutr_state = false;
}

bool Pumps_IsWaterRunning(void) {
    return pump_water_state;
}

bool Pumps_IsNutrientsRunning(void) {
    return pump_nutr_state;
}
