set(sources 
    "main.cpp"
    # "services/circular-buffer-service/circular_buffer_service.cpp"
    "services/uart-service/uart_service.cpp"
    "services/mpu-service/mpu_service.cpp"

    "drivers/rfid-driver/rfid_driver.cpp"
    "drivers/lcd-driver/lcd_driver.cpp"
    "drivers/mpu-driver/mpu_driver.cpp"

)

set(incl_dirs
    "."
    "models"

    "services/circular-buffer-service"
    "services/uart-service"
    "services/mpu-service"

    "drivers/rfid-driver"
    "drivers/lcd-driver"
    "drivers/mpu-driver"

    
)

idf_component_register(
    SRCS ${sources}
    INCLUDE_DIRS ${incl_dirs}
    REQUIRES
        esp_driver_uart
        esp_driver_i2c
        esp_driver_gpio
        esp_hal_gpspi
        esp_driver_spi
        esp_lcd
)

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-missing-field-initializers")