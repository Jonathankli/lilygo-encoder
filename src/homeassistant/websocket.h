#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>

class HAWebsocket
{
public:
	HAWebsocket(bool withLog = false);
	void connect();
	void loop();
	int send(JsonDocument doc, bool withId = true);
	
private:
	bool withLog = false;
	int id = 1;
	WebSocketsClient webSocket;

	void authenticate();
	void onEvent(WStype_t type, uint8_t *payload, size_t length);
    void log(String message, ...);
};

extern HAWebsocket haWebsocket;

#endif // WEBSOCKET_H
