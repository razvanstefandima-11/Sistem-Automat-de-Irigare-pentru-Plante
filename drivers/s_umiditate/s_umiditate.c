#include "s_umiditate.h"
#include "drivers/adc/adc.h"

void S_Umiditate_Init(void) {
    // Hardware-ul ADC este inițializat global în main()
}

void S_Umiditate_SetConfig(SUmiditateConfig_t* config, uint16_t th_vh, uint16_t th_h, uint16_t th_m, uint16_t th_l) {
    config->very_high_threshold = th_vh;
    config->high_threshold = th_h;
    config->medium_threshold = th_m;
    config->low_threshold = th_l;
}

SUmiditateLevel_t S_Umiditate_GetLevel(uint8_t adc_channel, SUmiditateConfig_t* config) {
    // Citim valoarea brută a senzorului de pe canalul ADC dorit
    uint16_t pv = ADC_Read(adc_channel);
    
    // Trecem valoarea prin filtrul de praguri 
    // Atenție: la HW-390 valorile mici înseamnă umiditate mare!
    if (pv <= config->very_high_threshold) {
        return S_UMIDITATE_VERY_HIGH;
    } 
    else if (pv > config->very_high_threshold && pv <= config->high_threshold) {
        return S_UMIDITATE_HIGH;
    } 
    else if (pv > config->high_threshold && pv <= config->medium_threshold) {
        return S_UMIDITATE_MEDIUM;
    } 
    else if (pv > config->medium_threshold && pv <= config->low_threshold) {
        return S_UMIDITATE_LOW;
    } 
    else {
        return S_UMIDITATE_VERY_LOW;
    }
}