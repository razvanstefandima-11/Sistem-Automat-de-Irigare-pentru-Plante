#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include <stdint.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define F_SCL 100000UL // Frecvența I2C Standard: 100 kHz
#define PRESCALER_1 1

/**
 * @brief Inițializează hardware-ul TWI (I2C) ca Master.
 * Configurează pinii SDA și SCL și setează frecvența (Baud Rate).
 */
void i2c_master_init(void);

/**
 * @brief Generează condiția de START pe magistrală.
 * @return 1 pentru succes (magistrala preluată), 0 pentru timeout/eroare.
 */
uint8_t i2c_master_start(void);

/**
 * @brief Generează condiția de STOP, eliberând magistrala.
 */
void i2c_master_stop(void);

/**
 * @brief Trimite un byte (adresă sau date) și așteaptă confirmarea (ACK).
 * @param data Byte-ul care trebuie trimis.
 * @return 1 pentru ACK primit (succes), 0 pentru NACK sau timeout.
 */
uint8_t i2c_master_write(uint8_t data);

#endif /* I2C_MASTER_H */