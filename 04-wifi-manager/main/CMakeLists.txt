# Daftarkan semua file sumber (.cpp)
set(sources
    "main.cpp"
    
    "cores/system-manager/system_manager.cpp"
    
    "config/nvs-config/nvs_config.cpp"
    
    "services/wifi-service/wifi_service.cpp"
    "services/http-service/http_service.cpp"
    
    "controllers/ap-controller/ap_controller.cpp"

    "utils/response/response.cpp"
    "utils/router-wrapper/router_wrapper.cpp"

    "router/ap-router/ap_router.cpp"
)

# Daftarkan semua folder yang berisi file header (.h)
set(include_dirs
    "."
    
    "cores/system-manager"

    "config/nvs-config"

    "services/wifi-service"
    "services/http-service"

    "models"
    
    "controllers/ap-controller"

    "utils/response"
    "utils/router-wrapper"

    "router/ap-router"
)

idf_component_register(
    SRCS ${sources}
    INCLUDE_DIRS ${include_dirs}
    REQUIRES nvs_flash esp_wifi esp_event esp_http_server
    EMBED_FILES
        "../frontend/dist/index.html"
        "../frontend/dist/assets/index-CQOd6Co-.css"
        "../frontend/dist/assets/index-19cTNh9z.js"

)