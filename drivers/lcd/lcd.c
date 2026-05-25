#include "lcd.h"
#include "drivers/gpio/gpio.h"
#include "utils/delay.h"

/**
 * @brief Trimite un puls scurt pe pinul Enable (E).
 */
static void LCD_PulseEnable(void) {
    GPIO_Write(LCD_E_PIN, GPIO_HIGH);
    
    // Înlocuim Delay(1) ms cu câteva NOP-uri pentru un puls rapid și precis (>450ns)
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    
    GPIO_Write(LCD_E_PIN, GPIO_LOW);
    
    // Ecranul are nevoie de ~37 microsecunde să proceseze nibble-ul citit.
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

// ==========================================
// FUNCȚII PUBLICE (API) - Expuse în lcd.h
// ==========================================

void LCD_Command(uint8_t cmd) {
    GPIO_Write(LCD_RS_PIN, GPIO_LOW); // RS = LOW înseamnă COMANDĂ
    
    LCD_Write4Bits(cmd >> 4);
    LCD_Write4Bits(cmd & 0x0F);
    
    if (cmd == LCD_CMD_CLEAR_DISPLAY || cmd == LCD_CMD_RETURN_HOME) {
        Delay(3); // Aceste comenzi durează mai mult (~1.5ms), punem 3ms pentru siguranță
    } else {
        Delay(1); // Siguranță pentru restul comenzilor
    }
}

void LCD_Char(char data) {
    GPIO_Write(LCD_RS_PIN, GPIO_HIGH); // RS = HIGH înseamnă DATE (TEXT)
    
    // 1. Trimitem prima jumătate de literă (High Nibble)
    LCD_Write4Bits(data >> 4);
    
    // 2. Trimitem a doua jumătate de literă (Low Nibble)
    LCD_Write4Bits(data & 0x0F);
    
    // 3. SECRETUL PENTRU A SCĂPA DE GARBAGE DATA:
    // Oferim LCD-ului 2 milisecunde să lipească ambele jumătăți și să afișeze litera
    // înainte ca bucla while din LCD_String să ne trimită următoarea literă.
    Delay(2); 
}

void LCD_Init(void) {
    // 1. Configurăm toți pinii ca OUTPUT
    GPIO_Init(LCD_RS_PIN, GPIO_OUTPUT);
    GPIO_Init(LCD_E_PIN, GPIO_OUTPUT);
    GPIO_Init(LCD_D4_PIN, GPIO_OUTPUT);
    GPIO_Init(LCD_D5_PIN, GPIO_OUTPUT);
    GPIO_Init(LCD_D6_PIN, GPIO_OUTPUT);
    GPIO_Init(LCD_D7_PIN, GPIO_OUTPUT);
    
    // Asigurăm starea inițială LOW pentru control
    GPIO_Write(LCD_RS_PIN, GPIO_LOW);
    GPIO_Write(LCD_E_PIN, GPIO_LOW);
    
    // 2. Așteptăm ca LCD-ul să se alimenteze complet
    Delay(50); 
    
    // 3. Secvența strictă de resetare hardware
    LCD_Write4Bits(0x03);
    Delay(5);
    
    LCD_Write4Bits(0x03);
    Delay(1); 
    
    LCD_Write4Bits(0x03);
    Delay(1); 
    
    // 4. Setăm ecranul în modul 4-biți
    LCD_Write4Bits(0x02);
    Delay(1); 
    
    // 5. Acum putem folosi funcțiile standard de comenzi pe 8-biți (trimise în 2 pași)
    LCD_Command(LCD_CMD_4BIT_2LINES);
    LCD_Command(0x08); // Display off command
    LCD_Command(LCD_CMD_CLEAR_DISPLAY);
    LCD_Command(LCD_CMD_ENTRY_MODE);
    LCD_Command(LCD_CMD_DISPLAY_ON);  // Îl pornim la final
}

void LCD_String(const char *str) {
    // Parcurgem textul literă cu literă până dăm de caracterul null ('\0')
    while (*str) {
        LCD_Char(*str++);
    }
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t row_offsets[] = {0x00, 0x40}; 
    
    if (row > 1) row = 1; 
    
    LCD_Command(0x80 | (col + row_offsets[row]));
}