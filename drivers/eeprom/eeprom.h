#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>

/**
 * @file eeprom.h
 * @brief Internal EEPROM Driver for ATmega2560 (4096 Bytes Storage).
 * 
 * Provides bare-metal byte and block read/write/update functions,
 * and high-level persistence routines for saving plant growth stages per slot.
 */

#define EEPROM_SIZE_BYTES 4096U
#define EEPROM_STAGE_STORAGE_BASE 0x0020U

/**
 * @brief Reads a single byte from internal EEPROM.
 * 
 * @param address 16-bit address (0 - 4095).
 * @return uint8_t Data byte.
 */
uint8_t EEPROM_Read(uint16_t address);

/**
 * @brief Writes a single byte to internal EEPROM.
 * 
 * @param address 16-bit address.
 * @param data Data byte to write.
 */
void EEPROM_Write(uint16_t address, uint8_t data);

/**
 * @brief Updates a byte in EEPROM (writes only if new value differs, saving wear).
 * 
 * @param address 16-bit address.
 * @param data Data byte.
 */
void EEPROM_Update(uint16_t address, uint8_t data);

/**
 * @brief Saves the phenological growth stage for a specific dock slot to non-volatile EEPROM.
 * 
 * @param slot_idx Index of dock slot (0 to 4).
 * @param stage Stored stage code (0: Seedling, 1: Vegetative, 2: Flowering).
 */
void EEPROM_SaveSlotStage(uint8_t slot_idx, uint8_t stage);

/**
 * @brief Loads the saved phenological growth stage for a specific dock slot from EEPROM.
 * 
 * @param slot_idx Index of dock slot (0 to 4).
 * @return uint8_t Stage code (defaults to 0: Seedling if uninitialized).
 */
uint8_t EEPROM_LoadSlotStage(uint8_t slot_idx);

#endif // EEPROM_H
