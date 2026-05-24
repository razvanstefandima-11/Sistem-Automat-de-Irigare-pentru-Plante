#ifndef S_UMIDITATE_H
#define S_UMIDITATE_H

#include <stdint.h>

// Nivelurile de umiditate abstractizate
typedef enum {
    S_UMIDITATE_VERY_LOW,  // Pământ complet uscat (valori ADC mari)
    S_UMIDITATE_LOW,       // Uscat, necesită apă
    S_UMIDITATE_MEDIUM,    // Umiditate optimă
    S_UMIDITATE_HIGH,      // Pământ ud
    S_UMIDITATE_VERY_HIGH  // Senzor inundat/în apă (valori ADC mici)
} SUmiditateLevel_t;

// Structura pentru configurarea pragurilor senzorului
typedef struct {
    uint16_t very_high_threshold; 
    uint16_t high_threshold;      
    uint16_t medium_threshold;    
    uint16_t low_threshold;       
} SUmiditateConfig_t;

// Inițializare (dacă este nevoie de setup specific pe viitor)
void S_Umiditate_Init(void);

// Configurează pragurile pentru un senzor specific
void S_Umiditate_SetConfig(SUmiditateConfig_t* config, uint16_t th_vh, uint16_t th_h, uint16_t th_m, uint16_t th_l);

// Returnează starea senzorului pe baza valorii citite
SUmiditateLevel_t S_Umiditate_GetLevel(uint8_t adc_channel, SUmiditateConfig_t* config);

#endif // S_UMIDITATE_H