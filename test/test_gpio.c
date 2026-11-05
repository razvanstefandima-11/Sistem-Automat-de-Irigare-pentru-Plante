#include <stdio.h>
#include "framework/test_framework.h"

// Include Mocks first
#include "mocks/avr/io.h"

// Include Driver to test
// We include the .c file directly to have access to everything, 
// and because we are not linking against the driver library in this simple setup
#include "../../drivers/gpio/gpio.c"

void test_gpio_init_output(void) {
    // Test initializing PB5,PC5,PD5 as OUTPUT
    GPIO_Init(GPIO_PORTB, 5, GPIO_OUTPUT);
    GPIO_Init(GPIO_PORTC, 5, GPIO_OUTPUT);
    GPIO_Init(GPIO_PORTD, 5, GPIO_OUTPUT);
    
    // Check if DDRB bit 5 is set
    ASSERT_EQUAL((1 << 5), DDRB & (1 << 5));
    // Check if DDRC bit 5 is set
    ASSERT_EQUAL((1 << 5), DDRC & (1 << 5));
    // Check if DDRD bit 5 is set
    ASSERT_EQUAL((1 << 5), DDRD & (1 << 5));
}

void test_gpio_init_input(void) {
    // Test initializing PB5,PC5,PD5 as INPUT
    // Set it to output first to ensure it changes
    DDRB = (1 << 5);
    DDRC = (1 << 5);
    DDRD = (1 << 5);
    GPIO_Init(GPIO_PORTB, 5, GPIO_INPUT);
    GPIO_Init(GPIO_PORTC, 5, GPIO_INPUT);
    GPIO_Init(GPIO_PORTD, 5, GPIO_INPUT);
    
    // Check if DDRB bit 5 is cleared
    ASSERT_EQUAL(0, DDRB & (1 << 5));
    // Check if DDRC bit 5 is cleared
    ASSERT_EQUAL(0, DDRC & (1 << 5));
    // Check if DDRD bit 5 is cleared
    ASSERT_EQUAL(0, DDRD & (1 << 5));
}

void test_gpio_write_high(void) {
    // Test writing HIGH to PB5,PC5,PD5
    GPIO_Write(GPIO_PORTB, 5, GPIO_HIGH);
    GPIO_Write(GPIO_PORTC, 5, GPIO_HIGH);
    GPIO_Write(GPIO_PORTD, 5, GPIO_HIGH);

    // Check if PORTB bit 5 is set
    ASSERT_EQUAL((1 << 5), PORTB & (1 << 5));
    // Check if PORTC bit 5 is set
    ASSERT_EQUAL((1 << 5), PORTC & (1 << 5));
    // Check if PORTD bit 5 is set
    ASSERT_EQUAL((1 << 5), PORTD & (1 << 5));
}

void test_gpio_write_low(void) {
    PORTB = (1 << 5); // Set high initially
    PORTC = (1 << 5); // Set high initially
    PORTD = (1 << 5); // Set high initially

    
    GPIO_Write(GPIO_PORTB, 5, GPIO_LOW);
    GPIO_Write(GPIO_PORTC, 5, GPIO_LOW);
    GPIO_Write(GPIO_PORTD, 5, GPIO_LOW);
    // Check if PORTB bit 5 is cleared
    ASSERT_EQUAL(0, PORTB & (1 << 5));
    // Check if PORTC bit 5 is cleared
    ASSERT_EQUAL(0, PORTC & (1 << 5));
    // Check if PORTD bit 5 is cleared
    ASSERT_EQUAL(0, PORTD & (1 << 5));
}

void test_gpio_read(void) {
    // Simulate reading HIGH from PB3,PC3,PD3
    PINB = (1 << 3);
    PINC = (1 << 3);
    PIND = (1 << 3);

    // Read the state of pin 3 on each port
    gpio_state_t state_b = GPIO_Read(GPIO_PORTB, 3);
    gpio_state_t state_c = GPIO_Read(GPIO_PORTC, 3);
    gpio_state_t state_d = GPIO_Read(GPIO_PORTD, 3);
    
    // Check if the state is HIGH
    ASSERT_EQUAL(GPIO_HIGH, state_b);
    ASSERT_EQUAL(GPIO_HIGH, state_c);
    ASSERT_EQUAL(GPIO_HIGH, state_d);
    

    // Simulate reading LOW from PB3,PC3,PD3
    PINB = 0;
    PINC = 0;
    PIND = 0;
    // Read the state of pin 3 on each port again
    state_b = GPIO_Read(GPIO_PORTB, 3);
    state_c = GPIO_Read(GPIO_PORTC, 3);
    state_d = GPIO_Read(GPIO_PORTD, 3);
    // Check if the state is LOW
    ASSERT_EQUAL(GPIO_LOW, state_b);
    ASSERT_EQUAL(GPIO_LOW, state_c);
    ASSERT_EQUAL(GPIO_LOW, state_d);

    // Check reading from a PORT that doesn't exist
    gpio_state_t state_invalid = GPIO_Read(999, 3); // Invalid port
    ASSERT_EQUAL(GPIO_LOW, state_invalid); // Should return LOW for invalid port
}

void test_gpio_toggle(void) {
    // Simulate toggling PB4,PC4,PD4
    PORTB = 0; // Start LOW
    PORTC = 0; // Start LOW
    PORTD = 0; // Start LOW

    GPIO_Toggle(GPIO_PORTB, 4);
    GPIO_Toggle(GPIO_PORTC, 4);
    GPIO_Toggle(GPIO_PORTD, 4);

    // Check if PORTB bit 4 is set (toggled to HIGH)
    ASSERT_EQUAL((1 << 4), PORTB & (1 << 4));
    // Check if PORTC bit 4 is set (toggled to HIGH)
    ASSERT_EQUAL((1 << 4), PORTC & (1 << 4));
    // Check if PORTD bit 4 is set (toggled to HIGH)
    ASSERT_EQUAL((1 << 4), PORTD & (1 << 4));

    GPIO_Toggle(GPIO_PORTB, 4);
    GPIO_Toggle(GPIO_PORTC, 4);
    GPIO_Toggle(GPIO_PORTD, 4);

    // Check if PORTB bit 4 is cleared (toggled back to LOW)
    ASSERT_EQUAL(0, PORTB & (1 << 4));
    // Check if PORTC bit 4 is cleared (toggled back to LOW)
    ASSERT_EQUAL(0, PORTC & (1 << 4));
    // Check if PORTD bit 4 is cleared (toggled back to LOW)
    ASSERT_EQUAL(0, PORTD & (1 << 4));
}


int main(void) {
    printf("--- GPIO Driver Unit Tests ---\n");
    
    RUN_TEST(test_gpio_init_output);
    RUN_TEST(test_gpio_init_input);
    RUN_TEST(test_gpio_write_high);
    RUN_TEST(test_gpio_write_low);
    RUN_TEST(test_gpio_read);
    RUN_TEST(test_gpio_toggle);
    
    
    // Add more tests as needed
    
    return 0;
}