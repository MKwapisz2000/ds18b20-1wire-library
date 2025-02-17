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

- DS18B20_1Wire.h

- DS18B20_1Wire.cpp

- Initialize the DS18B20 object in your program

    DS18B20_1Wire sensor(25, 15); // TH = 25°C, TL = 15°C

- Read temperature:

    sensor.checkTemperature();
    
    float temp = sensor.getTemperature();
    
