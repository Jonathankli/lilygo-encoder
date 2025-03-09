#include <Arduino.h>
#include <lvgl.h>
#include "lvgl_helper.h"

RGBColor lvgl_event_to_rgb(lv_event_t *e)
{
    lv_obj_t *colorwheel = lv_event_get_target(e); // Get color picker object
    lv_color_t color = lv_colorwheel_get_rgb(colorwheel); // Get selected color

    RGBColor rgb;
    rgb.r = (color.ch.red * 255) / 31;
    rgb.g = (((color.ch.green_h << 3) | color.ch.green_l) * 255) / 63;
    rgb.b = (color.ch.blue * 255) / 31;
    return rgb;
}

int lvgl_event_to_brightness(lv_event_t *e)
{
    lv_obj_t *arc = lv_event_get_target(e); // Get the arc object that triggered the event
    int brightness = lv_arc_get_value(arc);  // Get the arc's value (0-100)
    return map(brightness, 0, 100, 0, 255);
}
