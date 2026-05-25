#include "delay.h"

// Am redus la 300 pentru a compensa timpul pierdut de procesor cu bucla 'for'
// Crește această valoare pentru a-i da timp LCD-ului să "respire"
#define ITERATIONS_PER_MS 1500 

void Delay(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        for (volatile uint16_t j = 0; j < ITERATIONS_PER_MS; j++) {
            asm volatile("nop");
        }
    }
}