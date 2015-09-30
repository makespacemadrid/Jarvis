#include <Adafruit_NeoPixel.h>
#include "ws2812led.h"
#include "ssr.h"


int relayPin               = -1;
int currentMeterPin        = -1;
int relayTemperatureSensor = -1;
int fanPin                 = -1;
int maxAmps                = 15;
bool dimmable              = false;

int ledStripPin            = -1;
int ledNumber              =  1;


magneticCurrentSensor myCurrentSensor(currentMeterPin);
SSR mySwitch(relayPin ,currentMeterPin,maxAmps,dimmable,relayTemperatureSensor,fanPin);
ws2812Strip myLedStrip(ledStripPin, ledNumber);


void set2KhzInterrupt1()
{
cli();//stop interrupts
//set timer0 interrupt at 2kHz
  TCCR0A = 0;// set entire TCCR2A register to 0
  TCCR0B = 0;// same for TCCR2B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 124;// = (16*10^6) / (2000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
  sei();
}

void setup() {
  
  if(mySwitch.currentSensor().isValid())
    set2KhzInterrupt1();
  else
    cli();
}

ISR(TIMER0_COMPA_vect){
    mySwitch.currentSensor().isrRead();
}

void loop() {
  
  mySwitch.update();
  myLedStrip.update();
}
