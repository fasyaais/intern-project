# Daftarkan semua file sumber (.cpp)
set(sources
    "main.cpp"
    
    "cores/system-manager/system_manager.cpp"
    "cores/gpio-manager/gpio_manager.cpp"
    
    "config/nvs-config/nvs_config.cpp"

    "drivers/button-driver/button_driver.cpp"
    "drivers/led-driver/led_driver.cpp"
    
    "services/wifi-service/wifi_service.cpp"
    "services/http-service/http_service.cpp"
    "services/http-client-service/http_client_service.cpp"
    "services/time-service/time_service.cpp"
    "services/led-service/led_service.cpp"
    "services/storage-service/storage_service.cpp"
    
    "controllers/ap-controller/ap_controller.cpp"

    "utils/response/response.cpp"
    "utils/router-wrapper/router_wrapper.cpp"

    "router/ap-router/ap_router.cpp"
)

# Daftarkan semua folder yang berisi file header (.h)
set(include_dirs
    "."
    
    "cores/system-manager"
    "cores/gpio-manager"

    "config/nvs-config"

    "drivers/button-driver"
    "drivers/led-driver"

    "services/wifi-service"
    "services/http-service"
    "services/http-client-service"
    "services/time-service"
    "services/led-service"
    "services/storage-service"

    "models"
    
    "controllers/ap-controller"

    "utils/response"
    "utils/router-wrapper"

    "router/ap-router"
)

idf_component_register(
    SRCS ${sources}
    INCLUDE_DIRS ${include_dirs}
    REQUIRES 
        nvs_flash 
        esp_wifi 
        esp_event 
        esp_http_server 
        esp_http_client 
        mbedtls 
        esp_timer
        esp_driver_gpio

    EMBED_FILES
        "../frontend/dist/index.html"
        "../frontend/dist/assets/index-Bfr4IOl2.css"
        "../frontend/dist/assets/index-BFKvy6o_.js"

        "../app-frontend/dist/app.html"
        "../app-frontend/dist/assets/index-BXeNCRoy.css"
        "../app-frontend/dist/assets/index-DalG0i6k.js"
)