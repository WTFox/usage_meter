// This #include statement was automatically added by the Particle IDE.
#include <ArduinoJson.h>

// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>

#define PIXEL_PIN D2
#define PIXEL_COUNT 24
#define PIXEL_TYPE WS2812

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

void responseHandler(const char *event, const char *data);

void setup() {
    strip.begin();
    strip.setBrightness(50);
    strip.show();
    Particle.subscribe("hook-response/data_usage/0", responseHandler, MY_DEVICES);
}

void loop() {
    Particle.publish("data_usage");
    delay(65 * 60 * 1000);
}

void fillGradient(uint8_t length, int wait) {
    int pixelIndex;
    float lightValue = 0;
    float incrementingAmount = 255 / strip.numPixels();
    for (pixelIndex = 0; pixelIndex < length; pixelIndex++) {
        uint32_t color = strip.Color(0 + lightValue, 255 - lightValue, 0);
        strip.setPixelColor(pixelIndex, color);
        strip.show();
        lightValue += incrementingAmount;
        delay(wait);
    }
}

void updateLights(int dataUsedPercentage, int wait) {
    float length = strip.numPixels() * ((float)dataUsedPercentage / 100);
    for (int i = 0; i < 4; i++) {
        fillGradient(length, wait);
        // wait 15 minutes
        delay(15 * 60 * 1000);
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
    updateLights(dataUsedPercentage, 200);
}
