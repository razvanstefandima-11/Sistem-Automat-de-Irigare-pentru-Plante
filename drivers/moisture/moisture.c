#include "moisture.h"
#include "drivers/adc/adc.h"

/**
 * @file moisture.c
 * @brief Rockwool moisture sensor implementation.
 */

static moisture_calib_t slot_configs[MOISTURE_NUM_SLOTS];

void Moisture_Init(void) {
    ADC_Init();

    for (uint8_t i = 0; i < MOISTURE_NUM_SLOTS; i++) {
        slot_configs[i].adc_channel = i; // ADC0 through ADC4 (PF0 to PF4)
        slot_configs[i].raw_dry     = MOISTURE_RAW_ROCKWOOL_DRY;
        slot_configs[i].raw_wet     = MOISTURE_RAW_ROCKWOOL_WET;
    }
}

uint16_t Moisture_ReadRaw(uint8_t slot_idx) {
    if (slot_idx >= MOISTURE_NUM_SLOTS) return 0;

    // 4-sample moving average for noise immunity
    uint32_t sum = 0;
    for (uint8_t i = 0; i < 4; i++) {
        sum += ADC_Read(slot_configs[slot_idx].adc_channel);
    }
    return (uint16_t)(sum / 4);
}

uint8_t Moisture_GetPercentage(uint8_t slot_idx) {
    if (slot_idx >= MOISTURE_NUM_SLOTS) return 0;

    uint16_t raw = Moisture_ReadRaw(slot_idx);
    uint16_t dry = slot_configs[slot_idx].raw_dry;
    uint16_t wet = slot_configs[slot_idx].raw_wet;

    // Capacitive curve: Dry = Higher Raw ADC; Wet = Lower Raw ADC
    if (raw >= dry) return 0;
    if (raw <= wet) return 100;

    uint32_t span = (uint32_t)(dry - wet);
    uint32_t delta = (uint32_t)(dry - raw);

    return (uint8_t)((delta * 100UL) / span);
}

void Moisture_Calibrate(uint8_t slot_idx, uint16_t dry_val, uint16_t wet_val) {
    if (slot_idx >= MOISTURE_NUM_SLOTS) return;
    slot_configs[slot_idx].raw_dry = dry_val;
    slot_configs[slot_idx].raw_wet = wet_val;
}

bool Moisture_NeedsWater(uint8_t slot_idx, uint8_t target_pct) {
    return (Moisture_GetPercentage(slot_idx) < target_pct);
}
