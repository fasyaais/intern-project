#include "ap_controller.h"

APController::APController(WiFiService& wifiService,NVSConfig& nvsConfig):_wifiService(wifiService),_nvsConfig(nvsConfig){}

APController::~APController(){
    ESP_LOGI(_TAG, "ap controller close");
}

void APController::_cors(httpd_req_t* req){
    httpd_resp_set_hdr(req,"Access-Control-Allow-Origin","*");
    httpd_resp_set_hdr(req,"Access-Control-Allow-Method","GET, POST, PUT, OPTIONS, DELETE");
    httpd_resp_set_hdr(req,"Access-Control-Allow-Headers","Content-Type");
}

esp_err_t APController::apiScanAP(httpd_req_t* req){
    _cors(req);
    std::vector<wifi_ap_record_t> records = _wifiService.scanAP();
    cJSON* arry = cJSON_CreateArray();
    
    for (auto &record : records)
    {
        cJSON* item = cJSON_CreateObject();
        cJSON_AddStringToObject(item,"ssid",reinterpret_cast<char*>(record.ssid));    
        cJSON_AddNumberToObject(item,"rssi",record.rssi);
        cJSON_AddNumberToObject(item,"authmode",record.authmode);
        cJSON_AddItemToArray(arry,item);    
    }
    
    response::sendJSON(req,arry);
    
    return ESP_OK;
}

esp_err_t APController::indexCSS(httpd_req_t* req){
    httpd_resp_set_type(req, "text/css");
    httpd_resp_send(req, (const char *)index_css_start,
                    index_css_end- index_css_start);
    return ESP_OK;
}
esp_err_t APController::indexHTML(httpd_req_t* req){
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)index_html_start,
                    index_html_end - index_html_start);
    return ESP_OK;
}

esp_err_t APController::appJS(httpd_req_t* req){
    httpd_resp_set_type(req, "application/javascript");
    httpd_resp_send(req, (const char *)index_js_start,
                    index_js_end- index_js_start);
    return ESP_OK;
}

esp_err_t APController::connectAP(httpd_req_t* req){
    _cors(req);
    auto* ctx = static_cast<router_context_t*>(req->user_ctx);
    APController* ctlr = ctx->controller;
    auto json = std::unique_ptr<cJSON,decltype(&cJSON_Delete)>(response::readJSON(req),cJSON_Delete);
    cJSON* ssid = cJSON_GetObjectItem(json.get(),"ssid");
    cJSON* pass = cJSON_GetObjectItem(json.get(),"password");
    esp_err_t err1 = ctlr->_nvsConfig.write("ssid",ssid->valuestring);
    esp_err_t err2 = ctlr->_nvsConfig.write("password",pass->valuestring);
    esp_err_t err3 = ctlr->_nvsConfig.write("authmode","3");
    if(err1 != ESP_OK && err2 != ESP_OK){
        httpd_resp_send_500(req);
        return ESP_OK;
    }
    ESP_LOGI("http server","ssid: %s, pass: %s",ssid->valuestring,pass->valuestring);
    vTaskDelay(pdMS_TO_TICKS(2000));
    esp_restart();
    return ESP_OK;
}

