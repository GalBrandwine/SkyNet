// #include <HTTPClient.h>
#include <Preferences.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "RTCsettings.h"

bool setUpServer(SkyNetStruct *skyNetStruct)
{
    Serial.print("Initiating: ");
    Serial.println(__PRETTY_FUNCTION__);
    // Route for root / web page
    skyNetStruct->server->on("/", HTTP_GET, [skyNetStruct](AsyncWebServerRequest *request)
                             {
                                 if (!skyNetStruct->is_server_ready)
                                 {
                                     Serial.println("Server not yet ready, returning");
                                     request->send_P(503, "text/plain", "Service Unavailable /");
                                     return;
                                 }

                                 request->send(SPIFFS, "/index.html", String(), false);
                                 Serial.println("Got HTTP_GET on '/'");
                             });
    Serial.println(__LINE__);
    // Route to load style.css file
    skyNetStruct->server->on("/style.css", HTTP_GET, [skyNetStruct](AsyncWebServerRequest *request)
                             {
                                 if (!skyNetStruct->is_server_ready)
                                 {
                                     Serial.println("Server not yet ready, returning");
                                     request->send_P(503, "text/plain", "Service Unavailable: /style.css");
                                     return;
                                 }

                                 request->send(SPIFFS, "/style.css", "text/css");
                                 Serial.println("Got HTTP_GET on '/style.css'");
                             });
    Serial.println(__LINE__);
    // Route to load clout_1.jpg image
    skyNetStruct->server->on("/cloud_1.jpg", HTTP_GET, [skyNetStruct](AsyncWebServerRequest *request)
                             {
                                 if (!skyNetStruct->is_server_ready)
                                 {
                                     Serial.println("Server not yet ready, returning");
                                     request->send_P(503, "text/plain", "Service Unavailable: /cloud_1.jpg");
                                     return;
                                 }
                                 request->send(SPIFFS, "/cloud_1.jpg", String());
                                 Serial.println("Got HTTP_GET on '/cloud_1.jpg'");
                             });
    Serial.println(__LINE__);
    // Responce for getting current system time.
    skyNetStruct->server->on("/getCurrentSystemTime", HTTP_GET, [skyNetStruct](AsyncWebServerRequest *request)
                             {
                                 if (!skyNetStruct->is_server_ready)
                                 {
                                     Serial.println("Server not yet ready, returning");
                                     request->send_P(503, "text/plain", "Service Unavailable: /getCurrentSystemTime");
                                     return;
                                 }

                                 request->send_P(200, "text/plain", skyNetStruct->time_str.c_str());
                                 Serial.println("Got HTTP_GET on '/getCurrentSystemTime'");
                             });
    Serial.println(__LINE__);

    /**
   * @brief Responce for setting current system time.
   * Time setting could come with 2 different parameters ans values:
   * Hour:int
   * Minute:int
   */
    skyNetStruct->server->on("/setCurrentSystemTime", HTTP_GET, [skyNetStruct](AsyncWebServerRequest *request)
                             {
                                 if (!skyNetStruct->is_server_ready)
                                 {
                                     Serial.println("Server not yet ready, returning");
                                     request->send_P(503, "text/plain", "Service Unavailable: /setCurrentSystemTime");
                                     return;
                                 }
                                 // request->send_P(200, "text/plain", skyNetStruct->time_str.c_str());
                                 AsyncWebParameter *p = request->getParam(0);
                                 Serial.println("Got HTTP_GET on '/setCurrentSystemTime'");
                                 if (p->name() == "hour")
                                 {
                                     Serial.print("Received new hour setting: ");
                                     Serial.println(p->value());
                                     skyNetStruct->rtc_timestamp.hour = p->value().toInt();
                                     DS3231_set(skyNetStruct->rtc_timestamp);
                                     request->send_P(200, "text/plain", "OK set hour");
                                 }
                                 if (p->name() == "minute")
                                 {
                                     Serial.print("Received new minute setting: ");
                                     Serial.println(p->value());
                                     skyNetStruct->rtc_timestamp.min = p->value().toInt();
                                     DS3231_set(skyNetStruct->rtc_timestamp);
                                     request->send_P(200, "text/plain", "OK set minute");
                                 }
                                 if (p->name() == "day")
                                 {
                                     Serial.print("Received new day setting: ");
                                     Serial.println(p->value());
                                     skyNetStruct->rtc_timestamp.mday = p->value().toInt();
                                     DS3231_set(skyNetStruct->rtc_timestamp);
                                     request->send_P(200, "text/plain", "OK set day");
                                 }
                                 if (p->name() == "month")
                                 {
                                     Serial.print("Received new month setting: ");
                                     Serial.println(p->value());
                                     skyNetStruct->rtc_timestamp.mon = p->value().toInt();
                                     DS3231_set(skyNetStruct->rtc_timestamp);
                                     request->send_P(200, "text/plain", "OK set month");
                                 }
                                 if (p->name() == "year")
                                 {
                                     Serial.print("Received new year setting: ");
                                     Serial.println(p->value());
                                     auto year = p->value().toInt();
                                     if (year <= 1976 or year >= 9999)
                                     {
                                         Serial.print("Bad data received, filtering");

                                         request->send_P(400, "text/plain", "Year Not Implemented");
                                         return;
                                     }

                                     skyNetStruct->rtc_timestamp.year = year;
                                     DS3231_set(skyNetStruct->rtc_timestamp);
                                     request->send_P(200, "text/plain", "OK set year");
                                 }
                             });
    Serial.println(__LINE__);

    /**
   * @brief Set new weather request from users html.
   * 
   */
    skyNetStruct->server->on("/setCurrentSystemSystemWeather", HTTP_GET, [skyNetStruct](AsyncWebServerRequest *request)
                             {
                                 if (!skyNetStruct->is_server_ready)
                                 {
                                     Serial.println("Server not yet ready, returning");
                                     request->send_P(503, "text/plain", "Service Unavailable: /setCurrentSystemSystemWeather");
                                     return;
                                 }

                                 AsyncWebParameter *p = request->getParam(0);
                                 Serial.println("Got HTTP_GET on '/setCurrentSystemSystemWeather'");
                                 Serial.println(p->name());
                                 Serial.println(p->value());
                                 skyNetStruct->current_weather = Weather::to_weather(p->value());
                                 request->send_P(200, "text/plain", "OK set Weather");
                             });
    Serial.println(__LINE__);

    // Responce for getting current Weather.
    skyNetStruct->server->on("/getCurrentSystemWeather", HTTP_GET, [skyNetStruct](AsyncWebServerRequest *request)
                             {
                                 if (!skyNetStruct->is_server_ready)
                                 {
                                     Serial.println("Server not yet ready, returning");
                                     request->send_P(503, "text/plain", "Service Unavailable: /getCurrentSystemWeather");
                                     return;
                                 }
                                 request->send_P(200, "text/plain", Weather::to_string(skyNetStruct->current_weather).c_str());
                                 Serial.println("Got HTTP_GET on '/getCurrentSystemWeather'");
                             });
    Serial.println(__LINE__);

    // Responce for setting new leds brightness.
    skyNetStruct->server->on("/setLedsBrightness", HTTP_GET, [skyNetStruct](AsyncWebServerRequest *request)
                             {
                                 if (!skyNetStruct->is_server_ready)
                                 {
                                     Serial.println("Server not yet ready, returning");
                                     request->send_P(503, "text/plain", "Service Unavailable: /setLedsBrightness");
                                     return;
                                 }

                                 AsyncWebParameter *p = request->getParam(0);
                                 Serial.println("Got HTTP_GET on '/setLedsBrightness'");
                                 Serial.println(p->name());
                                 Serial.println(p->value());

                                 if (!skyNetStruct->preferences.begin("SkyNetCloud"))
                                 {
                                     Serial.println("Could not initiate 'preference', on setLedsBrightness. Aborting");
                                     // 500 Internal Server Error
                                     request->send_P(500, "text/plain", "Error, server could not open preferences storace object.");
                                     return;
                                 }

                                 ///< @brief Expected values between 0-255
                                 auto brightness = p->value().toInt();

                                 // Store new value EEPROM.
                                 skyNetStruct->preferences.putInt("LedBrightness", brightness);
                                 skyNetStruct->preferences.end();

                                 // Store new value in memory.
                                 skyNetStruct->ledsSettings.leds_brightness = brightness;
                                 skyNetStruct->ledsSettings.is_brightness_changed = true;

                                 request->send_P(200, "text/plain", "Brightness updated successfully");
                             });
    Serial.println(__LINE__);

    // Responce for getting current Weather.
    skyNetStruct->server->on("/getLedsBrightness", HTTP_GET, [skyNetStruct](AsyncWebServerRequest *request)
                             {
                                 if (!skyNetStruct->is_server_ready)
                                 {
                                     Serial.println("Server not yet ready, returning");
                                     request->send_P(503, "text/plain", "Service Unavailable: /getLedsBrightness");
                                     return;
                                 }

                                 Serial.println("Got HTTP_GET on '/getLedsBrightness'");
                                 if (!skyNetStruct->preferences.begin("SkyNetCloud"))
                                 {
                                     Serial.println("Could not initiate 'preference', on getLedsBrightness. Aborting");
                                     request->send_P(500, "text/plain", "Error, server could not open preferences storace object.");
                                     return;
                                 }
                                 skyNetStruct->ledsSettings.leds_brightness = skyNetStruct->preferences.getInt("LedBrightness");
                                 skyNetStruct->ledsSettings.is_brightness_changed = true;

                                 request->send_P(200, "text/plain", String(skyNetStruct->ledsSettings.leds_brightness).c_str());
                                 skyNetStruct->preferences.end();
                             });
    Serial.println(__LINE__);

    // Start server
    skyNetStruct->server->begin();
    Serial.println(__LINE__);

    /**
   * @brief Initialize SPIFFS
   * 
   */
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return false;
    }

    Serial.print("Done initiating: ");
    Serial.println(__PRETTY_FUNCTION__);
    skyNetStruct->is_server_ready = true;
    return true;
}