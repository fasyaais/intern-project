#include "http_service.h"

HTTPService::HTTPService(NVSConfig* nvsConfig):_nvsConfig(nvsConfig),_config(HTTPD_DEFAULT_CONFIG()){};

esp_err_t HTTPService::start(){
    return httpd_start(&server,&_config);
}

esp_err_t HTTPService::registerURI(httpd_uri_t* uri){
    return httpd_register_uri_handler(server,uri);
}

void HTTPService::begin(){

}

void HTTPService::_eventHandler(void* arg, esp_event_base_t base, int32_t id, void* data){
    auto ctx = reinterpret_cast<HTTPService*>(arg);
    if(base == WIFI_EVENT){
        if(ctx->_handler){
            ESP_LOGI(_TAG,"Stop webserver");
            if(httpd_stop(ctx->_handler)){
                ctx->_handler = nullptr;
            }else{
                ESP_LOGE(_TAG,"Failed to stop webserver");
            }
        }
    } else if(base == IP_EVENT){
        if(ctx->_handler == nullptr){
            ESP_LOGI(_TAG,"Starting webserver");
            // ctx->_handler = 
        }
    }
}