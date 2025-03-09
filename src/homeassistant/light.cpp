
#include "ui.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <lvgl.h>
#include "homeassistant/websocket.h"
#include "utils/lvgl_helper.h"

void ha_light_change_brightness(String entity, int brightness)
{
    // Send brightness to Home Assistant (assuming sendWSMessage function)
    JsonDocument doc;
    doc["type"] = "call_service";
    doc["domain"] = "light";
    doc["service"] = "turn_on";
    doc["service_data"]["entity_id"] = entity;
    doc["service_data"]["brightness"] = brightness;
    haWebsocket.send(doc, true);
}

void ha_light_change_brightness(String entity, lv_event_t * e)
{   
    ha_light_change_brightness(entity, lvgl_event_to_brightness(e));
}

void ha_light_change_color(String entity, uint8_t r, uint8_t g, uint8_t b)
{
    // Send RGB color to Home Assistant
    JsonDocument doc;
    doc["type"] = "call_service";
    doc["domain"] = "light";
    doc["service"] = "turn_on";
    doc["service_data"]["entity_id"] = entity;
    doc["service_data"]["rgb_color"][0] = r;
    doc["service_data"]["rgb_color"][1] = g;
    doc["service_data"]["rgb_color"][2] = b;
    haWebsocket.send(doc, true);
}

void ha_light_change_color(String entity, lv_event_t * e)
{
    RGBColor rgb = lvgl_event_to_rgb(e);
    ha_light_change_color(entity, rgb.r, rgb.g, rgb.b);
}

void ha_light_toggle(String entity)
{
    JsonDocument doc;
    doc["type"] = "call_service";
    doc["domain"] = "light";
    doc["service"] = "toggle";
    doc["service_data"]["entity_id"] = entity;
    haWebsocket.send(doc, true);
}