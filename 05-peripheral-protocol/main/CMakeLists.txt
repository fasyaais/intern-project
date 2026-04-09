set(sources 
    "main.cpp"
    # "services/circular-buffer-service/circular_buffer_service.cpp"
    "services/uart-service/uart_service.cpp"

    "drivers/i2c-driver/i2c_driver.cpp"
)

set(incl_dirs
    "."
    "models"

    "services/circular-buffer-service"
    "services/uart-service"

    "drivers/i2c-driver"
)

idf_component_register(
    SRCS ${sources}
    INCLUDE_DIRS ${incl_dirs}
    REQUIRES
        esp_driver_uart
        esp_driver_i2c
        esp_driver_gpio
)
