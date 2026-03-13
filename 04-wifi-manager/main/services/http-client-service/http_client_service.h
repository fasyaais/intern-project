#ifndef HTTP_CLIENT_SERVICE_H
#define HTTP_CLIENT_SERVICE_H

#include "esp_http_client.h"
#include "esp_log.h"
#include "cJSON.h"
#include "esp_crt_bundle.h"

#define PATH "192.168.1.51"
// #define PATH "dogapi.dog"
#define MAX_HTTP_OUTPUT_BUFFER 2048

class HTTPClientService
{
private:
    static constexpr const char* _TAG = "http client service";
    char _outputBuffer[MAX_HTTP_OUTPUT_BUFFER + 1] = {0};
    int _contentLength;

    esp_http_client_config_t _config;
    esp_http_client_handle_t _client;

    // esp_err_t _eventHandler;
public:
    // HTTPClientService();
    ~HTTPClientService();

    void begin(const char* url);
    cJSON* get();
    void post(cJSON* json);
};

#endif