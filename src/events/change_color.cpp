#include "ui.h"
#include "../homeassistant/websocket.h"
#include "../homeassistant/light.h"
#include <Arduino.h>
#include <ArduinoJson.h>

#define DEBOUNCE_TIME 333  // ms debounce time
unsigned long lastUpdateTime = 0;

void change_color(lv_event_t * e)
{
	unsigned long currentTime = millis();
    
    if (currentTime - lastUpdateTime < DEBOUNCE_TIME) {
        return;  // Ignore if within debounce time
    }
    
    lastUpdateTime = currentTime;  // Update last update time
    
    ha_light_change_color(e, "light.eckstrahler_wohnzimmer");
}
