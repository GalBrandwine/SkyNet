#include <HTTPClient.h>
// #include <Preferences.h>
// #include "ESPAsyncWebServer.h"
// #include "SPIFFS.h"
// #include "RTCsettings.h"

/**
 * @brief Create and send HTTP.Get(current-system-time) to the SkyNetMaster
 * 
 * @param[out] client_time 
 */
void HTTPgetCurrentSystemTime(ts &client_time)
{
  HTTPClient http;

  /**
   * @brief Create system-time url
   * 
   */
  http.begin("http://192.168.4.1/getCurrentSystemTime");
  // start connection and send HTTP header
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0)
  {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("\n*************************************\n");
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();
      Serial.println(payload);

      TimeParsing::to_time(payload, client_time);
      Serial.printf("\n");
      Serial.printf("Parsed time: ");
      Serial.print(client_time.hour);
      Serial.printf(":");
      Serial.print(client_time.min);
      Serial.printf(":");
      Serial.print(client_time.sec);
      Serial.printf("\n*************************************\n");
    }
  }
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

/**
 * @brief Get from server current weather
 * 
 * @param sky_net 
 */
void HTTPgetCurrentSystemWeather(SkyNetStruct &sky_net)
{
  HTTPClient http;

  /**
   * @brief Create system-time url
   * 
   */
  http.begin("http://192.168.4.1/getCurrentSystemWeather");
  // start connection and send HTTP header
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0)
  {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("\n*************************************\n");
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();
      Serial.println(payload);

      /**
       * @brief Interprate current wheather
       */
      sky_net.current_weather = Weather::to_weather(payload);
      Serial.printf("Got weather from server:\n");
      Serial.print(Weather::to_string(sky_net.current_weather));
      Serial.printf("\n*************************************\n");
    }
  }
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void HTTPgetCurrentSystemLedBrightness(SkyNetStruct &skyNet)
{
  HTTPClient http;

  /**
   * @brief Create system-time url
   * 
   */
  http.begin("http://192.168.4.1/getLedsBrightness");
  // start connection and send HTTP header
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0)
  {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("\n*************************************\n");
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();
      Serial.println(payload);

      /**
       * @brief Interprate current led brightness
       * 
       * @note Brightness can vary between 0-255
       */
      if (skyNet.ledsSettings.leds_brightness != payload.toInt())
      {
        Serial.printf("Brightness has changed to : ");
        Serial.println(payload);
        Serial.printf("\nUpdating");
        skyNet.ledsSettings.leds_brightness = payload.toInt();
        skyNet.ledsSettings.is_brightness_changed = true;
      }
      else
        Serial.printf("Brightness hasn't changed. Doing nothing");
        
      Serial.printf("\n*************************************\n");
    }
  }
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}