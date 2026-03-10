#ifndef NVS_CONFIG_H
#define NVS_CONFIG_H

#include "nvs.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "memory"

class NVSConfig
{
private:
    nvs_handle_t _handler;
    esp_err_t _err;
    const char* _TAG = "nvs storage";
public:
    // NVSConfig();
    ~NVSConfig();
    esp_err_t open();
    std::unique_ptr<char[]> read(const char* key);
    esp_err_t write(const char* key,const char* message);
    esp_err_t erase(const char* key);

};

#endif