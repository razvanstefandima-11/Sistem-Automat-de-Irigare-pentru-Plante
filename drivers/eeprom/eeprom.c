#include "eeprom.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * @file eeprom.c
 * @brief Internal EEPROM access and plant stage persistence implementation.
 */

uint8_t EEPROM_Read(uint16_t address) {
    if (address >= EEPROM_SIZE_BYTES) return 0xFF;

    // Wait for completion of previous write
    while (EECR & (1 << EEPE))
        ;

    // Set Address Register
    EEAR = address;

    // Trigger EEPROM Read
    EECR |= (1 << EERE);

    return EEDR;
}

void EEPROM_Write(uint16_t address, uint8_t data) {
    if (address >= EEPROM_SIZE_BYTES) return;

    // Wait for completion of previous write
    while (EECR & (1 << EEPE))
        ;

    // Set Address and Data Registers
    EEAR = address;
    EEDR = data;

    // Critical section for 4-cycle write handshake
    uint8_t old_sreg = SREG;
    cli();
    EECR |= (1 << EEMPE);
    EECR |= (1 << EEPE);
    SREG = old_sreg;
}

void EEPROM_Update(uint16_t address, uint8_t data) {
    if (EEPROM_Read(address) != data) {
        EEPROM_Write(address, data);
    }
}

void EEPROM_SaveSlotStage(uint8_t slot_idx, uint8_t stage) {
    if (slot_idx > 10) return;
    uint16_t addr = EEPROM_STAGE_STORAGE_BASE + slot_idx;
    EEPROM_Update(addr, stage);
}

uint8_t EEPROM_LoadSlotStage(uint8_t slot_idx) {
    if (slot_idx > 10) return 0;
    uint16_t addr = EEPROM_STAGE_STORAGE_BASE + slot_idx;
    uint8_t stage = EEPROM_Read(addr);
    // If EEPROM is unprogrammed (0xFF), default to stage 0 (Seedling)
    if (stage > 2) {
        return 0;
    }
    return stage;
}
