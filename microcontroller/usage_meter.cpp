#include "Particle.h"
#include "lib/Particle-NeoPixel/src/neopixel/neopixel.h"

SYSTEM_MODE(AUTOMATIC);

#define PIXEL_PIN D2
#define PIXEL_COUNT 24
#define PIXEL_TYPE WS2812

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

void setup() {
    Serial.begin(9600);
    Serial.println("Serial Begin");
    strip.begin();
    strip.setBrightness(20);
    strip.show(); 
}

void fillGradient(uint32_t startColor, uint32_t endColor) {
    uint16_t pixelIndex;
    uint16_t lightValue;
    for (pixelIndex=0; pixelIndex<strip.numPixels(); pixelIndex++) {
        for (lightValue=0; lightValue<255; lightValue+=25) {
            uint32_t color = strip.Color(0 +  lightValue, 255 - lightValue, 0);
            strip.setPixelColor(pixelIndex, color);
            // uint16_t mappedValue = map(i, 0, strip.numPixels(), 0, 1);
            // float x = mappedValue / 100;
            // uint32_t color = strip.Color(2.0f * x, 2.0f * (1 - x), 0);
        }
        strip.show();
        delay(100);
    }
}

void updateLights() {
    strip.clear();
    delay(1000);

    // Only light up 10 lights
    uint16_t length = 10;

    // updateLength to display period percentage
    strip.updateLength(length);

    // default start to green
    uint32_t startColor = strip.Color(0, 255, 0);
    
    // default end to red
    // TODO calculate ending gradient
    uint32_t endColor = strip.Color(255, 0, 0);

    fillGradient(startColor, endColor);
}

void loop() {
    updateLights();
    delay(1000 * 10);
}
