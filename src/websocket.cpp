#include <WiFi.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include "secrets.h"

WiFiClient client;
WebSocketsClient webSocket;
int id = 1;

int sendWSMessage(JsonDocument doc, boolean withId = true)
{
  int newId = -1;
  if(withId)
  {
    newId = ++id;
    doc["id"] = newId;
  }

  char output[256];
	serializeJson(doc, output);
  Serial.printf("Sending message: %s\n", output);
  webSocket.sendTXT(output); 
  return newId; 
}

void authenticate()
{
  JsonDocument auth;
  auth["type"] = "auth";
  auth["access_token"] = HA_ACCESS_TOKEN;
  sendWSMessage(auth, false);
}


// Event handler for WebSocket events
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
	switch (type)
	{
	case WStype_DISCONNECTED:
		Serial.println("Disconnected from WebSocket server");
		break;
	case WStype_CONNECTED:
		Serial.println("Connected to WebSocket server");
    authenticate();
		break;
	case WStype_TEXT:
		Serial.printf("Message from server: %s\n", payload);
		break;
	default:
		break;
	}
}

void connectWS()
{

	// Connect to Wi-Fi
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(1000);
		Serial.println("Connecting to WiFi...");
	}
	Serial.println("Connected to WiFi");

	// Connect to WebSocket server
	webSocket.beginSSL("ha.jkli.de", 443, "/api/websocket");

	// Add event handler
	webSocket.onEvent(webSocketEvent);
}

void wsLoop()
{
	// Maintain WebSocket connection and check for events
	webSocket.loop();
}