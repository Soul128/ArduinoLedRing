#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#define PIN 3
#define NUMPIXELS 12
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int newLightValCounter = 0;

float transitionSpeed = 0.05;

int delayval = 1000 / 10;
int newLightVal = delayval * 2;

float current[NUMPIXELS][3] = {};
float target[NUMPIXELS][3] = {};

void setup() {
  for (int i = 0; i < NUMPIXELS; i++) {
    for (int x = 0; x <= 2; x++) {
      current[i][x] = 0;
      target[i][x] = 128;
    }
  }
  pixels.begin();
}

void loop() {
  if (newLightValCounter >= newLightVal) {
    newLightValCounter = 0;
    for (int i = 0; i <= 4; i++) {
      int rndLight = random(0, 12);
      int intensity = random(0, 17) * random(0, 17);
      if (intensity > 128) {
        target[rndLight][0] = intensity * 1.0;
        target[rndLight][1] = intensity * 1.0;
        target[rndLight][2] = intensity * 1.0;
      } else {
        target[rndLight][0] = intensity * 0.1;
        target[rndLight][1] = intensity * 0.4;
        target[rndLight][2] = intensity * 0.5;
      }
    }
  }

  interpolateRGB();
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color((int)current[i][0], (int)current[i][1], (int)current[i][2]));
  }
  pixels.show();

  newLightValCounter += delayval;
  delay(delayval);
}

void interpolateRGB() {
  for (int i = 0; i < NUMPIXELS; i++) {
    for (int x = 0; x <= 2; x++) {
      current[i][x] += ((target[i][x] - current[i][x]) * transitionSpeed);
    }
  }
}
