#include "nivel_apa.h"
#include "drivers/adc/adc.h" 

void NivelApa_Init(void) {
    // În modul Bare Metal pentru ATmega328P, pinii portului C (A0-A5) 
    // sunt setați implicit ca intrări la resetare.
    // Inițializarea registrelor ADC (ADCSRA, ADMUX) este gestionată de ADC_Init() în main.c.
}

uint8_t NivelApa_Read(void) {
    // Citim valoarea digitală convertită de pe canalul analogic 3 (Pinul A3)
    uint16_t nivel_brut = ADC_Read(3); 
    
    // PRAG DE CALIBRARE COMPENSAT (Histerezis software pentru zgomotul de masă)
    // - Senzor complet uscat + pompe pornite (Ground Bounce) = ~150
    // - Senzor în apă = ~500 - 600
    // Alegem un prag de siguranță la 350 pentru a evita citirile false.
    if (nivel_brut > 350) {
        return 1; // APA PREZENTĂ (Sistemul poate iriga în siguranță)
    } else {
        return 0; // APA GOL (Rezervorul trebuie umplut, pompele se opresc)
    }
}