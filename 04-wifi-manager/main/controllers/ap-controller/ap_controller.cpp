#include "ap_controller.h"

ESP_EVENT_DEFINE_BASE(STORAGE_EVENT);

APController::APController(
    WiFiService &wifiService, 
    NVSConfig &nvsConfig, 
    GPIOManager &gpioMan,
    StorageService& storage,
    TimeService& time
) : 
    _wifiService(wifiService),
    _nvsConfig(nvsConfig), 
    _gpioMan(gpioMan),
    _storageService(storage), 
    _time(time),
    _eventGroup(xEventGroupCreate()) {}

APController::~APController()
{
    ESP_LOGI(_TAG, "ap controller close");
}

void APController::_cors(httpd_req_t *req)
{
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Method", "GET, POST, PUT, OPTIONS, DELETE");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Headers", "Content-Type");
}

esp_err_t APController::apiScanAP(httpd_req_t *req)
{
    _cors(req);
    std::vector<wifi_ap_record_t> records = _wifiService.scanAP();
    cJSON *arry = cJSON_CreateArray();

    for (auto &record : records)
    {
        cJSON *item = cJSON_CreateObject();
        cJSON_AddStringToObject(item, "ssid", reinterpret_cast<char *>(record.ssid));
        cJSON_AddNumberToObject(item, "rssi", record.rssi);
        cJSON_AddNumberToObject(item, "authmode", record.authmode);
        cJSON_AddItemToArray(arry, item);
    }

    response::sendJSON(req, arry);

    return ESP_OK;
}

esp_err_t APController::indexCSS(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/css");
    httpd_resp_send(req, (const char *)index_css_start,
                    index_css_end - index_css_start);
    return ESP_OK;
}
esp_err_t APController::indexHTML(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)index_html_start,
                    index_html_end - index_html_start);
    return ESP_OK;
}

esp_err_t APController::appJS(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/javascript");
    httpd_resp_send(req, (const char *)index_js_start,
                    index_js_end - index_js_start);
    return ESP_OK;
}
// UI APP CONTROL
esp_err_t APController::appUICSS(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/css");
    httpd_resp_send(req, (const char *)app_css_start,
                    app_css_end - app_css_start);
    return ESP_OK;
}
esp_err_t APController::appUIHTML(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)app_html_start,
                    app_html_end - app_html_start);
    return ESP_OK;
}

esp_err_t APController::appUIJS(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/javascript");
    httpd_resp_send(req, (const char *)app_js_start,
                    app_js_end - app_js_start);
    return ESP_OK;
}

esp_err_t APController::connectAP(httpd_req_t *req)
{
    _cors(req);
    auto *ctx = static_cast<router_context_t *>(req->user_ctx);
    APController *ctlr = ctx->controller;
    auto json = std::unique_ptr<cJSON, decltype(&cJSON_Delete)>(response::readJSON(req), cJSON_Delete);
    cJSON *ssid = cJSON_GetObjectItem(json.get(), "ssid");
    cJSON *pass = cJSON_GetObjectItem(json.get(), "password");
    esp_err_t err1 = ctlr->_nvsConfig.write("ssid", ssid->valuestring);
    esp_err_t err2 = ctlr->_nvsConfig.write("password", pass->valuestring);
    esp_err_t err3 = ctlr->_nvsConfig.write("authmode", "3");
    if (err1 != ESP_OK && err2 != ESP_OK)
    {
        httpd_resp_send_500(req);
        return ESP_OK;
    }
    ESP_LOGI("http server", "ssid: %s, pass: %s", ssid->valuestring, pass->valuestring);
    vTaskDelay(pdMS_TO_TICKS(2000));
    xEventGroupSetBits(_eventGroup, REBOOT_SUCCESS_BIT);
    return ESP_OK;
}

esp_err_t APController::apiScanGPIO(httpd_req_t *req)
{
    _cors(req);
    auto *ctx = static_cast<router_context_t *>(req->user_ctx);
    auto *cntrl = ctx->controller;
    auto raw_data_ptr = cntrl->_nvsConfig.read("data");
    if(!raw_data_ptr) return ESP_FAIL;
    std::string data = raw_data_ptr.get();
    if(data.empty()){
        httpd_resp_send_404(req);
        httpd_resp_set_type(req, "application/json");
        return ESP_OK;
    }

    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, data.c_str(), data.size());
    return ESP_OK;
}

esp_err_t APController::updateGPIO(httpd_req_t *req)
{
    _cors(req);
    auto *ctx = static_cast<router_context_t *>(req->user_ctx);
    auto *cntrl = ctx->controller;
    auto res = std::unique_ptr<cJSON,decltype(&cJSON_Delete)>(response::readJSON(req),cJSON_Delete);
    gpio_t data = {
        .pin = (gpio_num_t)cJSON_GetObjectItemCaseSensitive(res.get(),"pin")->valueint,
        .state = (bool)cJSON_IsTrue(cJSON_GetObjectItemCaseSensitive(res.get(),"state")),
        .last_click_at = _time.getNow() * 1000,
    };
    esp_event_post_to(_storageService.getHandle(),STORAGE_EVENT,STORAGE_WRITE,&data,sizeof(data),portMAX_DELAY);
    auto led = _gpioMan.getLed(data.pin);
    if(data.state) led->turnON();
    else led->turnOFF();
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, "{}", strlen("2"));
    return ESP_OK;
}

esp_err_t APController::createGPIO(httpd_req_t *req)
{
    _cors(req);
    auto *ctx = static_cast<router_context_t *>(req->user_ctx);
    auto *cntrl = ctx->controller;
    auto res = std::unique_ptr<cJSON,decltype(&cJSON_Delete)>(response::readJSON(req),cJSON_Delete);
    gpio_t data = {
        .pin = (gpio_num_t)cJSON_GetObjectItemCaseSensitive(res.get(),"pin")->valueint,
        .state = false,
        .last_click_at = 0
    };
    LedDriver* led = _gpioMan.getLed(data.pin);
    if(!led) return ESP_FAIL;
    led->turnOFF();
    esp_event_post_to(_storageService.getHandle(),STORAGE_EVENT,STORAGE_WRITE,&data,sizeof(data),portMAX_DELAY);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, "{}", strlen("{}"));
    return ESP_OK;
}

esp_err_t APController::deleteGPIO(httpd_req_t *req)
{
    _cors(req);
    auto *ctx = static_cast<router_context_t *>(req->user_ctx);
    auto *cntrl = ctx->controller;
    auto res = std::unique_ptr<cJSON,decltype(&cJSON_Delete)>(response::readJSON(req),cJSON_Delete);
    gpio_t data = {
        .pin = (gpio_num_t)cJSON_GetObjectItemCaseSensitive(res.get(),"pin")->valueint,
        .state = false,
        .last_click_at = 0
    };
    LedDriver* led = _gpioMan.getLed(data.pin);
    if(!led) return ESP_FAIL;
    {
        auto raw_data_ptr = ctx->controller->_nvsConfig.read("data");
        if(!raw_data_ptr) return ESP_FAIL;
        std::string raw_data = raw_data_ptr.get();
        cJSON* arr = cJSON_Parse(raw_data.c_str());
        cJSON* elemnt = nullptr;
        int itr = 0;
        cJSON_ArrayForEach(elemnt, arr){
            int pin = cJSON_GetObjectItemCaseSensitive(elemnt,"pin")->valueint;
            ESP_LOGW(_TAG,"to delete");
            if(data.pin == pin){
                cJSON_DeleteItemFromArray(arr,itr);
                ESP_LOGW(_TAG,"success to delete");
                break;
            }
            itr++;
        }
        std::string data = cJSON_PrintUnformatted(arr);
        ctx->controller->_nvsConfig.write("data",data.c_str());
        cJSON_Delete(arr);
    }
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, "success", strlen("success"));
    return ESP_OK;
}

void APController::waitReboot()
{
    xEventGroupWaitBits(_eventGroup, REBOOT_SUCCESS_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
}
