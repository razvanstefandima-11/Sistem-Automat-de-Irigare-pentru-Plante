#ifndef POMPE_H
#define POMPE_H

#include <stdint.h>

/**
 * @brief Inițializează pinii pentru modulul de relee.
 * Setează pinii ca OUTPUT și oprește pompele preventiv.
 */
void Pompe_Init(void);

/**
 * @brief Pornește Pompa 1 (Planta 1).
 */
void Pompa1_On(void);

/**
 * @brief Oprește Pompa 1 (Planta 1).
 */
void Pompa1_Off(void);

/**
 * @brief Pornește Pompa 2 (Planta 2).
 */
void Pompa2_On(void);

/**
 * @brief Oprește Pompa 2 (Planta 2).
 */
void Pompa2_Off(void);

/**
 * @brief Pornește Pompa 3 (Planta 3).
 */
void Pompa3_On(void);

/**
 * @brief Oprește Pompa 3 (Planta 3).
 */
void Pompa3_Off(void);

/**
 * @brief Failsafe: Oprește absolut toate pompele (în caz că rezervorul e gol).
 */
void Pompe_OprireUrgenta(void);

#endif // POMPE_H