#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
#define LEDS 150
#define LINE_LENGTH 20
#define MAX_TICK 360

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

const float pi = 3.1412;
double ledmap[150];
int tick = 0;

void setup() {
  strip.begin();
  spark();
  setAllToMapColor();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  next();
//  spark();
//  setAllToMapColor();
//  mutateMap();
  cycleColorWave(tick);
//  delay(100);
}

void next() {
  if (tick >= MAX_TICK) {
    tick = 0;
    return;
  }
  tick++;
}

void spark() {
  for (int i = 0; i < LEDS; i++) {
    ledmap[i] = random(0.0, 1.0);
  }
}

void setAllToMapColor() {
  // color
  for (int i = 0; i < LEDS; i++) {
    strip.setPixelColor(i, color23(ledmap[i]));
  }
  strip.show();
}

void cycleColorWave(int tick) {
  int i, deg;
  for(i=0; i < LEDS; i++) {
    deg = (tick*3 + i*10) % 360;
    float rad = deg * pi / 360;
    strip.setPixelColor(i, color23((sin(rad) * sin(rad)) + 0.5 * sin(tick * pi / 90) - 0.5));
  }
    strip.show();
}

void cycleColors(int tick) {
  // color
  float rad = tick * pi / 360;
  setAllTo(color23(sin(rad) * sin(rad)));
  strip.show();
}


void mutateMap() {
  uint32_t n[LEDS];
  for (int index = 0; index < LEDS; index++) {
    double avg = 
      ledmap[index-2] + 
      ledmap[index-1] + 
      ledmap[index] + 
      ledmap[index+1] +
      ledmap[index+2]
      / 5;
    if (avg > 0.7) {
      avg = 0.6;
    }
    if (avg < 0.2) {
      avg = 0.2;
    }
    n[index] = avg;
  }
  int g = 0;
  for (int j = 0; j < LEDS; j++) {
    double nval = (n[j] + ledmap[j])/2;
    nval += random(0.4, 1);
    ledmap[j] = nval;
  }
}

void setAllTo(uint32_t color) {
  int i;
  for(i=0; i < LEDS; i++) {
    strip.setPixelColor(i, color);
  }
}

uint32_t color23 (double value) {
  if (value > 1) { value = 1; }
  if (value < 0) { value = 0; }

  return strip.Color(
    floor(240*value), 
    floor(120*value*value), 
    floor(30*value*value*value)
  );
}

