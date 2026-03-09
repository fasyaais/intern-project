#include "wifi_service.h"

uint8_t WiFiService::_attempt = 0;

WiFiService::WiFiService(){}

void WiFiService::begin(){
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
}

void WiFiService::_eventHandler(void* arg, esp_event_base_t base, int32_t id, void* data){
    auto ctx = reinterpret_cast<WiFiService*>(arg);
    wifi_mode_t mode;
    esp_wifi_get_mode(&mode);
    printf("event id :%ld\n",id);
    if(base == WIFI_EVENT){
        if(id == WIFI_EVENT_AP_STACONNECTED){
            auto event = reinterpret_cast<wifi_event_ap_staconnected_t*>(data);
            ESP_LOGI(_TAG, "station "MACSTR" join, AID=%d",
                 MAC2STR(event->mac), event->aid);
        }else if(id == WIFI_EVENT_AP_STADISCONNECTED){
            auto event = reinterpret_cast<wifi_event_ap_stadisconnected_t*>(data);
            ESP_LOGI(_TAG, "station "MACSTR" leave, AID=%d, reason=%d",
                 MAC2STR(event->mac), event->aid, event->reason);
        }else if(id == WIFI_EVENT_STA_START){
            esp_wifi_connect();
        }else if(id == WIFI_EVENT_STA_DISCONNECTED){
            if(ctx->_attempt<20){
                esp_wifi_connect();
                ctx->_attempt++;
                ESP_LOGI(_TAG,"retry to connect to the AP");
            }else{
                esp_restart();
            }
        }
    } else if(base == IP_EVENT){
        if(id == IP_EVENT_STA_GOT_IP){
            auto event = reinterpret_cast<ip_event_got_ip_t*>(data);
            ESP_LOGI(_TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
            ctx->_attempt = 0;
        }
    }
}

esp_err_t WiFiService::apInit(){
    esp_netif_create_default_wifi_ap();
    return esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &_eventHandler,
                                                        this,
                                                        nullptr);
}

esp_err_t WiFiService::staInit(){
    esp_netif_create_default_wifi_sta();
    esp_err_t err = ESP_OK;
    err = esp_event_handler_instance_register(WIFI_EVENT,ESP_EVENT_ANY_ID,&_eventHandler,this,nullptr);
    if(err != ESP_OK){
        return err;
    }
    err = esp_event_handler_instance_register(IP_EVENT,IP_EVENT_STA_GOT_IP,&WiFiService::_eventHandler,
                                                            this,nullptr);
    return err;
}