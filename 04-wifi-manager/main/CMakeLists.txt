# Daftarkan semua file sumber (.cpp)
set(sources
    "main.cpp"
    "config/nvs-config/nvs_config.cpp"
    "services/wifi-service/wifi_service.cpp"
    "services/http-service/http_service.cpp"
)

# Daftarkan semua folder yang berisi file header (.h)
set(include_dirs
    "."
    "config/nvs-config"
    "services/wifi-service"
    "services/http-service"
    "models"
)

idf_component_register(
    SRCS ${sources}
    INCLUDE_DIRS ${include_dirs}
    REQUIRES nvs_flash esp_wifi esp_event esp_http_server
)