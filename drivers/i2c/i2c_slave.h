#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#include <stdint.h>

/**
 * @brief Initializeaza hardware-ul TWI (I2C) in modul Slave.
 * @param address Adresa pe 7 biti la care va raspunde placa Nano.
 */
void i2c_slave_init(uint8_t address);

/**
 * @brief Verifica daca au fost receptionate date noi si le preia in siguranta.
 * @param data Pointer catre variabila in care se va salva byte-ul primit.
 * @return 1 daca avem date noi de la Master, 0 daca nu.
 */
uint8_t i2c_slave_has_data(uint8_t *data);

#endif /* I2C_SLAVE_H */