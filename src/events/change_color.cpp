#include "ui.h"
#include "../homeassistant/websocket.h"
#include "../homeassistant/light.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../utils/debouncer.h"
#include "../utils/lvgl_helper.h"

static Debounce debounce(250);

void change_color(lv_event_t * e)
{
	RGBColor rgb = lvgl_event_to_rgb(e);
    debounce.call([rgb]() {
		ha_light_change_color("light.eckstrahler_wohnzimmer", rgb.r, rgb.g, rgb.b);
	});
}
