#include "nivel_apa.h"
#include "drivers/gpio/gpio.h"
#include "drivers/adc/adc.h"
#include "bsp/uno.h"
#include "utils/delay.h"

// Definim pinii conform arhitecturii tale
#define PIN_ALIMENTARE_SENZOR UNO_D7
#define CANAL_ADC_APA         3 // Corespunde pinului A3

void NivelApa_Init(void) {
    // Setăm pinul D7 ca OUTPUT
    GPIO_Init(PIN_ALIMENTARE_SENZOR, GPIO_OUTPUT);
    
    // Îl menținem LOW (oprit) la pornirea sistemului pentru a proteja senzorul
    GPIO_Write(PIN_ALIMENTARE_SENZOR, GPIO_LOW);
}

uint16_t NivelApa_Citeste(void) {
    // 1. Pornim curentul către senzor
    GPIO_Write(PIN_ALIMENTARE_SENZOR, GPIO_HIGH);
    
    // 2. Așteptăm puțin să se stabilizeze tensiunea și curentul pe senzor
    Delay(10);
    
    // 3. Efectuăm citirea analogică de pe pinul A3
    uint16_t nivel = ADC_Read(CANAL_ADC_APA);
    
    // 4. Tăiem imediat alimentarea pentru a bloca procesul de electroliză (coroziune)
    GPIO_Write(PIN_ALIMENTARE_SENZOR, GPIO_LOW);
    
    return nivel;
}