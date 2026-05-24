#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

// Inițializează pinul de control al buzzer-ului
void Buzzer_Init(void);

// Pornește sunetul
void Buzzer_On(void);

// Oprește sunetul
void Buzzer_Off(void);

// Schimbă starea buzzer-ului (util pentru alarme intermitente)
void Buzzer_Toggle(void);

#endif // BUZZER_H