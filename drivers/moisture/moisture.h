#ifndef MOISTURE_H
#define MOISTURE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file moisture.h
 * @brief Capacitive Moisture Sensor Driver calibrated for Rockwool Substrate.
 * 
 * Supports 5 modular dock slots using ATmega2560 ADC channels.
 * Calibrated for agricultural mineral wool dielectric constant.
 */

#define MOISTURE_NUM_SLOTS 5

// Rockwool Calibration Defaults (10-bit ADC, 0-1023)
#define MOISTURE_RAW_ROCKWOOL_DRY 620 /**< Raw ADC in completely dry rockwool */
#define MOISTURE_RAW_ROCKWOOL_WET 240 /**< Raw ADC in 100% field capacity saturated rockwool */

// Target moisture percentage per phenological stage
#define MOISTURE_TARGET_SEEDLING   60 /**< 1. Răsad: 60% */
#define MOISTURE_TARGET_VEGETATIVE 50 /**< 2. Creștere: 50% */
#define MOISTURE_TARGET_FLOWERING  40 /**< 3. Înflorire/Fruct: 40% (Stres hidric controlat) */

typedef struct {
    uint8_t adc_channel;
    uint16_t raw_dry;
    uint16_t raw_wet;
} moisture_calib_t;

/**
 * @brief Initializes the moisture sensing subsystem (ADC0 to ADC4).
 */
void Moisture_Init(void);

/**
 * @brief Reads the raw ADC value for a specific slot with oversampling.
 * 
 * @param slot_idx Slot index (0 to 4).
 * @return uint16_t 10-bit raw ADC reading.
 */
uint16_t Moisture_ReadRaw(uint8_t slot_idx);

/**
 * @brief Computes moisture percentage (0 - 100%) for a slot.
 * 
 * @param slot_idx Slot index (0 to 4).
 * @return uint8_t Volumetric Moisture Percentage (0-100%).
 */
uint8_t Moisture_GetPercentage(uint8_t slot_idx);

/**
 * @brief Calibrates a slot with custom dry and wet reference points.
 * 
 * @param slot_idx Slot index (0 to 4).
 * @param dry_val Raw ADC value when rockwool is dry.
 * @param wet_val Raw ADC value when rockwool is fully saturated.
 */
void Moisture_Calibrate(uint8_t slot_idx, uint16_t dry_val, uint16_t wet_val);

/**
 * @brief Checks if a slot requires watering based on a target threshold.
 * 
 * @param slot_idx Slot index (0 to 4).
 * @param target_pct Threshold percentage.
 * @return true if moisture < target_pct, false otherwise.
 */
bool Moisture_NeedsWater(uint8_t slot_idx, uint8_t target_pct);

#endif // MOISTURE_H
