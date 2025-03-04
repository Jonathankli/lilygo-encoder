#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <Arduino.h>
#include <ArduinoJson.h>

void connectWS();
void wsLoop();
void sendWSMessage(JsonDocument doc, boolean withId = true);

#endif // WEBSOCKET_H
