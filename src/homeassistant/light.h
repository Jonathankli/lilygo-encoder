
#include "ui.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <lvgl.h>
#include "homeassistant/websocket.h"

void ha_light_change_brightness(String entity, int brightness);
void ha_light_change_brightness(String entity, lv_event_t * e);
void ha_light_change_color(String entity, uint8_t r, uint8_t g, uint8_t b);
void ha_light_change_color(String entity, lv_event_t * e);
void ha_light_toggle(String entity);