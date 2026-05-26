#include "lcd.h"
#include "drivers/gpio/gpio.h"
#include "utils/delay.h"

/**
 * @brief Trimite un puls scurt pe pinul Enable (E).
 */
static void LCD_PulseEnable(void) {
    GPIO_Write(LCD_E_PIN, GPIO_HIGH);
    
    // Puls rapid și precis (>450ns)
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    
    GPIO_Write(LCD_E_PIN, GPIO_LOW);
    Delay(1); 
}

/**
 * @brief Trimite 4 biți (un nibble) către pinii de date ai LCD-ului.
 */
static void LCD_Write4Bits(uint8_t nibble) {
    GPIO_Write(LCD_D4_PIN, (nibble & 0x01) ? GPIO_HIGH : GPIO_LOW);
    GPIO_Write(LCD_D5_PIN, (nibble & 0x02) ? GPIO_HIGH : GPIO_LOW);
    GPIO_Write(LCD_D6_PIN, (nibble & 0x04) ? GPIO_HIGH : GPIO_LOW);
    GPIO_Write(LCD_D7_PIN, (nibble & 0x08) ? GPIO_HIGH : GPIO_LOW);

    LCD_PulseEnable();
}

void LCD_Command(uint8_t cmd) {
    GPIO_Write(LCD_RS_PIN, GPIO_LOW); 
    
    LCD_Write4Bits(cmd >> 4);
    LCD_Write4Bits(cmd & 0x0F);
    
    if (cmd == LCD_CMD_CLEAR_DISPLAY || cmd == LCD_CMD_RETURN_HOME) {
        Delay(3); 
    } else {
        Delay(1); 
    }
}

void LCD_Char(char data) {
    GPIO_Write(LCD_RS_PIN, GPIO_HIGH); 
    
    LCD_Write4Bits(data >> 4);
    LCD_Write4Bits(data & 0x0F);
    
    Delay(2); 
}

void LCD_Init(void) {
    GPIO_Init(LCD_RS_PIN, GPIO_OUTPUT);
    GPIO_Init(LCD_E_PIN, GPIO_OUTPUT);
    GPIO_Init(LCD_D4_PIN, GPIO_OUTPUT);
    GPIO_Init(LCD_D5_PIN, GPIO_OUTPUT);
    GPIO_Init(LCD_D6_PIN, GPIO_OUTPUT);
    GPIO_Init(LCD_D7_PIN, GPIO_OUTPUT);
    
    GPIO_Write(LCD_RS_PIN, GPIO_LOW);
    GPIO_Write(LCD_E_PIN, GPIO_LOW);
    
    Delay(50); 
    
    LCD_Write4Bits(0x03);
    Delay(5);
    
    LCD_Write4Bits(0x03);
    Delay(1); 
    
    LCD_Write4Bits(0x03);
    Delay(1); 
    
    LCD_Write4Bits(0x02);
    Delay(1); 
    
    LCD_Command(LCD_CMD_4BIT_2LINES);
    LCD_Command(0x08); 
    LCD_Command(LCD_CMD_CLEAR_DISPLAY);
    LCD_Command(LCD_CMD_ENTRY_MODE);
    LCD_Command(LCD_CMD_DISPLAY_ON); 
}

void LCD_String(const char *str) {
    while (*str) {
        LCD_Char(*str++);
    }
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t row_offsets[] = {0x00, 0x40}; 
    
    if (row > 1) row = 1; 
    
    LCD_Command(0x80 | (col + row_offsets[row]));
}