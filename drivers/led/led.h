#ifndef LED_H
#define LED_H

#include <stdint.h>

/**
 * @brief Inițializează pinul hardware pentru LED-ul de alarmă ca OUTPUT.
 */
void LED_Init(void);

/**
 * @brief Aprinde LED-ul de alarmă.
 */
void LED_On(void);

/**
 * @brief Stinge LED-ul de alarmă.
 */
void LED_Off(void);

/**
 * @brief Schimbă starea LED-ului (dacă e aprins se stinge, dacă e stins se aprinde).
 */
void LED_Toggle(void);

#endif // LED_H