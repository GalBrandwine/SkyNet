#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
bool setUpServer(SkyNetStruct *skyNetStruct)
{
  // Route for root / web page
  skyNetStruct->server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", String(), false);
    Serial.println("Got HTTP_GET on '/'");
  });

  // Route to load style.css file
  skyNetStruct->server->on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/style.css", "text/css");
    Serial.println("Got HTTP_GET on '/style.css'");
  });

  // Route to load clout_1.jpg image
  skyNetStruct->server->on("/cloud_1.jpg", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/cloud_1.jpg", String());
    Serial.println("Got HTTP_GET on '/cloud_1.jpg'");
  });

  // Responce for getting current system time.
  skyNetStruct->server->on("/getCurrentSystemTime", HTTP_GET, [skyNetStruct](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", skyNetStruct->time_str.c_str());
    Serial.println("Got HTTP_GET on '/getCurrentSystemTime'");
  });

  /**
   * @brief Responce for setting current system time.
   * Time setting could come with 2 different parameters ans values:
   * Hour:int
   * Minute:int
   */
  skyNetStruct->server->on("/setCurrentSystemTime", HTTP_GET, [skyNetStruct](AsyncWebServerRequest *request) {
    // request->send_P(200, "text/plain", skyNetStruct->time_str.c_str());
    AsyncWebParameter *p = request->getParam(0);
    Serial.println("Got HTTP_GET on '/setCurrentSystemTime'");
    if (p->name() == "hour")
    {
      Serial.print("Received new hour setting: ");
      Serial.println(p->value());
      skyNetStruct->rtc_timestamp.hour = p->value().toInt();
      DS3231_set(skyNetStruct->rtc_timestamp);
    }
    if (p->name() == "minute")
    {
      Serial.print("Received new minute setting: ");
      Serial.println(p->value());
      skyNetStruct->rtc_timestamp.min = p->value().toInt();
      DS3231_set(skyNetStruct->rtc_timestamp);
    }
    if (p->name() == "day")
    {
      Serial.print("Received new day setting: ");
      Serial.println(p->value());
      skyNetStruct->rtc_timestamp.mday = p->value().toInt();
      DS3231_set(skyNetStruct->rtc_timestamp);
    }
    if (p->name() == "month")
    {
      Serial.print("Received new month setting: ");
      Serial.println(p->value());
      skyNetStruct->rtc_timestamp.mon = p->value().toInt();
      DS3231_set(skyNetStruct->rtc_timestamp);
    }
  });

  /**
   * @brief Set new weather request from users html.
   * 
   */
  skyNetStruct->server->on("/setCurrentSystemSystemWeather", HTTP_GET, [skyNetStruct](AsyncWebServerRequest *request) {
    AsyncWebParameter *p = request->getParam(0);
    Serial.println("Got HTTP_GET on '/setCurrentSystemSystemWeather'");
    Serial.println(p->name());
    Serial.println(p->value());
    skyNetStruct->current_weather = Weather::to_weather(p->value());
  });

  // Responce for getting current Weather.
  skyNetStruct->server->on("/getCurrentSystemWeather", HTTP_GET, [skyNetStruct](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", Weather::to_string(skyNetStruct->current_weather).c_str());
    Serial.println("Got HTTP_GET on '/getCurrentSystemWeather'");
  });

  /**
   * @brief This is how the client pass information back to the server.
   * 
   */
  // server.on("/runtime", HTTP_GET, [](AsyncWebServerRequest *request) {
  //   // Take 'runtime' parameter from the request object.
  //   AsyncWebParameter *p = request->getParam(0);
  //   processRuntimeRequest(p);

  /**
   * @brief Send back to client "OK" message, and the desired value (as a confirmation)
   * 
   */
  //   if (runtime == RUNTIME_OFF_VALUE)
  //   {
  //     request->send_P(200, "text/plain", "off");
  //   }
  //   else
  //   {
  //     request->send_P(200, "text/plain", String(runtime).c_str());
  //   }
  // });

  // server.on("/frequency", HTTP_GET, [](AsyncWebServerRequest *request) {
  //   // Take 'runtime' parameter from the request object.
  //   AsyncWebParameter *p = request->getParam(0);
  //   processFrequencyRequest(p);
  //   request->send_P(200, "text/plain", String(frequency_hours).c_str());
  // });

  // server.on("/isSystemWatering", HTTP_GET, [](AsyncWebServerRequest *request) {
  //   request->send_P(200, "text/plain", String(isSystemWatering).c_str());
  // });

  /**
   * @brief Simple where the client is sending GET request, and the server send back as a respond information.
   * 
   */
  // server.on("/getFrequency", HTTP_GET, [](AsyncWebServerRequest *request) {
  //   request->send_P(200, "text/plain", String(frequency_hours).c_str());
  // });

  // server.on("/getRunTime", HTTP_GET, [](AsyncWebServerRequest *request) {
  //   if (runtime == RUNTIME_ON_VALUE)
  //   {
  //     request->send_P(200, "text/plain", "on");
  //   }
  //   else if (runtime == RUNTIME_OFF_VALUE)
  //   {
  //     request->send_P(200, "text/plain", "off");
  //   }
  //   else
  //   {
  //     request->send_P(200, "text/plain", String(runtime).c_str());
  //   }
  // });

  // server.on("/getTimeLeft", HTTP_GET, [](AsyncWebServerRequest *request) {
  //   request->send_P(200, "text/plain", String(time_left_until_next_watering_min).c_str());
  // });

  // Start server
  skyNetStruct->server->begin();

  /**
   * @brief Initialize SPIFFS
   * 
   */
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return false;
  }

  Serial.println("Server started");
  return true;
}