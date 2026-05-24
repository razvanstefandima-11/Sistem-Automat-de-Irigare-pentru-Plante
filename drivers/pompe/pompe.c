#include "pompe.h"
#include "drivers/gpio/gpio.h"

// Mapăm pinii de pe Nano conform tabelului tău inițial
// D8 = Port B, Pin 0
// D9 = Port B, Pin 1
// D10 = Port B, Pin 2
#define PIN_POMPA_1 0 
#define PIN_POMPA_2 1 
#define PIN_POMPA_3 2 

void Pompe_Init(void) {
    // Setăm pinii releelor ca ieșiri (OUTPUT)
    GPIO_Init(GPIO_PORTB, PIN_POMPA_1, GPIO_OUTPUT);
    GPIO_Init(GPIO_PORTB, PIN_POMPA_2, GPIO_OUTPUT);
    GPIO_Init(GPIO_PORTB, PIN_POMPA_3, GPIO_OUTPUT);
    
    // Oprim totul preventiv la pornirea plăcii
    Pompe_OprireUrgenta(); 
}

void Pompa1_On(void)  { GPIO_Write(GPIO_PORTB, PIN_POMPA_1, GPIO_HIGH); }
void Pompa1_Off(void) { GPIO_Write(GPIO_PORTB, PIN_POMPA_1, GPIO_LOW); }

void Pompa2_On(void)  { GPIO_Write(GPIO_PORTB, PIN_POMPA_2, GPIO_HIGH); }
void Pompa2_Off(void) { GPIO_Write(GPIO_PORTB, PIN_POMPA_2, GPIO_LOW); }

void Pompa3_On(void)  { GPIO_Write(GPIO_PORTB, PIN_POMPA_3, GPIO_HIGH); }
void Pompa3_Off(void) { GPIO_Write(GPIO_PORTB, PIN_POMPA_3, GPIO_LOW); }

void Pompe_OprireUrgenta(void) {
    Pompa1_Off();
    Pompa2_Off();
    Pompa3_Off();
}