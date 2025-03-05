#include <WiFi.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include "secrets.h"
#include "websocket.h"

HAWebsocket haWebsocket(true);

HAWebsocket::HAWebsocket(bool withLog)
{
	this->withLog = withLog;
}

void HAWebsocket::connect()
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
	webSocket.beginSSL(HA_DOMAIN, 443, "/api/websocket");

	// Add event handler
	webSocket.onEvent([this](WStype_t type, uint8_t *payload, size_t length) { this->onEvent(type, payload, length); });
}

int HAWebsocket::send(JsonDocument doc, bool withId)
{
	int newId = -1;
	if (withId)
	{
		newId = ++id;
		doc["id"] = newId;
	}

	char output[256];
	serializeJson(doc, output);
	log("Sending message: %s\n", output);
	webSocket.sendTXT(output);
	return newId;
}

void HAWebsocket::authenticate()
{
	JsonDocument auth;
	auth["type"] = "auth";
	auth["access_token"] = HA_ACCESS_TOKEN;
	this->send(auth, false);
}

// Event handler for WebSocket events
void HAWebsocket::onEvent(WStype_t type, uint8_t *payload, size_t length)
{
	switch (type)
	{
	case WStype_DISCONNECTED:
		log("Disconnected from WebSocket server");
		break;
	case WStype_CONNECTED:
		log("Connected to WebSocket server");
		authenticate();
		break;
	case WStype_TEXT:
		log("Message from server: %s\n", payload);
		break;
	default:
		break;
	}
}

void HAWebsocket::loop()
{
	webSocket.loop();
}

void HAWebsocket::log(String message, ...)
{
	if (!withLog)
	{
		return;
	}

	Serial.print("[HAWebsocket] ");
	va_list args;
	va_start(args, message);
	vprintf(message.c_str(), args);
	va_end(args);

	Serial.println();
}