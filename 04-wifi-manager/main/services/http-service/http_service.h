#ifndef HTTP_SERVICE_H
#define HTTP_SERVICE_H

#include "nvs_config.h"
#include "esp_http_server.h"

class HTTPService
{
private:
    NVSConfig* _nvsConfig;
    httpd_config_t _config;
public:
    HTTPService(NVSConfig* nvsConfig);
    ~HTTPService();
    httpd_handle_t server;
    esp_err_t start();
    esp_err_t registerURI(httpd_uri_t* uri);
};



#endif 