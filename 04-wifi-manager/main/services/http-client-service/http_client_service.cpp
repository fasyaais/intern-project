#include "http_client_service.h"

HTTPClientService::HTTPClientService(){
    _config = {};
    _sseConfig = {};
    _retryQueue = xQueueCreate(10,sizeof(char*));
}

HTTPClientService::~HTTPClientService(){
    ESP_LOGI(_TAG,"http client service close");
}

void HTTPClientService::begin(const char* url){
    _config.url = url;
    // _config.crt_bundle_attach = esp_crt_bundle_attach;
    _client = esp_http_client_init(&_config);
    if (_client == nullptr)
    {
        ESP_LOGE(_TAG,"Client nullptr");
    }
    
    _sseConfig.url = url;
    _sseConfig.keep_alive_enable = true;
    _sseConfig.timeout_ms = 200;
    _sseClient = esp_http_client_init(&_sseConfig);

    if (_sseClient == nullptr)
    {
        ESP_LOGE(_TAG,"sse client nullptr");
    }

}

void HTTPClientService::post(cJSON* json){
    esp_err_t err;
    char* str = cJSON_PrintUnformatted(json);
    esp_http_client_set_method(_client,HTTP_METHOD_POST);
    esp_http_client_set_header(_client,"Content-Type","application/json");
    esp_http_client_set_post_field(_client,str,strlen(str));
    err = esp_http_client_perform(_client);
    if(err != ESP_OK){
        ESP_LOGE(_TAG,"HTTP POST request failed: %s",esp_err_to_name(err));
        char* cpy = strdup(str);

        if (xQueueSend(_retryQueue,&cpy,0) != pdPASS)
        {
            char* oldest;

            if(xQueueReceive(_retryQueue,&oldest,0)){
                cJSON_free(oldest);
            }
            xQueueSend(_retryQueue,&cpy,0);
        }
        
    }
    cJSON_free(str);
}

cJSON* HTTPClientService::get(){
    ESP_ERROR_CHECK(esp_http_client_set_method(_client,HTTP_METHOD_GET));

    esp_err_t err = esp_http_client_perform(_client);
    if (err == ESP_OK) {
        ESP_LOGI(_TAG, "HTTP GET Status = %d, content_length = %"PRId64,
                esp_http_client_get_status_code(_client),
                esp_http_client_get_content_length(_client));
    } else {
        ESP_LOGE(_TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }
    
    // cJSON* parse = cJSON_Parse(_outputBuffer);
    // cJSON* msg = cJSON_GetObjectItem(parse,"msg");
    // ESP_LOGI(_TAG,"msg: %s",msg->valuestring);

    cJSON* str = cJSON_Parse(_outputBuffer);
    return str;
}

void HTTPClientService::checkStatus(){
    char* receiveQueue;
    for(;;){
        if(uxQueueMessagesWaiting(_retryQueue) > 0){
            esp_http_client_set_url(_client,"/health");
            esp_http_client_set_method(_client,HTTP_METHOD_HEAD);
            esp_http_client_set_header(_client,"Content-Type","application/json");
            esp_err_t err;
            err = esp_http_client_perform(_client);
            int contentLen = esp_http_client_get_content_length(_client);
            
            if(err == ESP_OK ){
                int statusCode = esp_http_client_get_status_code(_client);
                if(statusCode == 200){
                    if(xQueueReceive(_retryQueue,&receiveQueue,0)){
                        esp_http_client_set_url(_client,"/");
                        esp_http_client_set_method(_client,HTTP_METHOD_POST);
                        esp_http_client_set_header(_client,"Content-Type","application/json");
                        esp_http_client_set_post_field(_client,receiveQueue,strlen(receiveQueue));
                        esp_http_client_perform(_client);
                    }
                }
            }else {
                ESP_LOGI(_TAG,"HTTP still down: %s",esp_err_to_name(err));
            }
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void HTTPClientService::sse(){
    esp_http_client_set_url(_sseClient,"/sse");
    // esp_http_client_set_method(_sseClient,HTTP_METHOD_GET);
    esp_http_client_set_header(_sseClient,"Accept","text/event-stream");
    esp_http_client_set_header(_sseClient,"Cache-Control","no-cache");
    esp_err_t err = esp_http_client_open(_sseClient, 0);
    if (err != ESP_OK) {
        ESP_LOGE(_TAG, "open failed: %s", esp_err_to_name(err));
        return;
    }
    
    err = esp_http_client_request_send(_sseClient,0);
    if (err != ESP_OK) {
        ESP_LOGE(_TAG, "send request failed: %s", esp_err_to_name(err));
        return;
    }
    int contentLength = esp_http_client_fetch_headers(_sseClient);
    int statusCode = esp_http_client_get_status_code(_sseClient);
    ESP_LOGI(_TAG, "status: %d, content-length: %d", statusCode, contentLength);

    char buffer[512];
    int read_len;
    for(;;){
        read_len = esp_http_client_read(_sseClient, buffer, sizeof(buffer) - 1);
        if (read_len < 0) {
            ESP_LOGE(_TAG, "Error reading stream");
            break;
        } else if (read_len == 0) {
            ESP_LOGI(_TAG, "Connection closed");
            break;
        }
        buffer[read_len] = '\0';
        ESP_LOGI(_TAG, "Received: %s", buffer);
        // if (len > 0) {
        //     buffer[len] = '\0';
        //     ESP_LOGI(_TAG, "SSE: %s", buffer);
        // } else if (len == 0) {
        //     ESP_LOGW(_TAG, "connection closed, reconnecting...");
        //     esp_http_client_close(_sseClient);
        //     vTaskDelay(pdMS_TO_TICKS(3000));
        //     esp_http_client_open(_sseClient, 0);
        //     esp_http_client_request_send(_sseClient,0);
        //     esp_http_client_fetch_headers(_sseClient);
        // } else {
        //     ESP_LOGE(_TAG, "read error: %d", len);
        //     esp_http_client_close(_sseClient);
        //     vTaskDelay(pdMS_TO_TICKS(3000));
        //     esp_http_client_open(_sseClient, 0);
        //     esp_http_client_request_send(_sseClient, 0);
        //     esp_http_client_fetch_headers(_sseClient);
        // }
        vTaskDelay(pdMS_TO_TICKS(200));
    }

}