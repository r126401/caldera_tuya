


#include "thermostat_task.h"
#include "events_app.h"
#include "rgblcd.h"
#include "tal_system.h"
#include "tal_time_service.h"
#include "esp_log.h"
#include "events_app.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>


static char* TAG= "tuya_interface.h";
extern void tuya_app_main(void);
EventGroupHandle_t evt_between_task;


void get_date_app() {

    POSIX_TM_S tm;
    OPERATE_RET ret;

    ret = tal_time_get_local_time_custom(0, &tm);
    //ESP_LOGI(TAG, "La hora es %.02d:%.02d%:.02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
    ESP_LOGI(TAG, "Resultado: %d, la hora es: %.02d:%.02d:%.02d", ret, tm.tm_hour, tm.tm_min, tm.tm_sec);


}


void esp32_app_main() {


    evt_between_task = xEventGroupCreate();
    //tuya_app_main();
    //create_event_app_task();
    //create_task_thermostat();
    init_lcdrgb();
    xEventGroupWaitBits(evt_between_task, EVT_TUYA_TASK, pdFALSE, pdTRUE, portMAX_DELAY);
    tuya_app_main();
    create_event_app_task();
    create_task_thermostat();
    
    

    

}

void poner_semaforo() {

    xEventGroupSetBits(evt_between_task, EVT_EVENT_TASK);
    
}