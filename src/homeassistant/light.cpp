
#include "ui.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <lvgl.h>
#include "homeassistant/websocket.h"

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

void ha_light_change_brightness(lv_event_t * e, String entity)
{   
    lv_obj_t * arc = lv_event_get_target(e); // Get the arc object that triggered the event
    int brightness = lv_arc_get_value(arc);  // Get the arc's value (0-100)

    int ha_brightness = map(brightness, 0, 100, 0, 255);
    
    ha_light_change_brightness(entity, ha_brightness);
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

void ha_light_change_color(lv_event_t * e, String entity)
{
    lv_obj_t * colorwheel = lv_event_get_target(e); // Get color picker object
    lv_color_t color = lv_colorwheel_get_rgb(colorwheel); // Get selected color

    // Convert LVGL color to RGB values
    uint8_t r = (color.ch.red * 255) / 31;   // Scale 5-bit to 8-bit (0-31 → 0-255)
    uint8_t g = (((color.ch.green_h << 3) | color.ch.green_l) * 255) / 63; // Combine green_h & green_l, then scale
    uint8_t b = (color.ch.blue * 255) / 31;  // Scale 5-bit to 8-bit (0-31 → 0-255)

    ha_light_change_color(entity, r, g, b);
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