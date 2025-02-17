#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "DS18B20_1Wire.h"

DS18B20_1Wire ds18b20_1 = DS18B20_1Wire(0x18,0x0F);



//........................................................ USART ................................................................
void USART_init()
{
  UBRR0H = (unsigned char)(103>>8);
  UBRR0L = (unsigned char)103;
    
  //Adres we/wy rejestrów danych nadawczych USART i rejestry odbierania danych USART
  //UDR0

  //By bufor transmisji mógłbyć zapisany
  UCSR0A |= (1<<UDRE0);

  //Włączenie odbiornika
  UCSR0B |= (1<<RXEN0);

  //Włączenie nadajnika
  UCSR0B |= (1<<TXEN0);

  //Liczba bitów danych w ramce
  UCSR0C |= (1<<UCSZ00);
  UCSR0C |= (1<<UCSZ01);
}


void USART_Transmit( unsigned char data )
 {
 /* Wait for empty transmit buffer */
 while ( !( UCSR0A & (1<<UDRE0)) )
 ;
 /* Put data into buffer, sends the data */
 UDR0 = data;
 }

unsigned char USART_Receive()
 {
 /* Wait for data to be received */
 while ( !(UCSR0A & (1<<RXC0)) )
 ;
 /* Get and return received data from buffer */
 return UDR0;
 }

void USART_String(const char *array)
{
  int i=0;
  while(array[i]!='\0')
  {
    USART_Transmit(array[i]);
    i++;
  }
}



//.......................................................... MAIN .................................................................

int main()
{
  char string1[50];
  char string2[50];
  USART_init();
 

  while(1)
  {
    ds18b20_1.checkTemperature();
    dtostrf(ds18b20_1.getTemperature(), 5,1, string1);
    sprintf(string2, "\nPomiar temperatury: %sC\n", string1);
    USART_String(string2);

    _delay_ms(1000);
  }
  return 0;
}
