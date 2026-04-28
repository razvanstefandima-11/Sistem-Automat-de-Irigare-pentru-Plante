#include "drivers/gpio/gpio.h"
#include "drivers/timer/timer0.h"
#include "bsp/nano.h"

int main(void) {
    
    Timer0_Init();

    
    GPIO_Init(LED_BUILTIN, GPIO_OUTPUT);

    uint32_t last_time = 0;

    while (1) {
            
        if (Millis() - last_time >= 1000) {
            last_time = Millis();
            GPIO_Toggle(LED_BUILTIN);
        }
    }
}