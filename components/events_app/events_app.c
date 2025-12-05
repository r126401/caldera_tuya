

#include "events_app.h"

#include "thermostat_task.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include "esp_log.h"
#include "esp_err.h"
#include "alarms_app.h"
#include "events_lcd.h"


#include "lv_main_thermostat.h"
#include "lv_factory_thermostat.h"

xQueueHandle event_queue_app;
static const char *TAG = "events_app";
//extern esp_rmaker_device_t *thermostat_device;
extern float current_threshold;



#include <string.h>
#include "strings.h"
#include <stdio.h>


static void send_alarm(bool status) {

    set_lcd_update_icon_errors(status);
}


static void send_event_app_alarm_off() {

    send_alarm(false);

}

static void send_event_app_alarm_on() {

    send_alarm(true);

}



char* event_app_2mnemonic(EVENT_APP type) {

    static char mnemonic[30];
    switch (type) 
    {

        case EVENT_APP_SETPOINT_THRESHOLD:
            strncpy(mnemonic, "EVENT_APP_SETPOINT_THRESHOLD", 30);
        break;

        case EVENT_APP_TIME_VALID:
            strncpy(mnemonic, "EVENT_APP_TIME_VALID", 30);
        break;
        case EVENT_APP_AUTO:
            strncpy(mnemonic, "EVENT_APP_AUTO", 30);
        break;

        case EVENT_APP_MANUAL:
            strncpy(mnemonic, "EVENT_APP_MANUAL", 30);
        break;
        case EVENT_APP_ALARM_OFF:
            strncpy(mnemonic, "EVENT_APP_ALARM_OFF", 30);
        break;
        case EVENT_APP_ALARM_ON:
            strncpy(mnemonic, "EVENT_APP_ALARM_ON", 30);
        break;
        case EVENT_APP_FACTORY:
            strncpy(mnemonic, "EVENT_APP_FACTORY", 30);
        break;



    }


        return mnemonic;
}



void delay_get_schedules(void *arg) {

    int index;
    uint32_t time_end;
    float threshold;

    ESP_LOGW(TAG, "Vamos a chequear los schedules en la ultima tarea del arranque");

    index = get_next_schedule(&time_end);
    lv_update_schedule(true, time_end, index);
    set_app_status(STATUS_APP_AUTO);
    lv_enable_button_mode(true);
    //sacamos el ultimo schedule para poner el threshold correspndiente en el arranque.
    index = get_last_schedule(&time_end, &threshold);

    if (index >= 0) {
        set_app_update_threshold(threshold, true);
        set_lcd_update_threshold_temperature(threshold);
    }
   


}



void receive_event_app(event_app_t event) {

    status_app_t status_app = STATUS_APP_ERROR;


    switch (event.event_app) 
    {

        case EVENT_APP_SETPOINT_THRESHOLD:

            ESP_LOGI(TAG, "Recibido evento EVENT_APP_SETPOINT_THRESHOLD. Threshold = %.1f", event.value); 
            set_app_update_threshold(event.value, true);
            break;


        case EVENT_APP_TIME_VALID:
            status_app = get_app_status();

            if (status_app == STATUS_APP_UPGRADING) {
                ESP_LOGW(TAG, "No se actualiza nada porque estamos en upgrading");
            }

            if ((status_app == STATUS_APP_STARTING) || status_app == (STATUS_APP_CONNECTING) || status_app == (STATUS_APP_SYNCING)) {
                esp_timer_handle_t timer_delay_get_schedule;

                const esp_timer_create_args_t delay_get_schedule_shot_timer_args = {
                .callback = &delay_get_schedules,
                /* name is optional, but may help identify the timer when debugging */
                .name = "get schedule"
                };
                esp_timer_create(&delay_get_schedule_shot_timer_args, &timer_delay_get_schedule);
                esp_timer_start_once(timer_delay_get_schedule, 10 * 1000000);
            }

            
            break;

        case EVENT_APP_MANUAL:
            status_app = get_app_status();

            if (status_app == STATUS_APP_AUTO) {
                ESP_LOGW(TAG, "Vamos a cambiar al estado manual. Estamos en modo %s", status2mnemonic(status_app));
                set_app_status(STATUS_APP_MANUAL);
                if (get_status_relay() == OFF) {
                    ESP_LOGW(TAG, "Vamos a encender porque estaba apagado");
                    relay_operation(ON);
                } else {
                    ESP_LOGW(TAG, "Vamos a encender porque estaba encendido");
                    relay_operation(OFF);
                    }
                }

                lv_update_lcd_schedule(false);


            break;

        case EVENT_APP_AUTO:
            status_app = get_app_status();
            ESP_LOGW(TAG, "Vamos a cambiar al estado AUTO. Estamos en modo %s", status2mnemonic(status_app));

            if (status_app == STATUS_APP_MANUAL) {

                set_app_status(STATUS_APP_AUTO);
                set_lcd_update_threshold_temperature(current_threshold);
                lv_update_lcd_schedule(true);
                thermostat_action(get_app_current_temperature());

                
            }
        break;

        case EVENT_APP_ALARM_OFF:
            if (get_active_alarms() == 0) {

                send_event_app_alarm_off();


            }

        break;
        case EVENT_APP_ALARM_ON:
        
            if (get_active_alarms() == 0) {

                send_event_app_alarm_on();
            }

        break;

        case EVENT_APP_FACTORY:
            create_instalation_button();
            set_app_status(STATUS_APP_FACTORY);
            //inhibimos el boton mode para que no se pueda cambiar de modo
            lv_enable_button_mode(false);

        break;




    }

    //ESP_LOGW(TAG, "Retornamos despues de procesar la peticion");

}


void event_app_task(void *arg) {

	event_app_t event;


	event_queue_app = xQueueCreate(10, sizeof(event_app_t));

	for(;;) {
		ESP_LOGI(TAG, "ESPERANDO EVENTO DE APLICACION...Memoria libre: %d", (int) esp_get_free_heap_size());
		if (xQueueReceive(event_queue_app, &event,  portMAX_DELAY) == pdTRUE) {

			receive_event_app(event);


		} else {
			ESP_LOGE(TAG, "NO SE HA PODIDO PROCESAR LA PETICION");
		}

	}
	vTaskDelete(NULL);


}

void create_event_app_task() {



	xTaskCreatePinnedToCore(event_app_task, "event_app_task", CONFIG_RESOURCE_EVENT_TASK, NULL, 0, NULL,0);
	ESP_LOGW(TAG, "TAREA DE EVENTOS DE APLICACION CREADA CREADA");


}


static void send_event_app(event_app_t event) {


	ESP_LOGW(TAG, " envio de evento app %s", event_app_2mnemonic(event.event_app));
	if ( xQueueSend(event_queue_app, &event, 0) != pdPASS) {
		ESP_LOGE(TAG, "no se ha podido enviar el evento");

	}

}

void send_event_app_alarm(EVENT_APP alarm) {

    event_app_t event;
    event.event_app = alarm;



    switch (alarm) {

        case EVENT_APP_ALARM_ON:
        case EVENT_APP_ALARM_OFF:
            send_event_app(event);
        break;
        default:
            ESP_LOGE(TAG, "Error, no se trata aqui este tipo de alarma");
        break;
    }

}

void send_event_app_threshold(float threshold) {

    event_app_t event;
    event.event_app = EVENT_APP_SETPOINT_THRESHOLD;
    event.value = threshold;
    send_event_app(event);
}

void send_event_app_time_valid() {

    event_app_t event;
    event.event_app = EVENT_APP_TIME_VALID;
    send_event_app(event);

}

void send_event_app_status(EVENT_APP status)  {

    event_app_t event;
    event.event_app = status;
    switch (status) {

        case EVENT_APP_AUTO:
        case EVENT_APP_MANUAL:
        send_event_app(event);
        break;
        default:
            ESP_LOGE(TAG, "No se puede llamar a esta funcion para este evento");
        break;
    }
}

void send_event_app_factory() {

    event_app_t event;
    event.event_app = EVENT_APP_FACTORY;
    send_event_app(event);
}












