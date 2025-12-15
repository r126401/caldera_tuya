idf_component_register(
    SRCS "thermostat_task.c"
    INCLUDE_DIRS 
    "include" 
    "../../src"
    REQUIRES espressif__ds18b20 espressif__onewire_bus esp_driver_gpio events_app
    
)
