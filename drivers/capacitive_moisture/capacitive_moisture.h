#ifndef CAPACITIVE_MOISTURE_H
#define CAPACITIVE_MOISTURE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file capacitive_moisture.h
 * @brief Capacitive Soil Moisture Driver Calibrated for Rockwool (C.A.M.P.I. Master).
 * 
 * Supports 5 independent dock/pot slots on ATmega2560 ADC channels.
 * Accounts for the dielectric properties and water retention curves of agricultural
 * mineral wool (Rockwool).
 */

#define NUM_MOISTURE_SLOTS 5

// Rockwool Dielectric Calibration Reference Points (10-bit ADC, 0-1023)
#define ROCKWOOL_DEFAULT_RAW_DRY 620 /**< Raw ADC value in completely dry mineral wool */
#define ROCKWOOL_DEFAULT_RAW_WET 240 /**< Raw ADC value in 100% water saturated mineral wool */

// Phenological Target Thresholds (% Moisture)
#define MOISTURE_TARGET_SEEDLING    60 /**< 1. Răsad: 60% (Substrat Umed) */
#define MOISTURE_TARGET_VEGETATIVE  50 /**< 2. Creștere Vegetativă: 50% (Echilibrat) */
#define MOISTURE_TARGET_FLOWERING   40 /**< 3. Înflorire/Fruct: 40% (Stres Hidric Controlat) */

typedef struct {
    uint8_t adc_channel;       /**< ADC channel number (0-15) */
    uint16_t calib_dry;        /**< Raw value when dry */
    uint16_t calib_wet;        /**< Raw value when wet */
} rockwool_slot_calib_t;

/**
 * @brief Initializes the capacitive moisture subsystem.
 * Sets default ADC channels for the 5 slots (e.g. ADC0 - ADC4) and default Rockwool calibration.
 */
void CapacitiveMoisture_Init(void);

/**
 * @brief Sets custom calibration values for a specific slot.
 * 
 * @param slot Index of slot (0 to NUM_MOISTURE_SLOTS - 1).
 * @param adc_channel ADC channel assigned to slot.
 * @param dry_val Raw ADC value when rockwool is dry.
 * @param wet_val Raw ADC value when rockwool is wet.
 */
void CapacitiveMoisture_ConfigureSlot(uint8_t slot, uint8_t adc_channel, uint16_t dry_val, uint16_t wet_val);

/**
 * @brief Reads the raw ADC value with oversampling and anti-noise filtering.
 * 
 * @param slot Index of slot (0 to NUM_MOISTURE_SLOTS - 1).
 * @return uint16_t Filtered 10-bit raw ADC reading.
 */
uint16_t CapacitiveMoisture_ReadRaw(uint8_t slot);

/**
 * @brief Computes moisture percentage calibrated for the Rockwool substrate.
 * 
 * @param slot Index of slot (0 to NUM_MOISTURE_SLOTS - 1).
 * @return uint8_t Moisture percentage (0% to 100%).
 */
uint8_t CapacitiveMoisture_GetPercentage(uint8_t slot);

/**
 * @brief Checks if a slot requires irrigation based on its target threshold.
 * 
 * @param slot Index of slot.
 * @param target_threshold Target moisture percentage.
 * @return true if moisture < target_threshold, false otherwise.
 */
bool CapacitiveMoisture_NeedsWater(uint8_t slot, uint8_t target_threshold);

#endif // CAPACITIVE_MOISTURE_H
