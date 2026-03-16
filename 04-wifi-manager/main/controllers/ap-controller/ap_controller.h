#ifndef AP_CONTROLLER_H
#define AP_CONTROLLER_H

#include "wifi_service.h"
#include "esp_http_server.h"
#include <cJSON.h>
#include "response.h"
#include "nvs_config.h"
// #include "app_context.h"
#include "router_context.h"

#define REBOOT_SUCCESS_BIT BIT0

extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[] asm("_binary_index_html_end");
extern const uint8_t index_css_start[] asm("_binary_index_Bfr4IOl2_css_start");
extern const uint8_t index_css_end[] asm("_binary_index_Bfr4IOl2_css_end");
extern const uint8_t index_js_start[] asm("_binary_index_v8LhRFuD_js_start");
extern const uint8_t index_js_end[] asm("_binary_index_v8LhRFuD_js_end");

class APController
{
private:
    WiFiService& _wifiService;
    NVSConfig& _nvsConfig;

    void _cors(httpd_req_t* req);
    static constexpr const char* _TAG = "ap contoller";

    EventGroupHandle_t _eventGroup;

public:
    APController(WiFiService& WiFiService,NVSConfig& nvsConfig);
    ~APController();

    esp_err_t apiScanAP(httpd_req_t* req);
    esp_err_t indexCSS(httpd_req_t* req);
    esp_err_t indexHTML(httpd_req_t* req);
    esp_err_t appJS(httpd_req_t* req);
    esp_err_t connectAP(httpd_req_t* req);

    void waitReboot();

};

#endif