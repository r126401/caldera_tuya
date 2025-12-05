#include "lvgl.h"
#include <string.h>
#include <esp_log.h>

#include "lv_factory_thermostat.h"
#include "lv_main_thermostat.h"
#include "lv_styles_thermostat.h"
//#include "app_main.h"


#include "esp_timer.h"

extern lv_style_t style_buttons;
extern lv_display_t * display;

lv_style_t style_titles;
extern lv_obj_t *screen_main_thermostat;

static esp_timer_handle_t timer_factory;



lv_obj_t *screen_init_thermostat;
lv_obj_t *button_reset;
lv_obj_t *button_home;
lv_obj_t *label_button_reset;
lv_obj_t *label_button_home;
lv_obj_t *qr;
lv_obj_t *title_aplication;
lv_span_t *span;
lv_obj_t * spans;

extern char *text_qrcode;
static const char *TAG = "lv_factory_thermostat";
//extern _lock_t lock_lv;

#define TIMING_FACTORY 180000000
#define TEXT_INSTRUCTION "Escanea el codigo QR para enlazar el dispositivo con la aplicacion. (3 min.)"


void set_style_titles() {

        lv_style_init(&style_titles);
        lv_style_set_bg_opa(&style_titles, LV_OPA_TRANSP);
        lv_style_set_text_color(&style_titles, lv_color_hex(LV_COLOR_TEXT_TITLE));
        lv_style_set_text_font(&style_titles, &lv_font_montserrat_24);

}







static void lv_event_handler_button_reset(lv_event_t *event) {

    /**
     * Rutina de pulsado del boton reset
     */

    //esp_rmaker_factory_reset(0, 0);
    



}

static void lv_event_handler_button_home(lv_event_t *event) {

    /**
     * Rutina de pulsado del boton home
     */

    lv_screen_load(screen_main_thermostat);
    //create_main_thermostat();



}


static void paint_qr_code(char *qrcode) {

    // Tamaño del código QR y color
uint16_t qr_size = 130; // Tamaño en píxeles
//lv_color_t qr_fg_color = lv_color_black(); // Color de los píxeles del QR
//lv_color_t qr_bg_color = lv_color_white(); // Color de fondo del QR

// Crear el objeto QR code
//qr = lv_qrcode_create(screen_init_thermostat);

//lv_qrcode_set_size(qr, qr_size);


// Posición en pantalla (opcional)
lv_obj_align(qr, LV_ALIGN_CENTER, 0, 0);

ESP_LOGI(TAG, "qrcode:%s", qrcode);

// Actualizar el QR code con el contenido
//lv_qrcode_update(qr, qrcode, strlen(qrcode));
//free(qrcode);
//qrcode = NULL;


}


static void create_instalation_message() {

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_border_width(&style, 1);
    lv_style_set_border_color(&style, lv_palette_main(LV_PALETTE_ORANGE));
    lv_style_set_pad_all(&style, 2);

    spans = lv_spangroup_create(lv_screen_active());

    lv_obj_set_width(spans, 300);
    lv_obj_set_height(spans, 180);
    lv_obj_set_pos(spans, lv_pct(10), lv_pct(50));
    lv_spangroup_set_align(spans, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(spans, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_indent(spans, 2);
    lv_spangroup_set_mode(spans, LV_SPAN_MODE_BREAK);
    span = lv_spangroup_new_span(spans);
    lv_obj_align_to(spans, qr, LV_ALIGN_BOTTOM_LEFT, -82, 10);
    lv_span_set_text(span, TEXT_INSTRUCTION);
    lv_obj_add_style(spans, &style, LV_PART_MAIN);
}


static void handler_factory(void * arg) {

    lv_set_error_factory();


} 

void create_factory_screen() {

   

        /**
         * Button reset
         */

        screen_init_thermostat = lv_obj_create(NULL);
        lv_screen_load(screen_init_thermostat);
        button_reset = lv_button_create(screen_init_thermostat);
        label_button_reset = lv_label_create(button_reset);
        set_style_buttons();
        lv_label_set_text(label_button_reset, LV_SYMBOL_REFRESH);
        lv_obj_center(label_button_reset);
        lv_obj_add_style(button_reset, &style_buttons_threshold, 0);
        lv_obj_add_style(button_reset, &style_buttons_threshold_pressed, LV_STATE_PRESSED);
        lv_obj_set_pos(button_reset, lv_pct(10), lv_pct(40));
        lv_obj_add_event_cb(button_reset, lv_event_handler_button_reset, LV_EVENT_CLICKED, NULL);

        paint_qr_code(text_qrcode);


        /**
         * instructions of instalation
         */

        create_instalation_message();

        /**
         * Text title application
         */

        title_aplication = lv_label_create(screen_init_thermostat);
        set_style_titles();
        lv_label_set_text(title_aplication, TITLE_APLICATION);
        lv_obj_add_style(title_aplication, &style_titles, LV_PART_MAIN);
        lv_obj_align_to(title_aplication, qr, LV_ALIGN_OUT_TOP_MID, 0, -20);

        /**
         * Button HOME
         */

        button_home = lv_button_create(screen_init_thermostat);
        lv_obj_t *label_home = lv_label_create(button_home);
        lv_obj_add_style(button_home, &style_buttons_threshold, 0);
        lv_obj_add_style(button_home, &style_buttons_threshold_pressed, LV_STATE_PRESSED);
        lv_label_set_text(label_home, LV_SYMBOL_HOME);
        lv_obj_set_pos(button_home, lv_pct(80), lv_pct(40));
        lv_obj_add_flag(button_home, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(button_home, lv_event_handler_button_home, LV_EVENT_CLICKED, NULL);

    const esp_timer_create_args_t factory_shot_timer_args = {
    .callback = &handler_factory,
    /* name is optional, but may help identify the timer when debugging */
    .name = "time refresh date text"
};

   

    esp_timer_create(&factory_shot_timer_args, &timer_factory);
    esp_timer_start_once(timer_factory, TIMING_FACTORY);

    


}


void lv_delete_init_thermostat() {

	if (screen_init_thermostat != NULL)
	lv_obj_del(screen_init_thermostat);
}

void lv_qrcode_confirmed() {


        lv_screen_load(screen_main_thermostat);

}

void lv_set_error_factory() {

    //set_app_status(STATUS_APP_ERROR);
    lv_label_set_text(title_aplication, "ERROR AL INSTALAR!!!");
    lv_obj_set_style_text_color(title_aplication, lv_color_hex(LV_COLOR_TEXT_FAIL_NOTIFICATION), LV_PART_MAIN);
    lv_span_set_text(span, "Pulsa reset para volver a instalar el dispositivo.");
    lv_obj_set_style_text_color(button_reset, lv_color_hex(LV_COLOR_TEXT_FAIL_NOTIFICATION), LV_PART_MAIN);
    lv_obj_set_style_text_color(spans, lv_color_hex(LV_COLOR_TEXT_FAIL_NOTIFICATION), LV_PART_MAIN);



}

void lv_cancel_timer_factory() {

    if (esp_timer_is_active(timer_factory)) {
        esp_timer_stop(timer_factory);
        esp_timer_delete(timer_factory);
        ESP_LOGW(TAG, "Temporizador de factory reset cancelado");
        

    }



}

