#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
#define LEDS 150
#define LINE_LENGTH 20
#define MAX_TICK 100000

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

uint32_t palette[11];
int ledmap[150];
int tick = 0;


void setup() {
  strip.begin();
  palette[0] = strip.Color(  0,   0,  0, 0);
  palette[1] = strip.Color(  5,   0,  0, 0);
  palette[2] = strip.Color( 10,   1,  0, 0);
  palette[3] = strip.Color( 20,   2,  0, 0);
  palette[4] = strip.Color( 30,   4,  0, 0);
  palette[5] = strip.Color( 50,   8,  0, 0);
  palette[6] = strip.Color( 70,  16,  0, 0);
  palette[7] = strip.Color(100,  24,  0, 0);
  palette[8] = strip.Color(120,  36,  1, 0);
  palette[9] = strip.Color(140,  48,  5, 0);
  palette[10] = strip.Color(160, 64,  10, 0);
  palette[11] = strip.Color(180, 72,  22, 0);
  for (int i=0; i<150;i++) {
    ledmap[i] = random(0,11);
  }
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  next();
//  for (int i=0; i<150;i++) {
//    ledmap[i] = random(0,10);
//  }
  mutateMap();
  setAllToMapColor();
  delay(50);
//  setAllToRandomPaletteColor(tick);
//  delay(1250);
//  whiteFlash(tick);
}

void next() {
  if (tick >= MAX_TICK) {
    tick = 0;
    return;
  }
  tick++;
}

void setAllToMapColor() {
  // color
  for (int i=0; i<150;i++) {
    uint32_t color = palette[ledmap[i]];
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void setAllToRandomPaletteColor(int tick) {
  // color
  uint32_t color = palette[random(0, 11)];
  strip.setPixelColor(random(0,150), color);
  strip.show();
}

void mutateMap() {
  int index = 0;
  uint32_t n[150];
  for (;index<150;index++) {
    int avg = floor(
      ledmap[index] + 
      ledmap[index+1] + 
      ledmap[index+2]
      / 3);
    if (avg > 9) {
      avg = 6;
    }
    if (avg < 1) {
      avg = 2;
    }
    n[index] = avg;
  }
  int g = 0; //random(-1, 1);
  for (int j = 0;j<150;j++) {
//    int rnd = ledmap[j] + random(-4, 0);

    int nval = round(n[j]+ledmap[j]/2);
    nval += random(-4, -1);
    if (nval > 9) {
      nval = 9;
    }
    if (nval < 0) {
      nval = 0;
    }
    ledmap[j] = nval;
  }
}

void whiteFlash(int tick) {
  uint16_t j;
  int val;
  uint32_t color;
  if (tick % 50 > 0) {
    return;
  }
  for(j=0; j<10; j++) { // 5 cycles of all colors on wheel
    val = j * 2;
    color = strip.Color(val, val, val, 0);
    setAllTo(color);
    strip.show();
  }
  for(j=0; j<11; j++) { // 5 cycles of all colors on wheel
    val = 20 - j * 2;
    color = strip.Color(val, val, val, 0);
    setAllTo(color);
    strip.show();
  }
  delay(150);
}

void setAllTo(uint32_t color) {
  uint16_t i;
  for(i=0; i< strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
}

