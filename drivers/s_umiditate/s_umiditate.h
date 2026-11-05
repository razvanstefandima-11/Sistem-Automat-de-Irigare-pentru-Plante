#ifndef S_UMIDITATE_H
#define S_UMIDITATE_H

#include <stdint.h>

/**
 * @brief Nivelurile de umiditate abstractizate.
 * Atenție: La senzorii capacitivi/rezistivi standard, valori ADC mari = uscat.
 */
typedef enum {
    S_UMIDITATE_VERY_LOW,  
    S_UMIDITATE_LOW,       
    S_UMIDITATE_MEDIUM,    
    S_UMIDITATE_HIGH,      
    S_UMIDITATE_VERY_HIGH  
} SUmiditateLevel_t;

/**
 * @brief Structura pentru configurarea pragurilor senzorului.
 */
typedef struct {
    uint16_t very_high_threshold; 
    uint16_t high_threshold;      
    uint16_t medium_threshold;    
    uint16_t low_threshold;       
} SUmiditateConfig_t;

/**
 * @brief Inițializare hardware pentru senzorii de umiditate.
 */
void S_Umiditate_Init(void);

/**
 * @brief Configurează pragurile pentru un senzor specific.
 */
void S_Umiditate_SetConfig(SUmiditateConfig_t* config, uint16_t th_vh, uint16_t th_h, uint16_t th_m, uint16_t th_l);

/**
 * @brief Returnează starea senzorului pe baza valorii ADC citite și a pragurilor setate.
 */
SUmiditateLevel_t S_Umiditate_GetLevel(uint8_t adc_channel, SUmiditateConfig_t* config);

#endif // S_UMIDITATE_H