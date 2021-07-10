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

void frequencyTask(void *pvParameters);
void InitRainStuff()
{
    pinMode(RAIN_PIN, OUTPUT); // Declare the LED as an output

    // xTaskCreate should be in SETUP() function - if not the scheduler is not working properly, and there's a xTaskCreate shadowing.
    xTaskCreate(
        frequencyTask, // Function that should be called
        "Rain task",   // Name of the task (for debugging)
        1000,          // Stack size (bytes)
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

#ifdef IM_SLAVE
    InitRainStuff();
#endif IM_SLAVE
}

void StartRain()
{
    IS_RAINING = true;
    // digitalWrite(pin_number, HIGH); // Turn the LED on
}

void StopRain()
{
    IS_RAINING = false;
    // digitalWrite(pin_number, LOW); // Turn the LED off
}

void frequencyTask(void *pvParameters)
{
    int TICKS = 100;
    // int seconds = 0;
    // int valveTime = 0;
    // int hours_passed_from_last_watering = 0; //max watering interval is 168 hours (a week)

    for (;;)
    {

        // seconds++;
        // StartRain(RAIN_PIN);
        if (IS_RAINING)
        {
            digitalWrite(RAIN_PIN, HIGH); // Turn the LED on
            vTaskDelay(pdMS_TO_TICKS(TICKS));
            digitalWrite(RAIN_PIN, LOW); // Turn the LED on
            vTaskDelay(pdMS_TO_TICKS(TICKS));
        }
        else
            vTaskDelay(pdMS_TO_TICKS(TICKS * 10));
        // if (seconds % 3600 == 0)
        // { //10
        //     hours_passed_from_last_watering++;
        // }
        // Debug
        //    Serial.print("frequencyTask::isSystemWatering: ");Serial.println(isSystemWatering);
        //    Serial.print("frequencyTask::FREQUENCY_RESET_VALUE: ");Serial.println(FREQUENCY_RESET_VALUE);
        //    Serial.print("frequencyTask::frequency_hours: ");Serial.println(frequency_hours);
        //    Serial.print("frequencyTask::frequency_hours in seconds: ");Serial.println(frequency_hours*3600);
        //    Serial.print("frequencyTask::seconds passed from last watering: ");Serial.println(seconds);
        //    Serial.print("frequencyTask::hours_passed_from_last_watering: ");Serial.println(hours_passed_from_last_watering);
        //    Serial.print("frequencyTask::valveTime: ");Serial.println(valveTime);
        //    Serial.println("");

        // if (frequency_hours != FREQUENCY_RESET_VALUE)
        // {
        //     seconds = seconds % (frequency_hours * 3600);                            // Frequency_hours in seconds. (If frequency_hours==0) then it throws DivisionByZero!!)
        //     time_left_until_next_watering_min = frequency_hours * 60 - seconds / 60; // hours_passed_from_last_watering;
        // }
        // else
        // {
        //     seconds = 0;
        //     valveTime = seconds;
        //     hours_passed_from_last_watering = 0;
        //     Serial.println("frequencyTask::hours_passed_from_last_watering RESETED ");
        // }

        // xSemaphoreTake(xMutex, portMAX_DELAY); // Start critical section, that accessing SHARED data.
        // switch (valveMode)
        // {
        // case VALVE_MODES::timer:
        //     Serial.print(seconds - valveTime);
        //     Serial.print(" ");
        //     Serial.println(runtime * 60);
        //     if (seconds - valveTime >= runtime * 60)
        //     {
        //         if (closeValve())
        //         {
        //             isSystemWatering = false;
        //             valveTime = 0;
        //         }
        //     }
        //     if (hours_passed_from_last_watering >= frequency_hours && frequency_hours != FREQUENCY_RESET_VALUE)
        //     { // Time to water the plants.
        //         Serial.println("frequencyTask::TIME TO WATER THE PLANTS");
        //         if (!isSystemWatering)
        //         {
        //             if (openValve())
        //             {
        //                 isSystemWatering = true;
        //                 hours_passed_from_last_watering = 0;
        //                 valveTime = seconds;
        //             }
        //         }
        //     }
        //     break;
        // case VALVE_MODES::valve_open:
        //     if (openValve())
        //     {
        //         isSystemWatering = true;
        //         valveTime = seconds;
        //     }
        //     break;
        // case VALVE_MODES::valve_close:
        //     if (closeValve())
        //     {
        //         isSystemWatering = false;
        //     }
        //     break;
        // default:
        //     // statements
        //     break;
        // }
        // xSemaphoreGive(xMutex); // end start critical section
    }

    vTaskDelete(NULL);
}