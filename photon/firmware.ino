#include <math.h>

/* 
-- Dependencies --
- ArduinoJson.h
   - https://github.com/bblanchon/ArduinoJson
   
 - Particle-Neopixel
   - https://github.com/technobly/Particle-NeoPixel

I didn't include the dependencies since I built this via
https://build.particle.io, but I've included the repos 
in case they're needed later. 
*/
#include <ArduinoJson.h>
#include <neopixel.h>

/*
Intended to be used with a NeoPixel WS2812 LED ring. 
Connected to Pin D2 on a Particle Photon. 
*/
#define PIXEL_PIN D2
#define PIXEL_COUNT 24
#define PIXEL_TYPE WS2812
#define PIXEL_ANIMATION_DELAY 250
#define PIXEL_BRIGHTNESS 13
#define MINUTES 60 * 1000

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

void responseHandler(const char *event, const char *data);

void setup() {
    Particle.subscribe("hook-response/data_usage/0", responseHandler, MY_DEVICES);
    strip.begin();
    strip.setBrightness(PIXEL_BRIGHTNESS);
    strip.show();
}

void loop() {
    Particle.publish("data_usage");
    delay(65 * MINUTES);
}

void fillGradient(uint8_t length) {
    float lightValue = 0;
    float incrementingAmount = 255 / strip.numPixels();

    for (int pixelIndex = 0; pixelIndex < length; pixelIndex++) {
        uint32_t color = strip.Color(0 + lightValue, 255 - lightValue, 0);
        strip.setPixelColor(pixelIndex, color);
        strip.show();

        lightValue += incrementingAmount;
        delay(PIXEL_ANIMATION_DELAY);
    }
}

void updateLights(int dataUsedPercentage) {
    int length = round(strip.numPixels() * ((float)dataUsedPercentage / 100));

    for (int i = 0; i < 4; i++) {
        fillGradient(length);
        delay(15 * MINUTES);
        strip.clear();
    }
}

void responseHandler(const char *event, const char *data) {
    StaticJsonBuffer<512> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(data);
    int dataUsedPercentage = root["data_used_percentage"];

    if (dataUsedPercentage > 100) {
        dataUsedPercentage = 100;
    }

    updateLights(dataUsedPercentage);
}
