#include <Arduino.h>
#include <stdio.h> 
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include <WebServer.h>
#include "FS.h"
#include "SPIFFS.h"
#include "esp_spiffs.h"

#define LED_PIN 2

int nodeLength;
int color[100][3];

WebServer server(80);
Adafruit_NeoPixel pixels(nodeLength, LED_PIN, NEO_GRB + NEO_KHZ800);

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void configureNodes();
void sendSignal();
void handleRoot();
void handleNotFound();
void startPage();

void setup()
{
  Serial.begin(115200);

  configureNodes();
  pixels.updateLength(nodeLength);
  pixels.begin();
  sendSignal();

  WiFi.mode(WIFI_AP_STA);
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop()
{
  startPage();
  delay(1000);
}

//============================================== WEBPAGE HANDLE

void startPage()
{
  WiFi.softAP("RGBControl", "", 2);
  delay(500);
  Serial.println("Initialyzing AP...");
  while (1)
  {
    //webSocket.loop();
    server.handleClient();
  }
}

void handleRoot()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/index.html", "r");
  server.streamFile(f, "text/html");
  f.close();
  SPIFFS.end();
}

void handleNotFound()
{
  server.send(404, "text/plain", "404: Not found");
}

//============================================== SIGNAL HANDLE

void sendSignal()
{
  pixels.clear();
  for (int i = 0; i <= nodeLength; i++)
  {
    pixels.setPixelColor(i, pixels.Color(color[i][0], color[i][1], color[i][2]));
  }
  pixels.show();
}

//============================================== CONFIGURE NODES

void configureNodes()
{
  SPIFFS.begin();
  File rFile = SPIFFS.open("/config.txt","r");
  String content = rFile.readString();
  rFile.close();
  SPIFFS.end();
  
  StaticJsonDocument<512> jsonBuffer;
  deserializeJson(jsonBuffer, content);
  JsonObject jsonObject = jsonBuffer.as<JsonObject>();

  nodeLength = jsonObject["payload"]["size"];
  for (int i = 0; i < nodeLength; i++)
  {
    for(int n=0;n<3;n++){
      JsonVariant _color = jsonObject["payload"]["nodes"]["node_"+String(i)][String(n)];
      color[i][n] = _color.as<int>();
    }
  }
}