#ifndef TIME_SERVICE_H
#define TIME_SERVICE_H

#include "esp_log.h"
#include "esp_netif_sntp.h"
#include "nvs_config.h"
#include "esp_timer.h"


class TimeService
{
private:
    /* data */
    NVSConfig& _nvsConfig;
    static constexpr const char* _TAG = "time service";
    time_t _now;

    esp_err_t _obtainTime();
public:
    TimeService(NVSConfig& nvsConfig);
    // ~TimeService();

    void init();
    esp_err_t fetchTime();
    uint64_t getNow();
};


#endif