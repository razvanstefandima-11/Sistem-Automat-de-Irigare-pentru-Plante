#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include "bsp/uno.h" 

#define LCD_RS_PIN UNO_D12
#define LCD_E_PIN  UNO_D11

#define LCD_D4_PIN UNO_D5
#define LCD_D5_PIN UNO_D4
#define LCD_D6_PIN UNO_D3
#define LCD_D7_PIN UNO_D2

#define LCD_CMD_CLEAR_DISPLAY   0x01
#define LCD_CMD_RETURN_HOME     0x02
#define LCD_CMD_ENTRY_MODE      0x06
#define LCD_CMD_DISPLAY_ON      0x0C
#define LCD_CMD_4BIT_2LINES     0x28

void LCD_Init(void);
void LCD_Command(uint8_t cmd);
void LCD_Char(char data);
void LCD_String(const char *str);
void LCD_SetCursor(uint8_t row, uint8_t col);

#endif // LCD_H