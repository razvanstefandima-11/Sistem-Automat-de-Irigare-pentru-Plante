#include "capacitive_moisture.h"
#include "drivers/adc/adc.h"

/**
 * @file capacitive_moisture.c
 * @brief Rockwool-calibrated capacitive moisture sensor implementation.
 */

static rockwool_slot_calib_t slots[NUM_MOISTURE_SLOTS];

void CapacitiveMoisture_Init(void) {
    ADC_Init();

    // Default configuration: Slots 0-4 mapped to ADC0-ADC4
    for (uint8_t i = 0; i < NUM_MOISTURE_SLOTS; i++) {
        slots[i].adc_channel = i;
        slots[i].calib_dry   = ROCKWOOL_DEFAULT_RAW_DRY;
        slots[i].calib_wet   = ROCKWOOL_DEFAULT_RAW_WET;
    }
}

void CapacitiveMoisture_ConfigureSlot(uint8_t slot, uint8_t adc_channel, uint16_t dry_val, uint16_t wet_val) {
    if (slot >= NUM_MOISTURE_SLOTS) return;

    slots[slot].adc_channel = adc_channel;
    slots[slot].calib_dry   = dry_val;
    slots[slot].calib_wet   = wet_val;
}

uint16_t CapacitiveMoisture_ReadRaw(uint8_t slot) {
    if (slot >= NUM_MOISTURE_SLOTS) return 0;

    // 4-sample oversampling for noise filtering
    uint32_t sum = 0;
    for (uint8_t i = 0; i < 4; i++) {
        sum += ADC_Read(slots[slot].adc_channel);
    }
    return (uint16_t)(sum / 4);
}

uint8_t CapacitiveMoisture_GetPercentage(uint8_t slot) {
    if (slot >= NUM_MOISTURE_SLOTS) return 0;

    uint16_t raw = CapacitiveMoisture_ReadRaw(slot);
    uint16_t dry = slots[slot].calib_dry;
    uint16_t wet = slots[slot].calib_wet;

    // Inverse capacitive relationship: higher raw ADC = lower moisture (drier)
    if (raw >= dry) {
        return 0;
    }
    if (raw <= wet) {
        return 100;
    }

    uint32_t span = (uint32_t)(dry - wet);
    uint32_t delta = (uint32_t)(dry - raw);

    return (uint8_t)((delta * 100UL) / span);
}

bool CapacitiveMoisture_NeedsWater(uint8_t slot, uint8_t target_threshold) {
    uint8_t moisture = CapacitiveMoisture_GetPercentage(slot);
    return (moisture < target_threshold);
}
