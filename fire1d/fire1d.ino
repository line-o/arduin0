#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
#define LEDS 150
#define LINE_LENGTH 20
#define MAX_TICK 360

const int w = 20;
int horst[w];
int index = 0;
int sum = 0;

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
  Serial.begin(9600);
  pinMode(7, INPUT);
  strip.begin();
//  spark();
//  setAllToMapColor();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  next();
//  setAllToMapColor();
  index = (index + 1) % w;
  horst[index] = digitalRead(7);
  sum = 0;
  for (int i = 0; i<w; i++) {
    sum += horst[i];
  }
  cycleColorWave(tick);
  Serial.println(sum);
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
    deg = (tick*4);// % 360;
    float rad = deg * pi / 360;
    strip.setPixelColor(i, 
      color23(
        (0.5* sin(rad - i*pi/25) * sin(rad + i*pi/15)) + 
        0.25 * cos(i * pi / 3) * cos(i * pi / 25) + sum * 0.15 - 0.05)
    );
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
    floor(250*value), 
    floor(100*value*value), 
    floor(20*value*value*value)
  );
}

