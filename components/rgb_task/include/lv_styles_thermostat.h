
#include "lvgl.h"
#define LV_COLOR_TEXT_NOTIFICATION 0x81D1F9
#define LV_COLOR_TEXT_FAIL_NOTIFICATION 0XFF0000
#define LV_COLOR_DATE 0XFFFFFF

extern lv_style_t style_layout_schedule;
extern lv_style_t style_layout_notification;
extern lv_style_t style_layout_temperature;
extern lv_style_t style_layout_threshold;
extern lv_style_t style_layout_buttons_threshold;
extern lv_style_t style_buttons_threshold;
extern lv_style_t style_buttons_threshold_pressed;
extern lv_style_t style_text_mode;
extern lv_style_t style_reset;
extern lv_style_t style_schedule;
extern lv_style_t style_ok;
extern lv_style_t style_fail;


LV_FONT_DECLARE(russo48);
LV_FONT_DECLARE(russo36);


void set_style_threshold();

void set_style_buttons();
void set_style_layout_schedule();
void set_style_layout_notification();
void set_style_layout_temperature();
void set_style_layout_button_threshold();
void set_style_buttons_threshold();
void set_style_reset();
void set_style_mode();
void style_status_ok();
void style_status_fail();
void set_style_schedule();