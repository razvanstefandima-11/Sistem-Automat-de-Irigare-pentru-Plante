#ifndef NIVEL_APA_H
#define NIVEL_APA_H

#include <stdint.h>

/**
 * @brief Inițializează pinul de alimentare pentru senzorul de apă.
 */
void NivelApa_Init(void);

/**
 * @brief Citește nivelul apei din rezervorul de încălzire al camerei.
 * Alimentează senzorul temporar, face conversia ADC, apoi taie alimentarea.
 * * @return uint16_t Valoarea brută (0-1023) a nivelului de apă.
 */
uint16_t NivelApa_Citeste(void);

#endif // NIVEL_APA_H