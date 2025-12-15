idf_component_register(
    SRCS "tuya_interface.c"
    INCLUDE_DIRS 
    "include" 
    "../../src"
    REQUIRES thermostat_task events_app rgb_task
    
)
