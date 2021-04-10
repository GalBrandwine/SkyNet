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
    // request->send(SPIFFS, "/cloud_1.jpg", String());
    request->send_P(200, "text/plain", skyNetStruct->time_str.c_str());
    Serial.println("Got HTTP_GET on '/getCurrentSystemTime'");
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