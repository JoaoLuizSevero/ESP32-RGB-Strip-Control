/*

#include <WebSocketsServer.h>

WebSocketsServer webSocket(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);


void setup()
{

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

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

    // if (jsonObject.containsKey("payload"))
    // {
    //   int len = measureJson(jsonObject);
    //   char buff[len];
    //   serializeJson(jsonObject, buff, len + 1);

    //   unlink("/config.txt");
    //   delay(100);
    //   FILE *f = fopen("/config.txt", "w");
    //   fprintf(f, buff);
    //   fclose(f);
    //   delay(100);
    //   configureNodes()
    // }
    // if (jsonObject.containsKey("getConfig"))
    // {
    //   DynamicJsonDocument nodesConfig(JSON_ARRAY_SIZE(512));

    //   nodesConfig["payload"]["size"] = nodes.size();
    //   for (int i = 0; i < nodes.size(); i++)
    //   {
    //     nodesConfig["payload"]["nodes"]["node_" + i]["color"] = node[i].first;
    //     nodesConfig["payload"]["nodes"]["node_" + i]["time"] = node[i].second;
    //   }
    //   int len = measureJson(nodesConfig);
    //   char _array[len];
    //   serializeJson(nodesConfig, _array, len + 1);
    //   webSocket.sendTXT(0, _array, strlen(_array));
    // }
    // break;
  }
}
*/