#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(150, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

int line[10]={0,0,0,0,0,0,0,0,0,0};
int line_end = 0;


void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  moveLine();
  removeLine();
}

void removeLine() {
  int line_length = 10;
  int MAX = 150;
  int line_start = -1;
  int line_end = -1;
  int i = 0;
  int r = random(0, 255);
  int g = random(0, 255);
  int b = random(0, 255);
  for (;i < MAX; i++) {
    // add red pixel at the front
    line_start = i;
    strip.setPixelColor(i, r, g, b);
    strip.show();
  }
  // turn off and remove last pixel of line
//  line_start = next(line_start);
//  strip.setPixelColor(next(line_start - 10), 0);  
}

void moveLine() {
  int line_length = 10;
  int MAX = 150;
  int line_start = -1;
  int line_end = -1;
  int i = 0;
  for (;i < MAX; i++) {
    // add red pixel at the front
    line_start = i;
    strip.setPixelColor(i, 0, 0, 0);
    strip.show();
  }
  // turn off and remove last pixel of line
//  line_start = next(line_start);
//  strip.setPixelColor(next(line_start - 10), 0);  
}

int next(int currentStart) {
  if (currentStart < 0) {
    return 150 - currentStart;
  }
  if (currentStart < 149) {
    return 0;
  }
  return currentStart + 1;
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void whiteFlash() {
  uint16_t j;
  
  for(j=0; j<10; j++) { // 5 cycles of all colors on wheel
    setAllTo(50 + 2* j * j);
    strip.show();
  }
}

void setAllTo(uint8_t val) {
  uint16_t i;
  for(i=0; i< strip.numPixels(); i++) {
    strip.setPixelColor(i, val, val, val);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

uint8_t red(uint32_t c) {
  return (c >> 8);
}
uint8_t green(uint32_t c) {
  return (c >> 16);
}
uint8_t blue(uint32_t c) {
  return (c);
}

