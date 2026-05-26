#ifndef NIVEL_APA_H_
#define NIVEL_APA_H_

#include <stdint.h>

/**
 * @brief Inițializează hardware-ul pentru senzorul de nivel de apă.
 * Deoarece senzorul este analogic, pinii ADC sunt configurați global.
 */
void NivelApa_Init(void);

/**
 * @brief Citește starea senzorului de nivel de apă de pe pinul A3.
 * @return 1 dacă există apă în rezervor (OK), 0 dacă rezervorul este gol.
 */
uint8_t NivelApa_Read(void);

#endif /* NIVEL_APA_H_ */