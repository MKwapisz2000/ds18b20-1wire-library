#ifndef DS18B20_1Wire_H
#define DS18B20_1Wire_H

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>


class DS18B20_1Wire {
  public:
  bool temp_alarm;
  
  DS18B20_1Wire(uint8_t TH_, uint8_t TL_);
  
  uint8_t* addressScanner();
  void checkTemperature();
  float getTemperature();
 

  private:
  float temperature;
  uint8_t TH;
  uint8_t TL;
  
  void reset();
  void presence();
  void writeByte(uint8_t byte_);
  void writeBit(uint8_t bit_);
  uint8_t readBit();
  uint8_t readByte();
  void wScratchpad();
  void rScratchpad();
  uint8_t countCRC(uint8_t *rows);
  void cScratchpad();
  void checkAlarm(uint8_t th, uint8_t tl, float temp);
  void DS18B20_init();

};


#endif
