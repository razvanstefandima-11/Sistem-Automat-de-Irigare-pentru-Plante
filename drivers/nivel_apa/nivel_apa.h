#ifndef NIVEL_APA_H
#define NIVEL_APA_H

#include <stdint.h>

/**
 * @brief Inițializează hardware-ul pentru senzorul de apă (inclusiv pinul de alimentare).
 */
void NivelApa_Init(void);

/**
 * @brief Alimentează senzorul, citește nivelul și oprește alimentarea (anti-electroliză).
 * @return 1 dacă există apă în rezervor (OK), 0 dacă rezervorul este gol.
 */
uint8_t NivelApa_Read(void);

#endif // NIVEL_APA_H