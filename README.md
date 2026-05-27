# Sistem-Automat-de-Irigare-pentru-Plante

Acest repository este dedicat dezvoltării unui sistem inteligent de irigare automatizată, conceput pentru gestionarea eficientă a udării plantelor la scară mică.

Proiectul utilizează o arhitectură de tip Master-Slave bazată pe două plăci Arduino (Arduino Uno; Arduino Nano), care colaborează pentru a monitoriza și controla în timp real umiditatea solului.
Scopul principal al sistemului este menținerea unei umidități optime a solului pentru a susține sănătatea plantelor, reducând în același timp intervenția omului.
Prin folosirea senzorilor de umiditate, a unui sistem de distribuție a apei și a unui display pentru afișarea datelor relevante, utilizatorul beneficiază de o soluție eficientă și ușor de utilizat.

Repository-ul conține codul sursă, documentația și resursele necesare pentru înțelegerea, reproducerea și extinderea acestui sistem.

## Specificații

 - **Fără Librării Arduino**: Manipulare directă a regiștrilor (register manipulation) pentru control total asupra hardware-ului și eficiență maximă a resurselor.
 - **BSP (Board Support Package)**: Mapare hardware specifică pentru Arduino Uno (Master) și Arduino Nano (Slave).
 - **Drivere**: Arhitectură modulară, documentată și reutilizabilă pentru perifericele ATmega328P:
   
    **GPIO**: Inițializare, Write (Pompe, LED, Buzzer), Read (Senzor Ultrasonic), Toggle (LED Alarma). 
    **Interrupts**: Gestionarea ecoului de la HC-SR04 și a comunicației I2C prin vectori de întrerupere. 
    **Timer**: System Tick de 1ms folosind Timer0 în mod CTC (pentru cronometrarea alarmei de 60s). 
    **EEPROM**: Salvarea pragurilor de umiditate (Read/Write) pentru a nu pierde setările la pană de curent. 
    **ADC**: Conversie pe 10-biți în mod blocking pentru citirea celor 3 senzori de umiditate capacitivi. 
    **I2C (TWI)**: Driver pentru comunicarea Master (Uno) - Slave (Nano) și controlul display-ului LCD 1602. 
    **UART (USART)**: Comunicație serială pentru debugging, monitorizare în timp real și afișarea valorilor senzorilor în Serial Monitor. 
    **LCD 1602**: Driver pentru afișarea nivelului de umiditate, stării pompelor, mesajelor de avertizare și meniului de configurare. 
    **Buzzer**: Generarea alertelor sonore pentru nivel scăzut al apei și stări critice ale sistemului. 
    **Pump Control**: Driver pentru controlul pompelor prin relee, incluzând protecție software împotriva activării simultane necontrolate. 
    **Button/Input Driver**: Debouncing software și detectarea apăsărilor pentru navigarea prin meniul de configurare.  
    **Scheduler / Task Manager**: Execuție periodică non-blocking a taskurilor folosind System Tick pentru actualizarea senzorilor și logicii de control. 
    **Alarm Manager**: Gestionarea condițiilor de alarmă (rezervor gol, umiditate critică) și sincronizarea semnalizării LED/Buzzer. 

 - **Sistem de Build Robust**: Utilizarea Makefile pentru automatizarea proceselor de compilare, link-editare și scriere (flash) via avrdude.
 - **Unit Testing pe Host**: Rularea testelor unitare direct pe calculator (fără hardware) prin tehnici de register mocking.
 - **Acoperire Cod (Code Coverage)**: Generarea rapoartelor HTML (lcov) pentru verificarea integrității testelor.

## Roadmap

- [x] GPIO driver
- [x] ADC driver
- [x] EEPROM driver
- [x] Interrupt driver
- [x] Timer driver
- [x] PWM driver
- [x] I2C driver
- [x] Lcd driver
- [x] Pompe driver
- [x] PWM driver
- [x] Senzor Umiditate driver
- [x] LED driver


## Structura Proiect

```
├── bsp/            # Board definitions (uno.h, nano.h)
├── drivers/        # Hardware Abstraction Layer
│   ├── adc/
│    ├── button/
│   ├── buzzer/
│   ├── eeprom/
│   ├── gpio/
│   ├── i2c/
│   ├── interrupt/
│   ├── lcd/
│   ├── led/
│   ├── nivel apa/
│   ├── pompe/
│   ├── pwm
│   ├── s_umiditate
│   ├── timer/
│   └── usart/
├── src/            # Application source code (main.c)
├── test/           # Unit tests & Mocks
│   ├── mocks/      # Mock AVR registers for host testing
│   ├── framework/  # Minimal test runner
│   └── test_*.c    # Unit test files
├── utils/          # Helper macros (BIT manipulations)
└── Makefile        # Build configuration
```

## Build & Flash

### Prerequisites

- `avr-gcc` toolchain
- `avrdude`
- `make`

### Comenzi
| Comanda |  Descriere  |
|---------|-------------|
| `make all`   | Compile the project. |
| `make flash` | Flash the firmware to the connected board. |
| `make clean` | Remove build artifacts. |






