#include "ESPAsyncWebServer.h"
// Set these to your desired credentials.
const char *ssidAP = "SkyNetMaster";
const char *passwordAP = "SkyNetMaster";

// Existing wifi connection credentials.
const char *ssidWifi = "gozal_2.4";
const char *passwordWifi = "asdffdsa";

struct SkyNetStruct
{
    AsyncWebServer *server;
    String time_str = "None";
};
