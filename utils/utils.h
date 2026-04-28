#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

// Bit manipulation macros
#define BIT(x) (1UL << (x))
#define SET_BIT(reg, bit) ((reg) |= BIT(bit))
#define CLEAR_BIT(reg, bit) ((reg) &= ~BIT(bit))
#define TOGGLE_BIT(reg, bit) ((reg) ^= BIT(bit))
#define CHECK_BIT(reg, bit) (!!((reg) & BIT(bit)))
#define WRITE_BIT(reg, bit, val) ((val) ? SET_BIT(reg, bit) : CLEAR_BIT(reg, bit))

#endif // UTILS_H