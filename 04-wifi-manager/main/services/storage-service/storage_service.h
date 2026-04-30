#ifndef STORAGE_SERVICE_H
#define STORAGE_SERVICE_H

#include "nvs_config.h"
#include "gpio.h"
#include "cJSON.h"
#include "string"
#include "time_service.h"
#include "storage_event.h"

class StorageService
{
private:
    NVSConfig& _nvs_cfg;
    TimeService& _time_service;

    esp_event_loop_handle_t _event_task;
    esp_event_loop_args_t _event_args;

    static void _event_handler(void* arg, esp_event_base_t base, int32_t id, void* data);
public:
    StorageService(NVSConfig& nvs,TimeService& time);
    ~StorageService();

    void init();
    void deinit();
    esp_event_loop_handle_t getHandle();
};


#endif