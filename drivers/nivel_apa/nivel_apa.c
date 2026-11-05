#include "nivel_apa.h"
#include "drivers/adc/adc.h"
#include "drivers/gpio/gpio.h"
#include "bsp/uno.h"
#include "utils/delay.h" // <-- ÎNAPOI LA DELAY-UL TĂU

#define PIN_ALIMENTARE_APA UNO_D7
#define CANAL_ADC_APA      3
#define PRAG_APA_PREZENTA  350

void NivelApa_Init(void) {
    // Configurăm pinul de alimentare ca OUTPUT și îl oprim preventiv
    GPIO_Init(PIN_ALIMENTARE_APA, GPIO_OUTPUT);
    GPIO_Write(PIN_ALIMENTARE_APA, GPIO_LOW);
}

uint8_t NivelApa_Read(void) {
    // 1. Pornim alimentarea senzorului
    GPIO_Write(PIN_ALIMENTARE_APA, GPIO_HIGH);
    
    // 2. Așteptăm 10ms folosind funcția TA de delay
    Delay(10);
    
    // 3. Citim valoarea brută
    uint16_t nivel_brut = ADC_Read(CANAL_ADC_APA); 
    
    // 4. Oprim alimentarea imediat
    GPIO_Write(PIN_ALIMENTARE_APA, GPIO_LOW);
    
    // 5. Evaluăm nivelul apei
    if (nivel_brut > PRAG_APA_PREZENTA) {
        return 1; 
    }
    
    return 0; 
}