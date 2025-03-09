#ifndef LVGL_HELPER_H
#define LVGL_HELPER_H

#include <Arduino.h>
#include <lvgl.h>

struct RGBColor
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

RGBColor lvgl_event_to_rgb(lv_event_t *e);

int lvgl_event_to_brightness(lv_event_t *e);

#endif // LVGL_HELPER_H
