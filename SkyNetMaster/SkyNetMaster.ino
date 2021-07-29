
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

/**
 * @file SkyNetMaster.ino
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-03-28
 * 
 * @copyright Copyright (c) 2021
 * 
 * @dependencies:
 * * Git Clone these into ~/Arduino/libraries/
 * * * ESPAsyncWebServer:           https://github.com/me-no-dev/ESPAsyncWebServer
 * * * * Depends on AsyncTCP:       https://github.com/me-no-dev/AsyncTCP.git
 * * * SPIFFS:                      https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/
 * * * OTA example:                 https://lastminuteengineers.com/esp32-ota-updates-arduino-ide/
 * * * Using the RTC                https://create.arduino.cc/projecthub/MisterBotBreak/how-to-use-a-real-time-clock-module-ds3231-bc90fe
 * * * * Depending on ds3231:       https://github.com/rodan/ds3231.git
 * * * FastLed (WS2812B led driver) https://github.com/FastLED/FastLED.git
 * * * Preference:                  https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
 */

#include <SPIFFS.h>
#include <ArduinoOTA.h>

#include "/home/gal/dev/SkyNet/include/settings.h"
#include "/home/gal/dev/SkyNet/include/setUpServer.h"
#include "/home/gal/dev/SkyNet/include/RTCsettings.h"
#include "/home/gal/dev/SkyNet/include/LEDsettings.h"
#include "/home/gal/dev/SkyNet/include/Weathersettings.h"

int lighting_counter = 0;
int lightning_modulo_base = random(5, 10);
SkyNetStruct skyNet;
AsyncWebServer server(80);

void initAccessPoint()
{
    Serial.println();
    Serial.println("Configuring access point...");
    // digitalWrite(2, HIGH);
    // You can remove the password parameter if you want the AP to be open.
    auto value = WiFi.disconnect(false, true);

    WiFi.softAP(ssidAP, passwordAP);
    Serial.println(WiFi.softAPSSID());

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    // Print ESP32 Local IP Address
    Serial.println(WiFi.localIP());
    // digitalWrite(2, LOW);
}

/**
 * @brief Connect to existing wifi newtwork
 * 
 */
void initExistingWifiConnection()
{
    // We start by connecting to a WiFi network
    WiFi.begin(ssidWifi, passwordWifi);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void setup()
{
    Serial.begin(115200);
    pinMode(BUILTIN_LED, OUTPUT); // set the LED pin mode

    /**
   * @brief Init Leds
   * 
   */
    Serial.println("Initiating leds");
    digitalWrite(BUILTIN_LED, HIGH);
    initLeds(skyNet);
    digitalWrite(BUILTIN_LED, LOW);
    Serial.println("Leds initiated");

    /**
   * @brief CloudMaster should create an AP.
   */
#ifdef IM_MASTER
    skyNet.server = &server;

    initAccessPoint();

    if (!setUpServer(&skyNet))
    {
        showProgramError(3);
        Serial.println("Could not initiate 'setUpServer'. Resetting");
        ESP.restart();
    }

#endif IM_MASTER

    /**
   * @brief Cloud slave should connect to existing SkyNetWifi.
   */
#ifdef IM_SLAVE
    initExistingWifiConnection();
#endif IM_SLAVE

    /**
   * @brief I2C initiation
   * 
   */
    if (!Wire.begin())
    {
        showProgramError(3);
        Serial.println("Could not initiate 'Wire'. Resetting");
        ESP.restart();
    }

    DS3231_init(DS3231_CONTROL_INTCN);

    /**
   * @brief ****************************** OTA ************************
   * * Keep this part at the end of the INIT()
   * * Dont change  here nothing.
   */
    ArduinoOTA
        .onStart([]()
                 {
                     String type;
                     if (ArduinoOTA.getCommand() == U_FLASH)
                         type = "sketch";
                     else // U_SPIFFS
                         type = "filesystem";

                     // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                     SPIFFS.end();
                     Serial.println("Start updating " + type);
                 })
        .onEnd([]()
               { Serial.println("\nEnd"); })
        .onProgress([](unsigned int progress, unsigned int total)
                    { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
        .onError([](ota_error_t error)
                 {
                     Serial.printf("Error[%u]: ", error);
                     if (error == OTA_AUTH_ERROR)
                         Serial.println("Auth Failed");
                     else if (error == OTA_BEGIN_ERROR)
                         Serial.println("Begin Failed");
                     else if (error == OTA_CONNECT_ERROR)
                         Serial.println("Connect Failed");
                     else if (error == OTA_RECEIVE_ERROR)
                         Serial.println("Receive Failed");
                     else if (error == OTA_END_ERROR)
                         Serial.println("End Failed");
                 });

    ArduinoOTA.begin();
}

void loop()
{
    ArduinoOTA.handle();

    /**
   * @brief If I'm CloudMaster
   * 
   */
    DS3231_get(&skyNet.rtc_timestamp);
    TimeParsing::to_string(skyNet.rtc_timestamp, skyNet.time_str);
    Serial.print("\n");
    Serial.print(skyNet.time_str);
    Serial.print("\n");
    delay(1000);

    /**
   * @brief All clouds have this
   * 
   */
    if (skyNet.ledsSettings.is_brightness_changed)
        setBrightness(skyNet);
    showProgramTimeOfDay(skyNet);

    skyNet.current_weather = Weather::get_next_weather(skyNet.rtc_timestamp);
    Serial.print("Skynet current weather: ");
    Serial.println(Weather::to_string(skyNet.current_weather));

    switch (skyNet.current_weather)
    {
    case Weather::WeatherType::Stormy:
        lighting_counter++;
        if ((lighting_counter % lightning_modulo_base) == 0)
        {
            Serial.println("******* Doing lightning ******");
            doLightning(random(5) + 1, random(50, 500) + 1);
        }
        break;
    }
}