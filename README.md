# ds18b20-1wire-library

**Project Description - English**

The DS18B20_1Wire library is designed to interface with the DS18B20 temperature sensor using the 1-Wire protocol. It allows for temperature reading, alarm threshold detection, and device scanning on the 1-Wire bus.

Features

- Initialization of the DS18B20 sensor

- Reading temperature in Celsius

- Temperature alarm handling – configurable upper (TH) and lower (TL) thresholds

- ROM Address Scanning – detecting sensor addresses

- 1-Wire Communication – handling reset, byte reading/writing, and sensor commands

- CRC Check – verifying sensor data integrity

Hardware Requirements

- AVR Microcontroller (e.g., ATmega328P - Arduino Nano/Uno)

- DS18B20 Sensor

- 4.7kΩ Pull-up Resistor (required for 1-Wire communication)

Installation

- Add the following files to your project:

        DS18B20_1Wire.h
        
        DS18B20_1Wire.cpp

- Initialize the DS18B20 object in your program

        DS18B20_1Wire sensor(25, 15); // TH = 25°C, TL = 15°C

- Read temperature:

        sensor.checkTemperature();
            
        float temp = sensor.getTemperature();

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

**Opis projektu - Polski**

Biblioteka DS18B20_1Wire została stworzona w celu obsługi czujnika temperatury DS18B20 przy wykorzystaniu magistrali 1-Wire. Umożliwia odczyt temperatury, wykrywanie alarmów temperaturowych oraz skanowanie adresów urządzeń podłączonych do magistrali.

Funkcjonalność

- Inicjalizacja czujnika DS18B20

- Odczyt temperatury w stopniach Celsjusza

- Obsługa alarmu temperatury – ustawienie górnego (TH) i dolnego (TL) progu temperatury

- Skanowanie adresu ROM – wykrywanie adresu czujnika DS18B20

- Obsługa magistrali 1-Wire – implementacja resetowania, pisania i odczytu bajtów oraz komunikacji z czujnikiem

- Obsługa CRC – sprawdzanie poprawności danych z czujnika

Wymagania sprzętowe

- Mikrokontroler AVR (np. ATmega328P - Arduino Nano/Uno)

- Czujnik DS18B20

- Rezystor pull-up 4.7kΩ (podciągający magistralę 1-Wire do zasilania)

Instalacja

- Dodaj pliki biblioteki do projektu:

        DS18B20_1Wire.h
        
        DS18B20_1Wire.cpp

- Zainicjalizuj obiekt czujnika DS18B20 w kodzie:

        DS18B20_1Wire sensor(25, 15); // TH = 25°C, TL = 15°C

- Odczytaj temperaturę:
  
        sensor.checkTemperature();
        
        float temp = sensor.getTemperature();
