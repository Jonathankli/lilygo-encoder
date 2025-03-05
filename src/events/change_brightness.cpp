#include "ui.h"
#include "../homeassistant/websocket.h"
#include "../homeassistant/light.h"
#include <Arduino.h>
#include <ArduinoJson.h>

#define DEBOUNCE_TIME 333  // ms debounce time
unsigned long lastUpdateTimeBrightness = 0;

void change_brightness(lv_event_t * e)
{
	unsigned long currentTime = millis();
    
    if (currentTime - lastUpdateTimeBrightness < DEBOUNCE_TIME) {
        return;  // Ignore if within debounce time
    }
    
    lastUpdateTimeBrightness = currentTime;  // Update last update time
	ha_light_change_brightness(e, "light.eckstrahler_wohnzimmer");
}
