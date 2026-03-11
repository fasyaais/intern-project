#include "wifi_service.h"

uint8_t WiFiService::_attempt = 0;
EventGroupHandle_t WiFiService::_wifiEventGroup = nullptr;

WiFiService::WiFiService(NVSConfig& nvsConfig):_nvsConfig(nvsConfig){}

WiFiService::~WiFiService(){
    ESP_LOGI(_TAG,"wifi service close");
}

void WiFiService::begin(){
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
}

void WiFiService::_eventHandlerSTA(void* arg, esp_event_base_t base, int32_t id, void* data){
    auto ctx = reinterpret_cast<WiFiService*>(arg);
    if(base == WIFI_EVENT ){
        if(id == WIFI_EVENT_STA_DISCONNECTED){
            if(ctx->_attempt < 20){
                esp_wifi_connect();
                ctx->_attempt++;
                ESP_LOGI(_TAG,"retry to connect to the AP");
            }else{
                ctx->_nvsConfig.erase("ssid");
                ctx->_nvsConfig.erase("password");
                ctx->_nvsConfig.erase("authmode");
                esp_restart();
            }
        }else if ( id == WIFI_EVENT_STA_START){
            ESP_LOGI(_TAG,"Wifi Start");
            esp_wifi_connect();
        }else if(id == WIFI_EVENT_STA_CONNECTED){
            ESP_LOGI(_TAG,"WiFi connected");
        }
    } else if(base == IP_EVENT && id == IP_EVENT_STA_GOT_IP){
        auto event = reinterpret_cast<ip_event_got_ip_t*>(data);
        ESP_LOGI(_TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        ctx->_attempt = 0;
        xEventGroupSetBits(_wifiEventGroup,WIFI_CONNECTED_BIT);
    }
}

void WiFiService::_eventHandlerAP(void* arg, esp_event_base_t base, int32_t id, void* data){
    if(id == WIFI_EVENT_AP_STACONNECTED){
        auto event = reinterpret_cast<wifi_event_ap_staconnected_t*>(data);
        ESP_LOGI(_TAG, "station "MACSTR" join, AID=%d",
            MAC2STR(event->mac), event->aid);
    }else if( id == WIFI_EVENT_AP_STADISCONNECTED){
        auto event = reinterpret_cast<wifi_event_ap_stadisconnected_t*>(data);
        ESP_LOGI(_TAG, "station "MACSTR" leave, AID=%d, reason=%d",
                 MAC2STR(event->mac), event->aid, event->reason);
    }
}

esp_err_t WiFiService::apInit(){
    esp_netif_create_default_wifi_ap();
    return esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &_eventHandlerAP,
                                                        this,
                                                        nullptr);
}

esp_err_t WiFiService::staInit(){
    esp_netif_create_default_wifi_sta();
    esp_err_t err = ESP_OK;
    _wifiEventGroup = xEventGroupCreate();
    err = esp_event_handler_instance_register(WIFI_EVENT,ESP_EVENT_ANY_ID,&_eventHandlerSTA,this,nullptr);
    if(err != ESP_OK){
        return err;
    }
    err = esp_event_handler_instance_register(IP_EVENT,IP_EVENT_STA_GOT_IP,&_eventHandlerSTA,
                                                            this,nullptr);
    return err;
}

std::vector<wifi_ap_record_t> WiFiService::scanAP(){
    uint16_t count =0;
    ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&count));
    std::vector<wifi_ap_record_t>records(count);
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&count,records.data()));
    std::sort(records.begin(),records.end(),[](wifi_ap_record_t a, wifi_ap_record_t b){
        return a.rssi > b.rssi;
    });

    for (auto &i : records)
    {
        ESP_LOGI(_TAG,"ssid: %s,rssi: %d,authmode; %d",i.ssid,i.rssi,i.authmode);
    }
    

    return records;
}

void WiFiService::waitConnected(){
    xEventGroupWaitBits(_wifiEventGroup,WIFI_CONNECTED_BIT,pdFALSE,pdTRUE,portMAX_DELAY);
}