#include "pompe.h"
#include "drivers/gpio/gpio.h"
#include "bsp/nano.h"

#define PIN_POMPA_1 D8 
#define PIN_POMPA_2 D9 
#define PIN_POMPA_3 D10 

void Pompe_Init(void) {
    GPIO_Init(PIN_POMPA_1, GPIO_OUTPUT);
    GPIO_Init(PIN_POMPA_2, GPIO_OUTPUT);
    GPIO_Init(PIN_POMPA_3, GPIO_OUTPUT);
    
    // Oprim totul preventiv. La modulele Active LOW, HIGH înseamnă OPRIT.
    Pompe_OprireUrgenta(); 
}

// Logică Active LOW: LOW = pornește, HIGH = oprește
void Pompa1_On(void)  { GPIO_Write(PIN_POMPA_1, GPIO_LOW); }  
void Pompa1_Off(void) { GPIO_Write(PIN_POMPA_1, GPIO_HIGH); } 

void Pompa2_On(void)  { GPIO_Write(PIN_POMPA_2, GPIO_LOW); }
void Pompa2_Off(void) { GPIO_Write(PIN_POMPA_2, GPIO_HIGH); }

void Pompa3_On(void)  { GPIO_Write(PIN_POMPA_3, GPIO_LOW); }
void Pompa3_Off(void) { GPIO_Write(PIN_POMPA_3, GPIO_HIGH); }

void Pompe_OprireUrgenta(void) {
    Pompa1_Off();
    Pompa2_Off();
    Pompa3_Off();
}