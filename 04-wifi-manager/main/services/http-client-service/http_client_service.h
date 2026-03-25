#ifndef HTTP_CLIENT_SERVICE_H
#define HTTP_CLIENT_SERVICE_H

#include "esp_http_client.h"
#include "esp_log.h"
#include "cJSON.h"
#include "esp_crt_bundle.h"
#include "string"
#include "memory"
#include "gpio_manager.h"

#define PATH "192.168.1.51"
// #define PATH "dogapi.dog"
#define MAX_HTTP_OUTPUT_BUFFER 2048

class HTTPClientService
{
private:
    static constexpr const char* _TAG = "http client service";
    char _outputBuffer[MAX_HTTP_OUTPUT_BUFFER] = {0};
    int _contentLength;

    GPIOManager &_gpioManager;
    // std::queue<char> _dataQueue;
    QueueHandle_t _retryQueue;

    esp_http_client_config_t _config;
    esp_http_client_handle_t _client;

    esp_http_client_config_t _sseConfig;
    esp_http_client_handle_t _sseClient;

    static esp_err_t _httpEventHandler(esp_http_client_event_t *evt);
    void appendBuffer(const char* data, int len);
    // esp_err_t _eventHandler;
public:
    HTTPClientService(GPIOManager &gpioManager);
    ~HTTPClientService();

    void begin(const char* url);
    cJSON* get();
    void post(cJSON* json);
    void checkStatus();
    void sse();
    void postGPIO(cJSON* json,char* url);

    bool getGPIO(int pin);
    void createGPIO(cJSON* json);

    void checkState(const char* url);
};

#endif