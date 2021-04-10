/*
 WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 5

 created for arduino 25 Nov 2012
 by Tom Igoe

ported for sparkfun esp32 
31.01.2017 by Jan Hendrik Berlin

 */

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
 */

#include <SPIFFS.h>
#include <ArduinoOTA.h>

#include "/home/gal/dev/SkyNet/include/settings.h"
#include "/home/gal/dev/SkyNet/include/setUpServer.h"
#include "/home/gal/dev/SkyNet/include/RTCsettings.h"
#include "/home/gal/dev/SkyNet/include/LEDsettings.h"

SkyNetStruct skyNet;
AsyncWebServer server(80);

void initAccessPoint()
{
  Serial.println();
  Serial.println("Configuring access point...");
  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssidAP, passwordAP);
  Serial.println(WiFi.softAPSSID());

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
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
   * @brief Choose between Wifi or AP
   * 
   */
  initAccessPoint();
  // initExistingWifiConnection();

  skyNet.server = &server;

  if (setUpServer(&skyNet))
    digitalWrite(LED_BUILTIN, HIGH);
  else
    digitalWrite(LED_BUILTIN, LOW);

  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);

  Serial.println("Initiating leds");
  initLeds();
  Serial.println("Leds initiated");

  /**
   * @brief ****************************** OTA ************************
   * * Keep this part at the end of the INIT()
   * * Dont change  here nothing.
   */
  ArduinoOTA
      .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else // U_SPIFFS
          type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        SPIFFS.end();
        Serial.println("Start updating " + type);
      })
      .onEnd([]() {
        Serial.println("\nEnd");
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      })
      .onError([](ota_error_t error) {
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

  DS3231_get(&rtc_timestamp);
  time_to_string(rtc_timestamp, skyNet.time_str);
  Serial.println(skyNet.time_str);
  delay(1000);

  showProgramTimeOfDay(rtc_timestamp);
  // showProgramRandom(10, 1000);

  // WiFiClient client = server.available(); // listen for incoming clients

  // if (client)
  // {                                // if you get a client,
  //   Serial.println("New Client."); // print a message out the serial port
  //   String currentLine = "";       // make a String to hold incoming data from the client
  //   while (client.connected())
  //   { // loop while the client's connected
  //     if (client.available())
  //     {                         // if there's bytes to read from the client,
  //       char c = client.read(); // read a byte, then
  //       Serial.write(c);        // print it out the serial monitor
  //       if (c == '\n')
  //       { // if the byte is a newline character

  //         // if the current line is blank, you got two newline characters in a row.
  //         // that's the end of the client HTTP request, so send a response:
  //         if (currentLine.length() == 0)
  //         {
  //           // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  //           // and a content-type so the client knows what's coming, then a blank line:
  //           client.println("HTTP/1.1 200 OK");
  //           client.println("Content-type:text/html");
  //           client.println();

  //           // the content of the HTTP response follows the header:
  //           client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
  //           client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");

  //           // The HTTP response ends with another blank line:
  //           client.println();
  //           // break out of the while loop:
  //           break;
  //         }
  //         else
  //         { // if you got a newline, then clear currentLine:
  //           currentLine = "";
  //         }
  //       }
  //       else if (c != '\r')
  //       {                   // if you got anything else but a carriage return character,
  //         currentLine += c; // add it to the end of the currentLine
  //       }

  //       // Check to see if the client request was "GET /H" or "GET /L":
  //       if (currentLine.endsWith("GET /H"))
  //       {
  //         digitalWrite(5, HIGH); // GET /H turns the LED on
  //       }
  //       if (currentLine.endsWith("GET /L"))
  //       {
  //         digitalWrite(5, LOW); // GET /L turns the LED off
  //       }
  //     }
  //   }
  //   // close the connection:
  //   client.stop();
  //   Serial.println("Client Disconnected.");
  // }
}
