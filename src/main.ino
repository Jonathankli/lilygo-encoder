#include <Arduino.h>
#include <lvgl.h>
#include <ui.h>
#include "Arduino_GFX_Library.h"
#include "TouchDrvCHSC5816.hpp"
#include "pin_config.h"
#include "knob.h"
#include "homeassistant/websocket.h"
#include "knob_events.h"
#include "utils/debouncer.h"

// https://www.youtube.com/nishad2m8
// https://buymeacoffee.com/nishad2m8

static size_t Window_CycleTime1 = 0;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;

// Display-specific settings
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    LCD_CS /* CS */, LCD_SCLK /* SCK */, LCD_SDIO0 /* SDIO0 */,
    LCD_SDIO1 /* SDIO1 */, LCD_SDIO2 /* SDIO2 */, LCD_SDIO3 /* SDIO3 */);

Arduino_GFX *gfx = new Arduino_SH8601(bus, LCD_RST /* RST */, 0 /* rotation */, false /* IPS */, LCD_WIDTH, LCD_HEIGHT);

TouchDrvCHSC5816 touch;
TouchDrvInterface *pTouch;

/* Initialize CHSC5816 Touch Driver */
void CHSC5816_Initialization(void)
{
    touch.setPins(TOUCH_RST, TOUCH_INT);
    if (!touch.begin(Wire, CHSC5816_SLAVE_ADDRESS, IIC_SDA, IIC_SCL))
    {
        Serial.println("Failed to find CHSC5816 - check your wiring!");
        while (1)
        {
            delay(1000);
        }
    }
    Serial.println("Init CHSC5816 Touch device success!");
}

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
    gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

    lv_disp_flush_ready(disp); // Notify LVGL that the flushing is complete
}

/* Read the touchpad */
void my_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    int16_t Touch_x[2], Touch_y[2];
    uint8_t touchpad = touch.getPoint(Touch_x, Touch_y);

    if (touchpad > 0)
    {
        data->state = LV_INDEV_STATE_PR; // Touch pressed
        data->point.x = Touch_x[0];      // X coordinate
        data->point.y = Touch_y[0];      // Y coordinate
    }
    else
    {
        data->state = LV_INDEV_STATE_REL; // Touch released
    }
}

/* LVGL initialization */
void lvgl_initialization(void)
{
    lv_init(); // Initialize LVGL

    /* Set display dimensions */
    uint16_t lcdWidth = gfx->width();
    uint16_t lcdHeight = gfx->height();

    /* Allocate draw buffer */
    disp_draw_buf = (lv_color_t *)malloc(sizeof(lv_color_t) * lcdWidth * 40);
    if (!disp_draw_buf)
    {
        Serial.println("LVGL draw buffer allocation failed!");
        return;
    }

    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, lcdWidth * 40);

    /* Initialize display driver */
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = lcdWidth;
    disp_drv.ver_res = lcdHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /* Initialize input (touch) driver */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Ciallo"); // Initialization message

    /* Hardware setup */
    pinMode(LCD_VCI_EN, OUTPUT);
    digitalWrite(LCD_VCI_EN, HIGH); // Enable display power

    CHSC5816_Initialization(); // Initialize the touch driver

    gfx->begin(40000000);   // Initialize the display
    gfx->fillScreen(BLACK); // Clear the display

    /* Gradually increase display brightness */
    for (int i = 0; i <= 255; i++)
    {
        gfx->Display_Brightness(i);
        delay(3);
    }

    lvgl_initialization(); // Initialize LVGL

    ui_init();

    KNOB_Init();

    haWebsocket.connect(); // Connect to the WebSocket server
}

void loop()
{
    lv_timer_handler(); // Let the LVGL library handle GUI tasks
    delay(5);           // Add a small delay to reduce CPU usage

    KNOB_Logical_Scan_Loop(); // KNOB logical scan loop

    // Call the knob scanning function periodically (every 20ms)
    if (millis() > KNOB_CycleTime)
    {
        KNOB_Logical_Scan_Loop();
        KNOB_CycleTime = millis() + 20;
    }

    // Check if the knob has been triggered (turned)
    if (KNOB_Trigger_Flag == true)
    {
        KNOB_Trigger_Flag = false;

        // Update the knob data based on the knob state
        switch (KNOB_State_Flag)
        {
        case KNOB_State::KNOB_INCREMENT:
            KNOB_Data++;
            on_knob_increment(); // Custom function
            break;
        case KNOB_State::KNOB_DECREMENT:
            KNOB_Data--;
            on_knob_decrement(); // Custom function
            break;
        default:
            break;
        }

        on_knob_turn(KNOB_Data); // Custom function
    }

    haWebsocket.loop(); // Maintain the WebSocket connection
    Debounce::updateAll(); // Update all debouncers
}