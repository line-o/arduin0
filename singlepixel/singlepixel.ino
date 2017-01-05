#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
#define LEDS 150
#define LINE_LENGTH 20

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

int line[10]={0,0,0,0,0,0,0,0,0,0};
int line_end = 0;
int tick = 0;
int MAX_TICK = LEDS + 2 * LINE_LENGTH;


void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  next();
  randomLine(tick);
  delay(150);
  removeLine(tick);
  delay(150);
  whiteFlash(tick);
}

void next() {
  if (tick >= MAX_TICK) {
    tick = 0;
    return;
  }
  tick++;
}

void randomLine(int tick) {
  int index = random(floor(tick/2), tick);
  int u = index + LINE_LENGTH;
  // color
  uint32_t color = strip.Color(
    random(2, 80),
    random(2, 20),
    random(2, 20),
    0
  );
  for (;index < u; index++){
    strip.setPixelColor(index, color);
  }
  strip.show();
}

void removeLine(int tick) {
  int index = random(floor(tick/2), tick);
  int u = index + 3;
  for (;index < u; index++){
    strip.getPixelColor(index);
    strip.setPixelColor(index, 0);
  }
  strip.show();
}

void whiteFlash(int tick) {
  uint16_t j;
  if (tick % 50 > 0) {
    return;
  }
  for(j=0; j<10; j++) { // 5 cycles of all colors on wheel
    setAllTo(j * 2);
    strip.show();
  }
  for(j=0; j<11; j++) { // 5 cycles of all colors on wheel
    setAllTo(20 - j * 2);
    strip.show();
  }
  delay(150);
}

void setAllTo(uint8_t val) {
  uint16_t i;
  uint32_t color = strip.Color(val, val, val, 0);
  for(i=0; i< strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
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

uint8_t blue(uint32_t c) {
  return (c);
}

uint8_t red(uint32_t c) {
  return (c >> 8);
}

uint8_t green(uint32_t c) {
  return (c >> 16);
}

