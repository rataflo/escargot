/*
 * RGB light from potentiometer value.
 * Flo Gales 2020, license "rien à branler"
 */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIXEL_PIN 1
#define POT_PIN A1

Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
unsigned int potAvg = analogRead(POT_PIN);   

void setup() {
  //Serial.begin(9600);
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif

  strip.begin();
  strip.show();
}

void loop() {
  int pot = analogRead(POT_PIN);
  pot = xpavg(pot, potAvg);
  potAvg = (potAvg + pot) / 2;
  //Serial.println(potAvg);
  byte potMap = map(potAvg, 0, 1023, 0, 255);
  for(int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(potMap));
      //strip.setPixelColor(i, 255,255,255);
  }
  strip.show();
  delay(100);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

int16_t xpavg(int16_t newdat, int16_t avg){

   // (3 * avg + 1 * newdat) / 4
   return( (((avg<<2) - avg + newdat) + 2) >> 2 );
}
