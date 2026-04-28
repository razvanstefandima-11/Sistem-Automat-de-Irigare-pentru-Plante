#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include "bsp/uno.h" // Aducem definițiile pinilor (UNO_D12, etc.)

/**
 * @brief Configurarea Hardware (Harta Pinilor)
 * Mapăm pinii LCD la pinii definiți în uno.h
 */
#define LCD_RS_PIN UNO_D12
#define LCD_E_PIN  UNO_D11

#define LCD_D4_PIN UNO_D5
#define LCD_D5_PIN UNO_D4
#define LCD_D6_PIN UNO_D3
#define LCD_D7_PIN UNO_D2

/**
 * @brief Comenzi standard pentru controller-ul HD44780
 */
#define LCD_CMD_CLEAR_DISPLAY   0x01
#define LCD_CMD_RETURN_HOME     0x02
#define LCD_CMD_ENTRY_MODE      0x06
#define LCD_CMD_DISPLAY_ON      0x0C
#define LCD_CMD_4BIT_2LINES     0x28

/**
 * @brief Initializeaza afisajul LCD in modul 4-biti.
 */
void LCD_Init(void);

/**
 * @brief Trimite o comanda de control catre LCD.
 */
void LCD_Command(uint8_t cmd);

/**
 * @brief Trimite un singur caracter pentru a fi afisat.
 */
void LCD_Char(char data);

/**
 * @brief Afiseaza un sir de caractere (string) pe ecran.
 */
void LCD_String(const char *str);

/**
 * @brief Muta cursorul la pozitia specificata.
 */
void LCD_SetCursor(uint8_t row, uint8_t col);

#endif // LCD_H