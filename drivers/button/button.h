#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

/**
 * @brief Inițializează pinii pentru cele 3 butoane cu Pull-up intern.
 * PB1 (D9) = MINUS
 * PB2 (D10) = SELECT
 * PB5 (D13) = PLUS
 */
void BUTTON_Init(void);

/**
 * @brief Verifică dacă butonul MINUS este apăsat.
 * @return 1 dacă este apăsat, 0 altfel.
 */
uint8_t BUTTON_IsMinusPressed(void);

/**
 * @brief Verifică dacă butonul SELECT este apăsat.
 * @return 1 dacă este apăsat, 0 altfel.
 */
uint8_t BUTTON_IsSelectPressed(void);

/**
 * @brief Verifică dacă butonul PLUS este apăsat.
 * @return 1 dacă este apăsat, 0 altfel.
 */
uint8_t BUTTON_IsPlusPressed(void);

#endif /* BUTTON_H */