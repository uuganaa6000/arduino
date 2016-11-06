#include <Adafruit_NeoPixel.h>

#define PIN 14
Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN, NEO_GRB + NEO_KHZ800);

void ledsetup(void) {
  // This is for 5V 16MHz
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif  
  strip.begin();
  strip.show(); 
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

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

int startup = 0;
void ledloop(void) {
  if(startup == 0){
    rainbowCycle(10);
    colorWipe(strip.Color(0, 0, 0), 10); // black
    delay(100);
    for(int i = 0; i < 3; i++){
      colorWipe(strip.Color(0, 0, 255), 0); // blue
      delay(1000);
      colorWipe(strip.Color(0, 0, 0), 0); // black
      delay(400);
    }
  }
  startup = 1;
}

void ledAirconController(int commandReciever) {
  if(commandReciever == 1){
      colorWipe(strip.Color(0, 128, 0), 0); // green
  }
  else if(commandReciever == 0){
      colorWipe(strip.Color(0, 0, 0), 0); // black
  }
}

