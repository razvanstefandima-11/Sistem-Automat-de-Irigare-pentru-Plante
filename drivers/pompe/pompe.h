#ifndef POMPE_H
#define POMPE_H

#include <stdint.h>

// Inițializare pini pentru relee
void Pompe_Init(void);

// Control Pompă 1 (Planta 1)
void Pompa1_On(void);
void Pompa1_Off(void);

// Control Pompă 2 (Planta 2)
void Pompa2_On(void);
void Pompa2_Off(void);

// Control Pompă 3 (Planta 3)
void Pompa3_On(void);
void Pompa3_Off(void);

// Failsafe: Oprește absolut toate pompele (în caz că rezervorul e gol)
void Pompe_OprireUrgenta(void);

#endif // POMPE_H