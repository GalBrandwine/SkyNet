#include <FastLED.h>
#define DATA_PIN 27
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS 6
#define BRIGHTNESS 96
#define MIN_BRIGHTNESS 50
#define MAX_BRIGHTNESS 255

CRGB leds[NUM_LEDS];

/**
 * @brief Convert time of day to HSV values
 * 
 * Based on Classification Other application, of the HSV color space can be the day/night classification depending on the average Value, (amount of light).
 *  @code 
 *  sum_brightness = np.sum(hsv[:,:,2])
 *  area           = 600*1100.0 # pixels
 *  avg            = sum_brightness/area
 *  @endcode
 *  If the average is greater than 120 for example, is a day image, otherwise, a night image.
 * 
 * @param timeOfDay 
 * @return const CHSV 
 */
const CHSV timeOfDayToHSV(const ts timeOfDay)
{
    auto hours = timeOfDay.hour;
    auto hourAngle = map(hours, 1, 24, 0, 360);
    auto hue = map8(hourAngle, HUE_RED, HUE_PINK);
    auto value = map8(hourAngle, MIN_BRIGHTNESS, MAX_BRIGHTNESS);

    Serial.print("\ntimeOfDayToHSV\n");
    Serial.print("For hour: ");
    Serial.println(hours);
    Serial.print("Got hourAngle: ");
    Serial.println(hourAngle);
    Serial.print("Got hue: ");
    Serial.println(hue);
    Serial.print("And value: ");
    Serial.println(value);
    return CHSV(hue, 255, value);
}

void showProgramTimeOfDay(const ts timeOfDay)
{
    auto newHsv = timeOfDayToHSV(timeOfDay);

    for (int i = 0; i < NUM_LEDS; ++i)
    {
        // leds[i] = CHSV(random8(), 255, 255); // hue, saturation, value
        leds[i].setHSV(newHsv.h, newHsv.s, newHsv.v);
        ;
    }
    FastLED.show();
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
