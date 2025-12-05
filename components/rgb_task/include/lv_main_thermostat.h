




#include "lv_styles_thermostat.h"

void create_main_thermostat();
void lv_update_time(int hour, int minute);
void lv_update_text_mode(char *mode);
void lv_update_label_mode(char *status);
void lv_update_temperature(float temperature);
void lv_update_threshold_temperature(float threshold);
void lv_update_wifi_status(bool status);
void lv_update_broker_status(bool status);
void lv_update_heating(bool status);
void lv_update_schedule(bool show, int max, int index);
void lv_update_icon_errors(bool errors);
void lv_update_text_schedule(int min, int max);
void lv_update_percent(int cursor);
void create_instalation_button();
int lv_update_lcd_schedule(bool status);
void lv_upgrade_firmware(char* message, int cursor);
void lv_enable_button_mode(bool enable);
void lv_set_button_instalation(bool show);
void lv_create_device_name();

