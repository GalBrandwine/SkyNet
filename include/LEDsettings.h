#include <FastLED.h>
#define DATA_PIN 27
#define LED_TYPE WS2812
#define COLOR_ORDER GRB

#define NUM_LEDS_IN_GROUP 6
#define NUM_OF_GROUPS 3

#define BRIGHTNESS 96
#define MIN_BRIGHTNESS 50
#define MAX_BRIGHTNESS 255

// Sunset/Sunrise
#define SUNSET_TIME_HOUR 19
#define SUNRISE_TIME_HOUR 6

// Months
#define WINTER_START 10
#define WINTER_END 4
#define SUMMER_START 5
#define SUMMER_END 9

#define IM_SLAVE "Compiling SkyNet for slave version"

static const uint8_t RAIN_PIN = 15;
static bool IS_RAINING = false;

CRGB leds[NUM_LEDS_IN_GROUP * NUM_OF_GROUPS];

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
        // value = map8(hourAngle, MAX_BRIGHTNESS, MIN_BRIGHTNESS);
        value = map(hourAngle, 0, 360, MAX_BRIGHTNESS, MIN_BRIGHTNESS);

    Serial.print("\ntimeOfDayToHSV\n");
    Serial.print("For hour: ");
    Serial.println(hours);
    Serial.print("Got hourAngle: ");
    Serial.println(hourAngle);
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
 * @brief Set the Brightness object
 * 
 * @param skyNetStruct 
 */
void setBrightness(SkyNetStruct &skyNetStruct)
{
    Serial.println("Setting brightness");
    FastLED.setBrightness(skyNetStruct.ledsSettings.leds_brightness);
    skyNetStruct.ledsSettings.is_brightness_changed = false;
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
    for (int i = 0; i < NUM_LEDS_IN_GROUP * NUM_LEDS_IN_GROUP; ++i)
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
    auto light_group_number = random(0, NUM_OF_GROUPS);
    Serial.print("Doing lightning in group: ");
    Serial.println(light_group_number);
    for (int iteration = 0; iteration < numIterations; ++iteration)
    {
        for (int i = 0; i < NUM_LEDS_IN_GROUP; ++i)
        {
            leds[light_group_number * NUM_LEDS_IN_GROUP + i] = CRGB::White;
        }
        FastLED.show();

        for (int i = 0; i < NUM_LEDS_IN_GROUP; ++i)
        {
            leds[light_group_number * NUM_LEDS_IN_GROUP + i] = CRGB::Black;
        }
        FastLED.show();

        // Delay between lightnings
        delay(delayTime);
    }

    // Done with lighting, restoring old settings
    for (int i = 0; i < NUM_LEDS_IN_GROUP; ++i)
    {
        leds[light_group_number * NUM_LEDS_IN_GROUP + i] = current_color;
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
        for (int i = 0; i < NUM_LEDS_IN_GROUP; ++i)
        {
            // leds[i] = CHSV(random8(), 255, 255); // hue, saturation, value
            leds[i].setHSV(random8(), 255, 255);
        }
        FastLED.delay(delayTime);
    }
}

void showProgramError(int numIterations)
{
    for (int iteration = 0; iteration < numIterations; ++iteration)
    {
        for (int i = 0; i < NUM_LEDS_IN_GROUP * NUM_LEDS_IN_GROUP; ++i)
        {
            // leds[i] = CHSV(random8(), 255, 255); // hue, saturation, value
            leds[i].setColorCode(CRGB::Red);
        }
        FastLED.show();

        for (int i = 0; i < NUM_LEDS_IN_GROUP * NUM_LEDS_IN_GROUP; ++i)
        {
            leds[i] = CRGB::Black;
        }
        FastLED.show();

        // Delay between lightnings
        delay(500);
    }
}

/**
 * @brief switches off all LEDs
 * 
 * @param delayTime 
 */
void showProgramCleanUp(long delayTime)
{
    for (int i = 0; i < NUM_LEDS_IN_GROUP * NUM_LEDS_IN_GROUP; ++i)
    {
        leds[i] = CRGB::Black;
    }
    FastLED.delay(delayTime);
}

void AsyncRainTask(void *pvParameters);
void InitRainStuff()
{
    pinMode(RAIN_PIN, OUTPUT); // Declare the LED as an output

    // xTaskCreate should be in SETUP() function - if not the scheduler is not working properly, and there's a xTaskCreate shadowing.
    xTaskCreate(
        AsyncRainTask, // Function that should be called
        "Rain task",   // Name of the task (for debugging)
        10000,         // Stack size (bytes)
        NULL,          // Parameter to pass
        1,             // Task priority
        NULL           // Task handle
    );
}

/**
 * @brief Init the FastLED driver.
 * 
 */
void initLeds(const SkyNetStruct &skyNetStruct)
{
    delay(1000);                                                                                                              // initial delay of a few seconds is recommended
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS_IN_GROUP * NUM_OF_GROUPS).setCorrection(TypicalLEDStrip); // initializes LED strip
    FastLED.setBrightness(BRIGHTNESS);                                                                                        // global brightness
    showProgramCleanUp(100);

    randomSeed(analogRead(0));

#ifdef IM_SLAVE
    InitRainStuff();
#endif IM_SLAVE
}

void StartRain()
{
    IS_RAINING = true;
}

void StopRain()
{
    IS_RAINING = false;
}

void AsyncRainTask(void *pvParameters)
{
    int TICKS = 100;

    /** @brief Time to rain in minutes */
    int time_to_rain = -1;

    bool is_raining_just_started = true;
    bool is_raining_stopped_by_itself = false;
    auto time = millis();
    auto time_between_rain_occasions = random(10, 50);

    for (;;)
    {
        if (IS_RAINING and !is_raining_stopped_by_itself)
        {
            if (is_raining_just_started)
            {
                is_raining_just_started = false;
                time = millis();

                /** @brief Minutes */
                time_to_rain = random(5, 30);
                time = millis();
            }
            while (IS_RAINING and time_to_rain > 0)
            {
                digitalWrite(RAIN_PIN, HIGH); // Turn the LED on
                vTaskDelay(pdMS_TO_TICKS(TICKS));
                digitalWrite(RAIN_PIN, LOW); // Turn the LED on
                vTaskDelay(pdMS_TO_TICKS(TICKS));

                auto now = millis();
                int time_passed_in_seconds = (int(now - time) / 1000) % 60;
                int time_passe_in_minutes = ((int(now - time) / 1000) / 60) % 60;

                if (time_to_rain - time_passe_in_minutes <= 0)
                {
                    time_to_rain = -1;
                    is_raining_stopped_by_itself = true;

                    time = millis();
                    Serial.printf("Stopping rain by itself.\n");
                }
                else
                {
                    Serial.printf("Will rain for: %d\n", time_to_rain);
                    Serial.printf("Raining for: %d:%d\n", time_passe_in_minutes, time_passed_in_seconds);
                }
            }
        }
        else
        {
            auto now = millis();
            int time_passed_in_seconds = (int(now - time) / 1000) % 60;
            int time_passe_in_minutes = ((int(now - time) / 1000) / 60) % 60;

            if (time_between_rain_occasions - time_passe_in_minutes <= 0)
            {
                is_raining_stopped_by_itself = false;
                is_raining_just_started = true;
                Serial.printf("\nStarting rain by itself.\n");
            }
            else
            {
                Serial.printf("Time between rain: %d\n", time_between_rain_occasions);
                Serial.printf("Time passed from last rain: %d:%d\n", time_passe_in_minutes, time_passed_in_seconds);
            }

            vTaskDelay(pdMS_TO_TICKS(TICKS * 10));
        }
    }

    vTaskDelete(NULL);
}