#include "lcd.h"
#include "drivers/gpio/gpio.h"
#include "util/delay.h"

/**
 * @brief Trimite un puls scurt pe pinul Enable (E).
 * Acest puls îi spune ecranului să citească datele de pe pinii D4-D7.
 */
static void LCD_PulseEnable(void) {
    GPIO_Write(LCD_E_PIN, GPIO_HIGH);
    _delay_us(1); // Ecranul are nevoie de minim 450ns
    GPIO_Write(LCD_E_PIN, GPIO_LOW);
    _delay_us(100); // Așteptăm ca LCD-ul să proceseze datele
}

/**
 * @brief Trimite 4 biți (un nibble) către pinii de date ai LCD-ului.
 * @param nibble Cei 4 biți de date plasați în partea de jos a octetului (biții 0-3).
 */
static void LCD_Write4Bits(uint8_t nibble) {
    // Extragem fiecare bit din nibble și îl punem pe pinul corespunzător
    GPIO_Write(LCD_D4_PIN, (nibble & 0x01) ? GPIO_HIGH : GPIO_LOW);
    GPIO_Write(LCD_D5_PIN, (nibble & 0x02) ? GPIO_HIGH : GPIO_LOW);
    GPIO_Write(LCD_D6_PIN, (nibble & 0x04) ? GPIO_HIGH : GPIO_LOW);
    GPIO_Write(LCD_D7_PIN, (nibble & 0x08) ? GPIO_HIGH : GPIO_LOW);

    
    LCD_PulseEnable();
}

// FUNCȚII PUBLICE (API) - Expuse în lcd.h

void LCD_Command(uint8_t cmd) {
    GPIO_Write(LCD_RS_PIN, GPIO_LOW); // RS = LOW înseamnă COMANDĂ
    
    // Trimitem întâi cei 4 biți superiori (High Nibble)
    LCD_Write4Bits(cmd >> 4);
    // Trimitem apoi cei 4 biți inferiori (Low Nibble)
    LCD_Write4Bits(cmd & 0x0F);
    
    if (cmd == LCD_CMD_CLEAR_DISPLAY || cmd == LCD_CMD_RETURN_HOME) {
        _delay_ms(2); // Aceste comenzi durează mai mult
    }
}

void LCD_Char(char data) {
    GPIO_Write(LCD_RS_PIN, GPIO_HIGH); // RS = HIGH înseamnă DATE (TEXT)
    
    // Trimitem întâi cei 4 biți superiori
    LCD_Write4Bits(data >> 4);
    // Trimitem apoi cei 4 biți inferiori
    LCD_Write4Bits(data & 0x0F);
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
    
    // 2. Așteptăm ca LCD-ul să se alimenteze complet (>15ms)
    _delay_ms(20);
    
    // 3. Secvența strictă de resetare hardware cerută de HD44780
    LCD_Write4Bits(0x03);
    _delay_ms(5);
    LCD_Write4Bits(0x03);
    _delay_us(150);
    LCD_Write4Bits(0x03);
    
    // 4. Setăm ecranul în modul 4-biți
    LCD_Write4Bits(0x02);
    
    // 5. Acum putem folosi funcțiile standard de comenzi
    LCD_Command(LCD_CMD_4BIT_2LINES);
    LCD_Command(LCD_CMD_DISPLAY_ON);
    LCD_Command(LCD_CMD_CLEAR_DISPLAY);
    LCD_Command(LCD_CMD_ENTRY_MODE);
}

void LCD_String(const char *str) {
    // Parcurgem șirul de caractere până la caracterul null ('\0')
    while (*str) {
        LCD_Char(*str++);
    }
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t row_offsets[] = {0x00, 0x40}; // Adresele memoriei pentru Rândul 1 și Rândul 2
    
    if (row > 1) row = 1; // Protecție: avem doar 2 rânduri (0 și 1)
    
    // Adresa finală se obține adunând 0x80 (comanda de setare adresă) + offset rând + coloană
    LCD_Command(0x80 | (col + row_offsets[row]));
}