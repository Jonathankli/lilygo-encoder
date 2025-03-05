#include "ui.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../homeassistant/websocket.h"
#include "../homeassistant/light.h"

void power_toggle(lv_event_t * e)
{
	ha_light_toggle("light.eckstrahler_wohnzimmer");
}