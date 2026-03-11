#ifndef HTTP_SERVICE_H
#define HTTP_SERVICE_H

#include "nvs_config.h"
#include "esp_http_server.h"
#include "esp_wifi.h"

class APRouter; 

class HTTPService
{
private:
    NVSConfig& _nvsConfig;
    httpd_config_t _config;
    httpd_handle_t _server;
    static constexpr const char* _TAG = "http service";
    APRouter& _apRouter;
    
    static void _eventHandler(void* arg, esp_event_base_t base, int32_t id, void* data);
public:
    HTTPService(NVSConfig& nvsConfig,APRouter& apRouter);
    ~HTTPService();

    esp_err_t start();
    esp_err_t start(uint16_t port);
    esp_err_t stop();
    
    // esp_err_t registerURI(httpd_uri_t* uri);
    httpd_handle_t getServer();
};



#endif 