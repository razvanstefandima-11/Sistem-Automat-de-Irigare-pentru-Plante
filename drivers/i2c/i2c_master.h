#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include <stdint.h>

// Adresa de I2C pe care o va avea Arduino Nano (Slave)
#define NANO_SLAVE_ADDR  0x3A

// Inițializare magistrală I2C Master
void I2C_Master_Init(void);

// Funcții de control de bază (Hardware TWI)
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Write(uint8_t data);

// Funcție utilitară pentru a trimite un singur octet direct către un Slave
void I2C_Master_Send_Cmd(uint8_t slave_addr, uint8_t cmd);

#endif // I2C_MASTER_H