#include "http_client_service.h"

HTTPClientService::HTTPClientService(GPIOManager &gpioManager):_gpioManager(gpioManager){
    _config = {};
    _sseConfig = {};
    _retryQueue = xQueueCreate(10,sizeof(char*));
}

HTTPClientService::~HTTPClientService(){
    ESP_LOGI(_TAG,"http client service close");
}

void HTTPClientService::begin(const char* url){
    _config.url = url;
    _config.user_data = _outputBuffer;
    _config.event_handler = _httpEventHandler;
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

void HTTPClientService::postGPIO(cJSON* json,char* url){
    esp_err_t err;
    char* str = cJSON_PrintUnformatted(json);
    printf("%s\n",str);
    esp_http_client_set_url(_client,url);
    esp_http_client_set_method(_client,HTTP_METHOD_POST);
    esp_http_client_set_header(_client,"Content-Type","application/json");
    esp_http_client_set_post_field(_client,str,strlen(str));
    err = esp_http_client_perform(_client);
    if(err != ESP_OK){
        ESP_LOGE(_TAG,"HTTP POST request failed: %s",esp_err_to_name(err));
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
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void HTTPClientService::checkState(const char* url){
    char output_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};
    int content_length = 0;
    
    esp_http_client_config_t config = {};
    config.url = url;
    esp_http_client_handle_t client = esp_http_client_init(&_sseConfig);
    esp_http_client_set_url(client,"/api/gpio");
    esp_http_client_set_method(client,HTTP_METHOD_GET);
    esp_http_client_set_header(client,"Content-Type","application/json");
    for(;;){
        esp_err_t err = esp_http_client_open(client, 0);
        if (err != ESP_OK) {
            ESP_LOGE(_TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        } else {
            content_length = esp_http_client_fetch_headers(client);
            if (content_length < 0) {
                ESP_LOGE(_TAG, "HTTP client fetch headers failed");
            } else {
                // memset(output_buffer,0,MAX_HTTP_OUTPUT_BUFFER);
                int data_read = esp_http_client_read_response(client, output_buffer, MAX_HTTP_OUTPUT_BUFFER);
                if (data_read >= 0) {
                    auto json = std::unique_ptr<cJSON,decltype(&cJSON_Delete)>(cJSON_Parse(output_buffer), cJSON_Delete);
                    auto* data = cJSON_GetObjectItem(json.get(),"data");
                    if(cJSON_IsArray(data)){
                        cJSON* element = nullptr;
                        cJSON_ArrayForEach(element,data){
                            cJSON* pin   = cJSON_GetObjectItem(element, "pin");
                            cJSON* state = cJSON_GetObjectItem(element, "state");
                            if (pin == nullptr || state == nullptr) {
                                ESP_LOGW(_TAG, "Skipping GPIO is nullptr");
                                continue;
                            }
                            if (!cJSON_IsNumber(pin) || !cJSON_IsBool(state)) {
                                ESP_LOGW(_TAG, "Skipping malformed GPIO entry");
                                continue;
                            }

                            int pinNum = pin->valueint;
                            // char* pinState = state->valuestring;
                            // bool pinState = state->valuestring;
                            bool pinState = cJSON_IsTrue(state);
                            auto led = _gpioManager.getLed((gpio_num_t)pinNum);
                            if(led){
                                if(pinState != led->isON()){
                                    if(pinState)  led->turnON();
                                    else led->turnOFF();
                                    // ESP_LOGI(_TAG,"pin: %d, current state: %d",led->getPin(),led->isON());    
                                }
                                // ESP_LOGI(_TAG,"pin: %d, state: %d",pinNum, pinState);    
                            }
                        }
                    }
                    // auto pin = cJSON_GetObjectItem(json.get(),"pin");
                    // auto state = cJSON_GetObjectItem(json.get(),"state");
                    // cJSON_ArrayForEach(element,data){
                    //     ESP_LOGI(_TAG,"response gpio: %s",element->type);
                    // }
                } else {
                    ESP_LOGE(_TAG, "Failed to read response");
                }
            }
        }
        esp_http_client_close(client);
        // ESP_LOGI(_TAG,"size check state task: %ld",uxTaskGetStackHighWaterMark(NULL));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

esp_err_t HTTPClientService::_httpEventHandler(esp_http_client_event_t *evt){
    auto *instance = static_cast<HTTPClientService*>(evt->user_data);

    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        if(instance && !esp_http_client_is_chunked_response(evt->client)){
            instance->appendBuffer(static_cast<const char*>(evt->data),evt->data_len);
        }
        break;
    default:
        break;
    }
    return ESP_OK;
}

void HTTPClientService::appendBuffer(const char* data, int len){
    int current_len = strlen(_outputBuffer);
    int space_left = MAX_HTTP_OUTPUT_BUFFER - current_len - 1;
    int to_copy = std::min(len, space_left);

    if (to_copy > 0) {
        memcpy(_outputBuffer + current_len, data, to_copy);
        _outputBuffer[current_len + to_copy] = '\0';
    }
}

bool HTTPClientService::getGPIO(int pin){
    char buffer[12];
    sprintf(buffer,"/api/gpio/%d",pin);
    esp_http_client_set_url(_client,buffer);
    memset(_outputBuffer, 0, MAX_HTTP_OUTPUT_BUFFER);
    esp_http_client_set_method(_client,HTTP_METHOD_GET);
    esp_http_client_set_header(_client,"Content-Type","application/json");
    esp_err_t err = esp_http_client_perform(_client);

    if(err != ESP_OK){
        ESP_LOGE(_TAG,"HTTP GET FAILED: %s",esp_err_to_name(err));
        return false;
    }

    if(esp_http_client_get_status_code(_client) >= 400 ){
        ESP_LOGE(_TAG,"NOT FOUND");
        return false;
    }
    auto json = cJSON_Parse(_outputBuffer);
    ESP_LOGW(_TAG,"message: %s",cJSON_GetObjectItem(json,"data")->valuestring);
    cJSON_free(json);
    return true;
}

void HTTPClientService::createGPIO(cJSON* json){
    esp_http_client_set_url(_client,"/api/gpio");
    esp_http_client_set_method(_client,HTTP_METHOD_GET);
    esp_http_client_set_header(_client,"Content-Type","application/json");
    std::string data(cJSON_PrintUnformatted(json));
    esp_http_client_set_post_field(_client,data.c_str(),data.size());
    esp_err_t err = esp_http_client_perform(_client);

    if(err != ESP_OK){
        ESP_LOGE(_TAG,"HTTP GET FAILED: %s",esp_err_to_name(err));
        return;
    }

    if(esp_http_client_get_status_code(_client) >= 400 ){
        ESP_LOGE(_TAG,"NOT FOUND");
        return;
    }
}