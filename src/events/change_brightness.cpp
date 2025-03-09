#include "ui.h"
#include "../homeassistant/websocket.h"
#include "../homeassistant/light.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../utils/debouncer.h"
#include "../utils/lvgl_helper.h"

static Debounce debounce(250);

void change_brightness(lv_event_t * e)
{
	int brightness = lvgl_event_to_brightness(e);
	debounce.call([brightness]() {
		ha_light_change_brightness("light.eckstrahler_wohnzimmer", brightness);
	});
}