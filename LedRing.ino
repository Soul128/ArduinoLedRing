#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#define PIN 3
#define NUMPIXELS 12
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int newLightValCounter = 0;

float transitionSpeed = 0.01;

int delayval = 1000 / 100;
int newLightVal = delayval * 50;

float current[NUMPIXELS][3] = {};
float target[NUMPIXELS][3] = {};
float hueShift = 0;
float hueShiftSpeed = 100;

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
  randomBlueColor(255, 255);
  }

  interpolateRGB(true); 

  // rotateRGB(255, 255);
  pixels.show();

  newLightValCounter += delayval;
  delay(delayval);
}

void randomHue(int sat, int val) {
  for (int i = 0; i < NUMPIXELS; i++) {
    target[i][0] = (float)random(0, 65534);
    target[i][1] = sat;
    target[i][2] = val;
  }
}

void randomBlueColor(int sat, int val) {
  for (int i = 0; i < NUMPIXELS; i++) {
    // int rndLight = random(0, 12);
    float hue = 32767 + random(-20000, 20000);
    target[i][0] = hue;
    target[i][1] = sat;
    target[i][2] = val;
  }
}

void rotateRGB(int sat, int val) {
  hueShift += hueShiftSpeed;
  if (hueShift > 65535)
    hueShift = 0;

  for (float i = 0; i < NUMPIXELS; i++) {
    int hue = hueShift + (i / NUMPIXELS) * 65535;
    if (hue > 65535)
      hue -= 65535;
    pixels.setPixelColor(i, pixels.ColorHSV(hue, sat, val));
  }
}


void interpolateRGB(bool hsv) {
  for (int i = 0; i < NUMPIXELS; i++) {
    for (int x = 0; x <= 2; x++) {
      current[i][x] += ((target[i][x] - current[i][x]) * transitionSpeed);
    }
  }
  for (int i = 0; i < NUMPIXELS; i++) {
    if (hsv)
      pixels.setPixelColor(i, pixels.ColorHSV((uint16_t)current[i][0], (int)current[i][1], (int)current[i][2]));
    else
      pixels.setPixelColor(i, pixels.Color((int)current[i][0], (int)current[i][1], (int)current[i][2]));
  }
}
