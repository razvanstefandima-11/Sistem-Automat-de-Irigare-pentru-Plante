#include "eeprom.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * @brief Reads a single byte from the internal EEPROM.
 * 
 * Blocks until any ongoing write operation is complete, then
 * configures the address register and triggers the read.
 * 
 * @param address The EEPROM address (0 - 1023).
 * @return uint8_t The byte stored at the address.
 */
uint8_t EEPROM_Read(uint16_t address) {
    // Wait for completion of previous write
    while (EECR & (1 << EEPE));

    // Set up address register
    EEAR = address;

    // Start eeprom read
    EECR |= (1 << EERE);

    // Return data
    return EEDR;
}

/**
 * @brief Writes a single byte to the internal EEPROM.
 * 
 * Blocks until any ongoing write operation is complete.
 * Follows the specific write sequence required by the AVR architecture:
 * 1. Set Address and Data registers.
 * 2. Write logical one to EEMPE (Master Write Enable).
 * 3. Write logical one to EEPE (Write Enable) within 4 clock cycles.
 * Interrupts are briefly disabled to ensure the timing sequence is met.
 * 
 * @param address The EEPROM address to write to.
 * @param data The byte to write.
 */
void EEPROM_Write(uint16_t address, uint8_t data) {
    // Wait for completion of previous write
    while (EECR & (1 << EEPE));

    // Set up address and Data Registers
    EEAR = address;
    EEDR = data;

    // Critical section for write sequence
    uint8_t oldSREG = SREG;
    cli(); 
    EECR |= (1 << EEMPE);
    EECR |= (1 << EEPE);
    SREG = oldSREG; 
}

/**
 * @brief Updates a byte in the EEPROM.
 * 
 * Reads the current value at the address and writes the new value
 * ONLY if it differs. This helps prolong the lifespan of the EEPROM,
 * which has a limited number of write cycles (~100.000).
 * 
 * @param address The EEPROM address to update.
 * @param data The new byte to store.
 */
void EEPROM_Update(uint16_t address, uint8_t data) {
    uint8_t current_val = EEPROM_Read(address);
    if (current_val != data) {
        EEPROM_Write(address, data);
    }
}