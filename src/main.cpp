#include "Arduino.h"
#include "pins_arduino.h"

#define CLOCK_PIN 12
#define DATA_PIN  11

unsigned long tempmicros;

void setup() 
{
  Serial.begin( 115200 );
  pinMode( CLOCK_PIN, INPUT );
  pinMode( DATA_PIN, INPUT );
  Serial.print( "Ready:" );
}

float decode() {
  int sign = 1;
  long value = 0;

  for (int i = 0; i < 23; i++) 
  {
    //wait until clock returns to HIGH - the first bit is not needed
    while ( digitalRead( CLOCK_PIN ) == LOW ) {}
    
    //wait until clock returns to LOW
    while ( digitalRead( CLOCK_PIN) == HIGH ) {}
    
    if ( digitalRead( DATA_PIN ) == HIGH ) 
    {
      if (i < 20) 
      {
        value |= 1 << i;
      }
      if (i == 20) 
      {
        sign = -1;
      }
    }
  }
  return ( value * sign ) / 100.00;
}
 
void loop () {
  //wait until clock turns to HIGH
  while (digitalRead( CLOCK_PIN) == LOW ) {}

  tempmicros = micros();

  //wait for the end of the HIGH pulse
  while ( digitalRead( CLOCK_PIN ) == HIGH ) {}
  
  //if the HIGH pulse was longer than 500 micros we are at the start of a new bit sequence
  if ( ( micros() - tempmicros ) > 500 ) 
  { 
    float result = decode();

    //print measurments
    Serial.print( "Lenght: " );
    Serial.println( result, 2 );
    delay( 1000 );
  }
}