#include "wifi_service.h"

void WiFiService::begin(){
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
}

void WiFiService::_eventHandlerDispatcher(void* arg, esp_event_base_t base, int32_t id, void* data){
    auto obj = reinterpret_cast<WiFiService*>(arg);
    if(base == WIFI_EVENT){
        // if(id == WIFI_IF_STA) 
        // esp_event_handler_instance_register()
    }
}
