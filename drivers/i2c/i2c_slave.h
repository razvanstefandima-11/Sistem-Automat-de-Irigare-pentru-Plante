#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#include <stdint.h>

// Inițializează modulul TWI în modul Slave și îl pune în ascultare
void I2C_Slave_Init(uint8_t slave_addr);

#endif // I2C_SLAVE_H