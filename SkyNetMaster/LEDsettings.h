#include <FastLED.h>
#define DATA_PIN 27
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS 6
#define BRIGHTNESS 96
#define MIN_BRIGHTNESS 50
#define MAX_BRIGHTNESS 255
#define SUNSET_TIME_HOUR 19
#define SUNRISE_TIME_HOUR 6

#define WINTER_START 10
#define WINTER_END 4

#define SUMMER_START 5
#define SUMMER_END 9

CRGB leds[NUM_LEDS];

const int weather_to_sat(const SkyNetStruct &skyNetStruct);
/**
 * @brief Convert time of day to HSV values
 * 
 * @param timeOfDay 
 * @return const CHSV 
 */
const CHSV timeOfDayToHSV(const SkyNetStruct &skyNetStruct)
{
    auto hours = skyNetStruct.rtc_timestamp.hour;
    auto hourAngle = map(hours, 1, 24, 0, 360);
    // auto hue = hourAngle; //map8(hourAngle, HUE_RED, HUE_PINK + 60);

    auto sat = weather_to_sat(skyNetStruct);

    // This is brightness. We want brightness to rise until noon.
    uint8_t value;
    if (hours < SUNSET_TIME_HOUR)
        value = map8(hourAngle, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    else
        value = map8(hourAngle, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
        value = map(hours, 1, 24, MAX_BRIGHTNESS, MIN_BRIGHTNESS);

    Serial.print("\ntimeOfDayToHSV\n");
    Serial.print("For hour: ");
    Serial.println(hours);
    Serial.print("Got hourAngle: ");
    Serial.println(hourAngle);
    // Serial.print("Got hue: ");
    // Serial.println(hue);
    Serial.print("sat: ");
    Serial.println(sat);
    Serial.print("And value: ");
    Serial.println(value);

    return CHSV(hourAngle, sat, value);
}

const int weather_to_sat(const SkyNetStruct &skyNetStruct)
{
    int sat = 255;
    switch (skyNetStruct.current_weather)
    {
    case Weather::WeatherType::Cloudy:
        sat = 155;
    case Weather::WeatherType::Drizzle:
        sat = 160;
    case Weather::WeatherType::Overcast:
        sat = 185;
    case Weather::WeatherType::PartiallyCloudy:
        sat = 225;
    case Weather::WeatherType::Rain:
        sat = 200;
    case Weather::WeatherType::Snow:
        sat = 100;
    case Weather::WeatherType::Stormy:
        sat = 155;
    case Weather::WeatherType::Sunny:
        sat = 255;
    }
    return sat;
}

/**
 * @brief Set LED's color values according to time of day
 * 
 * @param timeOfDay timestamp with current time of day
 */
void showProgramTimeOfDay(const SkyNetStruct &skyNetStruct)
{
    auto newHsv = timeOfDayToHSV(skyNetStruct);
    // auto seasonRGB = weather_to_rbg(skyNetStruct);
    for (int i = 0; i < NUM_LEDS; ++i)
    {
        leds[i].setHSV(newHsv.h, newHsv.s, newHsv.v);
    }
    FastLED.show();
}

/**
 * @brief Perform lightning effects
 * 
 * @param numIterations number of lightnings
 * @param delayTime delay between lighting
 */
void doLightning(int numIterations, long delayTime)
{
    // Save current color
    auto current_color = leds[0];

    for (int iteration = 0; iteration < numIterations; ++iteration)
    {
        for (int i = 0; i < NUM_LEDS; ++i)
        {
            leds[i] = CRGB::White;
        }
        FastLED.show();

        for (int i = 0; i < NUM_LEDS; ++i)
        {
            leds[i] = CRGB::Black;
        }
        FastLED.show();

        // Delay between lightnings
        delay(delayTime);
    }

    // Done with lighting, restoring old settings
    for (int i = 0; i < NUM_LEDS; ++i)
    {
        leds[i] = current_color;
    }
}

/**
 * @brief switches on all LEDs.
 * 
 * @param numIterations indicates how often LEDs are switched on in random colors
 * @param delayTime indicates for how long LEDs are switched on.
 */
void showProgramRandom(int numIterations, long delayTime)
{
    for (int iteration = 0; iteration < numIterations; ++iteration)
    {
        for (int i = 0; i < NUM_LEDS; ++i)
        {
            // leds[i] = CHSV(random8(), 255, 255); // hue, saturation, value
            leds[i].setHSV(random8(), 255, 255);
        }
        FastLED.delay(delayTime);
    }
}

/**
 * @brief switches off all LEDs
 * 
 * @param delayTime 
 */
void showProgramCleanUp(long delayTime)
{
    for (int i = 0; i < NUM_LEDS; ++i)
    {
        leds[i] = CRGB::Black;
    }
    FastLED.delay(delayTime);
}

/**
 * @brief Init the FastLED driver.
 * 
 */
void initLeds()
{
    delay(1000);                                                                                     // initial delay of a few seconds is recommended
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip); // initializes LED strip
    FastLED.setBrightness(BRIGHTNESS);                                                               // global brightness
    showProgramCleanUp(100);
}
