#include "pompe.h"
#include "drivers/gpio/gpio.h"

#define PIN_POMPA_1 0 
#define PIN_POMPA_2 1 
#define PIN_POMPA_3 2 

void Pompe_Init(void) {
    GPIO_Init(GPIO_PORTB, PIN_POMPA_1, GPIO_OUTPUT);
    GPIO_Init(GPIO_PORTB, PIN_POMPA_2, GPIO_OUTPUT);
    GPIO_Init(GPIO_PORTB, PIN_POMPA_3, GPIO_OUTPUT);
    
    // Oprim totul preventiv. La modulele Active LOW, HIGH înseamnă OPRIT!
    Pompe_OprireUrgenta(); 
}

// INVERSĂM LOGICA AICI:
void Pompa1_On(void)  { GPIO_Write(GPIO_PORTB, PIN_POMPA_1, GPIO_LOW); }  // LOW = PORNEȘTE
void Pompa1_Off(void) { GPIO_Write(GPIO_PORTB, PIN_POMPA_1, GPIO_HIGH); } // HIGH = OPREȘTE

void Pompa2_On(void)  { GPIO_Write(GPIO_PORTB, PIN_POMPA_2, GPIO_LOW); }
void Pompa2_Off(void) { GPIO_Write(GPIO_PORTB, PIN_POMPA_2, GPIO_HIGH); }

void Pompa3_On(void)  { GPIO_Write(GPIO_PORTB, PIN_POMPA_3, GPIO_LOW); }
void Pompa3_Off(void) { GPIO_Write(GPIO_PORTB, PIN_POMPA_3, GPIO_HIGH); }

void Pompe_OprireUrgenta(void) {
    Pompa1_Off();
    Pompa2_Off();
    Pompa3_Off();
}