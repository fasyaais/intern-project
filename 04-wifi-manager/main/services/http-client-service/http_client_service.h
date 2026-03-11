#ifndef HTTP_CLIENT_SERVICE_H
#define HTTP_CLIENT_SERVICE_H

#include "esp_http_client.h"
#include "esp_log.h"

#define PATH "192.168.190.42"
#define MAX_HTTP_OUTPUT_BUFFER 2048

class HTTPClientService
{
private:
    static constexpr const char* _TAG = "http client service";
    char _output_buffer[MAX_HTTP_OUTPUT_BUFFER + 1] = {0};
    int _contentLength;

    esp_http_client_config_t _config;
    esp_http_client_handle_t _client;
public:
    // HTTPClientService();
    ~HTTPClientService();

    void begin(const char* url);
    void post(const char* data);
};

#endif