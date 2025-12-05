#pragma once

#include <string.h>
#include "strings.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_sntp.h>
#include <esp_timer.h>
#include <esp_err.h>


#include "esp_err.h"
#include "esp_rmaker_core.h"
#include <esp_rmaker_standard_types.h>
#include <esp_rmaker_standard_params.h>
#include <esp_rmaker_standard_devices.h>
#include <esp_rmaker_schedule.h>
#include <esp_rmaker_scenes.h>
#include <esp_rmaker_console.h>
#include <esp_rmaker_ota.h>
#include <esp_rmaker_utils.h>

#include "esp_rmaker_schedule.h"




typedef enum EVENT_APP {

    EVENT_APP_SETPOINT_THRESHOLD,
    EVENT_APP_TIME_VALID,
    EVENT_APP_AUTO,
    EVENT_APP_MANUAL,
    EVENT_APP_ALARM_ON,
    EVENT_APP_ALARM_OFF,
    EVENT_APP_FACTORY,

}EVENT_APP;

typedef struct event_app_t {

    float value;
    EVENT_APP event_app;
} event_app_t;


void create_event_app_task();
//void send_event_app(event_app_t event);
void send_event_app_threshold(float threshold);
void send_event_app_alarm(EVENT_APP type);
void send_event_app_time_valid();
void send_event_app_status(EVENT_APP status);
void send_event_app_factory();







esp_err_t write_cb(const esp_rmaker_device_t *device, const esp_rmaker_param_t *param,
            const esp_rmaker_param_val_t val, void *priv_data, esp_rmaker_write_ctx_t *ctx);
