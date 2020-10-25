#include <Arduino.h>
#include <stdio.h> 
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include "FS.h"
#include "SPIFFS.h"
#include "esp_spiffs.h"

#define LED_PIN 2

int nodeLength = 10;
int color[100][3];

WebServer server(80);
WebSocketsServer webSocket(81);
Adafruit_NeoPixel pixels(nodeLength, LED_PIN, NEO_GRB + NEO_KHZ800);
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
void configureNodes();
void sendSignal();
void handleRoot();
void handleNotFound();
void startPage();

void setup()
{
  Serial.begin(115200);
  
  pixels.begin();
  configureNodes();
  
  //pixels.updateLength(nodeLength);

  //sendSignal();

  WiFi.mode(WIFI_AP_STA);
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
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
    webSocket.loop();
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

//============================================== WEBSOCKET HANDLE

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    break;
  case WStype_CONNECTED:
  {
    IPAddress ip = webSocket.remoteIP(num);
  }
  break;
  case WStype_TEXT:

    StaticJsonDocument<512> jsonBuffer;
    deserializeJson(jsonBuffer, payload);
    JsonObject jsonObject = jsonBuffer.as<JsonObject>();

    if (jsonObject.containsKey("payload"))
    {
      int len = measureJson(jsonObject);
      char buff[len];
      serializeJson(jsonObject, buff, len + 1);

      SPIFFS.begin();
      SPIFFS.remove("/config.txt");
      delay(100);
      File rFile = SPIFFS.open("/config.txt","w+");
      rFile.print(buff);
      rFile.close();
      SPIFFS.end();
      delay(100);

      configureNodes();
    }

    if (jsonObject.containsKey("getConfig"))
    {
      DynamicJsonDocument nodesConfig(JSON_ARRAY_SIZE(512));

      nodesConfig["payload"]["size"] = nodeLength;
      for (int i = 0; i < nodeLength; i++)
      {
        nodesConfig["payload"]["nodes"]["node_" + String(i)]["r"] = color[i][0];
        nodesConfig["payload"]["nodes"]["node_" + String(i)]["g"] = color[i][1];
        nodesConfig["payload"]["nodes"]["node_" + String(i)]["b"] = color[i][2];
      }

      int len = measureJson(nodesConfig);
      char _array[len];
      serializeJson(nodesConfig, _array, len + 1);

      Serial.print("SENDING NODES: ");
      Serial.println(_array);

      webSocket.sendTXT(0, _array, strlen(_array));
    }
    break;
  }
}

//============================================== SIGNAL HANDLE

void sendSignal()
{
  pixels.clear();
  for (int i = 0; i <= nodeLength-1; i++)
  {
    pixels.setPixelColor(i, pixels.Color(color[i][0], color[i][1], color[i][2]));
  }
  pixels.show();
}

//============================================== CONFIGURE NODES

void configureNodes()
{
  pixels.clear();

  SPIFFS.begin();
  File rFile = SPIFFS.open("/config.txt","r");
  String content = rFile.readString();
  rFile.close();
  SPIFFS.end();
  
  Serial.print("LOADING NODES: ");
  Serial.println(content);

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
  sendSignal();
}