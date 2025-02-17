#include "DS18B20_1Wire.h"

//ROM COMMANDS
#define READ_ROM          0x33
#define MATCH_ROM         0x55
#define SKIP_ROM          0xCC
#define ALARM_SEARCH      0xEC
#define SEARCH_ROM        0xF0

//FUNCTION COMMANDS
#define CONVERT           0x44
#define W_SCRATCHPAD      0x4E
#define R_SCRATCHPAD      0xBE
#define COPY_SCRATCHPAD   0x48
#define RECALL            0xB8

//Configuration
#define CONF              0b01111111      //rozdzielczość 12-bitowa
#define PIN               PD4             //pin na którym podłączone jest DS18B20
 
DS18B20_1Wire::DS18B20_1Wire(uint8_t TH_, uint8_t TL_)
{
  DDRD &= ~(1<<PIN);
  PORTD |= (1<<PIN);
  
  TH = TH_;
  TL = TL_;
  temp_alarm=0;

  DS18B20_init();
}

void DS18B20_1Wire::DS18B20_init()
{
  reset();
  presence();
  
  writeByte(SKIP_ROM);
  writeByte(W_SCRATCHPAD);
  wScratchpad();
  
  reset();
  presence();
  
  writeByte(SKIP_ROM);
  writeByte(R_SCRATCHPAD);
  rScratchpad();

  reset();
  presence();
  
  writeByte(SKIP_ROM);
  writeByte(COPY_SCRATCHPAD);
  cScratchpad();
}

void DS18B20_1Wire::reset()
{
  //Master jako transmiter
  DDRD |= (1<<PIN);
  PORTD &= ~(1<<PIN);
  _delay_us(480);

  //Zwolnienie magistrali 
  DDRD &= ~(1<<PIN);
  _delay_us(60);
}

void DS18B20_1Wire::presence()
{
  //Czas na odpowiedz czujnika
  _delay_us(60);

  //Sprawdzenie czy czujnik odpowiedział (PIN jest w stanie niskim) 
  if(!(PIND & (1<<PIN)))
  {
    _delay_us(240); 
  }
}

void DS18B20_1Wire::writeByte(uint8_t byte_)
{
  for(int i=0; i<8; i++)
  {
    writeBit((byte_>>i) & 0b00000001);
  }
}

void DS18B20_1Wire::writeBit(uint8_t bit_)
{
  //Master jako transmiter
  DDRD |= (1<<PIN);

  //Magistrala w dół
  PORTD &= ~(1<<PIN);
  
  if(bit_== 1)
  {
    _delay_us(2);

    //Zwolnienie magistrali
    DDRD &= ~(1<<PIN);

    _delay_us(60);
  }
  else
  {
    _delay_us(60);
    
    //Zwolnienie magistrali
    DDRD &= ~(1<<PIN);
    
    _delay_us(2);
  } 
}

uint8_t DS18B20_1Wire::readBit()
{
  uint8_t bit_;
  
  DDRD |= (1<<PIN);
  PORTD &= ~(1<<PIN);
  _delay_us(2);
    
  //Zwolnienie magistrali
  DDRD &= ~(1<<PIN);
  _delay_us(15);
    
  if(PIND & (1<<PIN))
  {
    bit_ = 1;
  }
  else
  {
    bit_ = 0;
  }
  
  _delay_us(45);

  return bit_;
}

uint8_t DS18B20_1Wire::readByte()
{
  uint8_t byte_ = 0b00000000;
  uint8_t bit_;
  
  for(int i=0; i<8; i++)
  {
    bit_ = readBit();

    if(bit_ == 1)
    {
      byte_ = byte_ | (1<<i);
    }

     _delay_us(45);
  }
  
  return byte_;
}

void DS18B20_1Wire::wScratchpad()
{
  writeByte(TH);
  writeByte(TL);
  writeByte(CONF);
}

void DS18B20_1Wire::rScratchpad()
{
  uint8_t ROWS[9];

  for(int i=0; i<9; i++)
  {
    ROWS[i] = readByte();
  }

  uint8_t crc = countCRC(ROWS);

  if(crc != ROWS[8])
  {
    //Błędny crc, odczyt powtórzony
    DDRB |= (1<<PB5);
    PORTB |= (1<<PB5);  
    
    writeByte(R_SCRATCHPAD);
    rScratchpad();
  }
  else
  {
    return;
  }
}

uint8_t DS18B20_1Wire::countCRC(uint8_t *rows)
{
  uint8_t crc = 0b00000000;
  
  for(int i=0; i<8; i++)
  {
    crc ^= rows[i];

    for(int j=0; j<8; j++)
    {
      if((crc&0b00000001)==1)
      {
        crc = (crc >> 1) ^ 0x8C;
      }
      else
      {
        crc >>= 1;
      }
    }
  }

  return crc;
}

void DS18B20_1Wire::cScratchpad()
{
  DDRD |= (1<<PIN);
  PORTD |= (1<<PIN);
  _delay_us(10);

  DDRD &= ~(1<<PIN);
  _delay_ms(10);
}

uint8_t* DS18B20_1Wire::addressScanner()
{
  static uint8_t address_array[8];
  char string[50];

  reset();
  presence();
  
  writeByte(READ_ROM);
    
  for(int j=0; j<8; j++)
  {
    address_array[j]=readByte();
  }

  return address_array;
}

void DS18B20_1Wire::checkTemperature()
{
    reset();
    presence();

    writeByte(SKIP_ROM);
    writeByte(CONVERT);
    //czekaj dopóki konwersja trwa
    while(!(PIND & (1<<PIN))) {}

    reset();
    presence();
    writeByte(SKIP_ROM);
    writeByte(R_SCRATCHPAD);

    uint8_t ROWS[4];
    for(int i = 0; i <4; i++)
    {
       ROWS[i] = readByte();
    }

    //odczyt temperatury
    uint16_t read_temp = (ROWS[1] << 8) | ROWS[0]; 
    float temp = read_temp / 16.0; 

    uint8_t th = ROWS[2];
    uint8_t tl = ROWS[3];

    temperature = temp;
    
    checkAlarm(th, tl, temp);
}

void DS18B20_1Wire::checkAlarm(uint8_t th, uint8_t tl, float temp)
{
  char str_temp1[20];
  char str_temp2[20];
  
  reset();
  presence();
  writeByte(ALARM_SEARCH);

  _delay_us(60);

  uint8_t address = readByte();
  if (address != 0xFF){
    
    if(temp>=th){
      DDRB |= (1<<PB5);
      PORTB |= (1<<PB5);
      temp_alarm=1;
    }
    else if(temp<=tl){
      DDRB |= (1<<PB5);
      PORTB |= (1<<PB5);
      temp_alarm=1;
    }
  }
  
  else{
      PORTB &= ~(1<<PB5);
  }
}

float DS18B20_1Wire::getTemperature()
{
  return temperature;
}
