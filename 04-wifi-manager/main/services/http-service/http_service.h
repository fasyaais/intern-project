#ifndef HTTP_SERVICE_H
#define HTTP_SERVICE_H

#include "nvs_config.h"
#include "esp_http_server.h"
#include "esp_wifi.h"

class HTTPService
{
private:
    NVSConfig* _nvsConfig;
    httpd_config_t _config;
    httpd_handle_t _handler;
    const char* _TAG = "wifi service";

    void _eventHandler(void* arg, esp_event_base_t base, int32_t id, void* data);
public:
    HTTPService(NVSConfig* nvsConfig);
    ~HTTPService();
    httpd_handle_t server;
    esp_err_t start();
    void begin();
    esp_err_t registerURI(httpd_uri_t* uri);
};



#endif 