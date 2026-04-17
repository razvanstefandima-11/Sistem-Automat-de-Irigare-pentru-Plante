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
    - **GPIO**: Inițializare, Write (Pompe, LED, Buzzer), Read (Senzor Ultrasonic), Toggle (LED Alarma).
    - **Interrupts**: Gestionarea ecoului de la HC-SR04 și a comunicației I2C prin vectori de întrerupere.
    - **Timer**: System Tick de 1ms folosind Timer0 în mod CTC (pentru cronometrarea alarmei de 60s).
    - **EEPROM**: Salvarea pragurilor de umiditate (Read/Write) pentru a nu pierde setările la pană de curent.
    - **ADC**: Conversie pe 10-biți în mod blocking pentru citirea celor 3 senzori de umiditate capacitivi.
    - **I2C (TWI)**: Driver pentru comunicarea Master (Uno) - Slave (Nano) și controlul display-ului LCD 1602.
 - **Sistem de Build Robust**: Utilizarea Makefile pentru automatizarea proceselor de compilare, link-editare și scriere (flash) via avrdude.
 - **Unit Testing pe Host**: Rularea testelor unitare direct pe calculator (fără hardware) prin tehnici de register mocking.
 - **Acoperire Cod (Code Coverage)**: Generarea rapoartelor HTML (lcov) pentru verificarea integrității testelor.

## Roadmap

-

## Structura Proiect

```
├── bsp/            
├── drivers/       
│   ├── adc/
│   ├── eeprom/
│   ├── gpio/
│   ├── interrupt/
│   └── timer/
├── src/            
├── test/          
│   ├── mocks/      
│   ├── framework/  
│   └── test_*.c    
├── utils/         
└── Makefile        
```

## Build & Flash

### Prerequisites

- `avr-gcc` toolchain
- `avrdude`
- `make`

### Comenzi
| Comanda |  Descriere  |
|---------|-------------|
| `make all`   | Compile the project for Arduino Nano. |
| `make flash` | Flash the firmware to the connected board. |
| `make clean` | Remove build artifacts. |






