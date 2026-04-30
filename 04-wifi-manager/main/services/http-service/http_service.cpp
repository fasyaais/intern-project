#include "http_service.h"
#include "ap_router.h"

HTTPService::HTTPService(NVSConfig& nvsConfig,APRouter& apRouter):_nvsConfig(nvsConfig),_config(HTTPD_DEFAULT_CONFIG()),_server(nullptr),_apRouter(apRouter){}

HTTPService::~HTTPService(){
    ESP_LOGI(_TAG,"http service close");
    // stop();
}

esp_err_t HTTPService::start(){
    _config.server_port = 80;
    _config.lru_purge_enable = true;

    if(_server != nullptr){
        ESP_LOGW(_TAG,"Server already started");
    }
    esp_err_t err = httpd_start(&_server,&_config);
    if(err == ESP_OK){
        ESP_LOGI(_TAG, "HTTP server started on port %d", 80);
    }else{
        ESP_LOGE(_TAG, "Failed to start HTTP server");
    }
    return err;
}

esp_err_t HTTPService::start(uint16_t port){
    _config.server_port = port;
    _config.lru_purge_enable = true;

    if(_server != nullptr){
        ESP_LOGW(_TAG,"Server already started");
    }

    esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED,&_eventHandler,this,&_server);

    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP,&_eventHandler,this,&_server);

    esp_err_t err = httpd_start(&_server,&_config);
    if(err == ESP_OK){
        ESP_LOGI(_TAG, "HTTP server started on port %d", port);
    }else{
        ESP_LOGE(_TAG, "Failed to start HTTP server");
    }
    return err;
}

esp_err_t HTTPService::startStation(){
    _config.server_port = 80;
    _config.lru_purge_enable = true;

    if(_server != nullptr){
        ESP_LOGW(_TAG,"Server already started");
    }

    esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED,&_eventHandlerStation,this,&_server);

    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP,&_eventHandlerStation,this,&_server);

    esp_err_t err = httpd_start(&_server,&_config);
    if(err == ESP_OK){
        ESP_LOGI(_TAG, "HTTP server started on port %d", 80);
    }else{
        ESP_LOGE(_TAG, "Failed to start HTTP server");
    }
    return err;
}

esp_err_t HTTPService::stop(){
    if(_server == nullptr){
        return ESP_OK;
    }

    esp_err_t err = httpd_stop(_server);
    if(err == ESP_OK){
        ESP_LOGI(_TAG,"HTTP server stoped");
        _server = nullptr;
    }
    return err;
}

// esp_err_t HTTPService::registerURI(httpd_uri_t* uri){
//     return httpd_register_uri_handler(_server,uri);
// }


void HTTPService::_eventHandler(void* arg, esp_event_base_t base, int32_t id, void* data){
    auto ctx = reinterpret_cast<HTTPService*>(arg);
    if(base == WIFI_EVENT){
        if(ctx->_server){
            ESP_LOGI(_TAG,"Stop webserver");
            if(httpd_stop(ctx->_server) == ESP_OK){
                ctx->_server = nullptr;
            }else{
                ESP_LOGE(_TAG,"Failed to stop webserver");
            }
        }
    } else if(base == IP_EVENT && id == IP_EVENT_STA_GOT_IP){
        if(ctx->_server == nullptr){
            ESP_LOGI(_TAG,"Starting webserver");
            if(httpd_start(&ctx->_server,&ctx->_config) == ESP_OK){
                ctx->_apRouter.registerRouter(ctx->_server);
                // ctx->_apRouter
            }
        }
    }
}

void HTTPService::_eventHandlerStation(void* arg, esp_event_base_t base, int32_t id, void* data){
    auto ctx = reinterpret_cast<HTTPService*>(arg);
    if(base == WIFI_EVENT){
        if(ctx->_server){
            ESP_LOGI(_TAG,"Stop webserver");
            if(httpd_stop(ctx->_server) == ESP_OK){
                ctx->_server = nullptr;
            }else{
                ESP_LOGE(_TAG,"Failed to stop webserver");
            }
        }
    } else if(base == IP_EVENT && id == IP_EVENT_STA_GOT_IP){
        if(ctx->_server == nullptr){
            ESP_LOGI(_TAG,"Starting webserver");
            if(httpd_start(&ctx->_server,&ctx->_config) == ESP_OK){
                ctx->_apRouter.registerRouterStation(ctx->_server);
                // ctx->_apRouter
            }
        }
    }

}

httpd_handle_t HTTPService::getServer(){
    return _server;
}