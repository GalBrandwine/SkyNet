#include <ESPAsyncWebServer.h>
#include <ds3231.h>
#include "/home/gal/dev/SkyNet/include/Weathersettings.h"

// Set these to your desired credentials.
const char *ssidAP = "SkyNetMaster";
const char *passwordAP = "SkyNetMaster";

// Existing wifi connection credentials.
const char *ssidWifi = "gozal_2.4";
const char *passwordWifi = "asdffdsa";

struct SkyNetStruct
{
    AsyncWebServer *server;
    Weather::WeatherType current_weather{Weather::WeatherType::Automatic};
    float current_temperature = 0;
    struct ts rtc_timestamp;
    String time_str = "None";
};
