#include "http_client_service.h"

HTTPClientService::~HTTPClientService(){
    ESP_LOGI(_TAG,"http client service close");
}

void HTTPClientService::begin(const char* url){
    _config.url = url;
    _client = esp_http_client_init(&_config);
    if (_client == nullptr)
    {
        ESP_LOGE(_TAG,"Client nullptr");
    }
    
}

void HTTPClientService::post(const char* data){
    ESP_ERROR_CHECK(esp_http_client_set_url(_client,"http://" PATH ":3000/"));
    ESP_ERROR_CHECK(esp_http_client_set_method(_client, HTTP_METHOD_POST));
    ESP_ERROR_CHECK(esp_http_client_set_header(_client,"Content-Type","application/json"));
    esp_err_t err = esp_http_client_open(_client,strlen(data));

    if(err != ESP_OK){
        ESP_LOGE(_TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
    }else{
        int wlen = esp_http_client_write(_client,data,strlen(data));
        if(wlen < 0){
            ESP_LOGE(_TAG, "write failed");
        }
        _contentLength = esp_http_client_fetch_headers(_client);
        if(_contentLength < 0){
            ESP_LOGE(_TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        }else {
            int data_read = esp_http_client_read_response(_client,_output_buffer,MAX_HTTP_OUTPUT_BUFFER);
            if(data_read >= 0){
                ESP_LOGI(_TAG,"HTTP GET Status = %d, content_length = %"PRId64,esp_http_client_get_status_code(_client),esp_http_client_get_content_length(_client));
                ESP_LOG_BUFFER_HEX(_TAG,_output_buffer,data_read);
            }else{
                ESP_LOGE(_TAG, "Failed to read response");
            }
        }
    }

    esp_http_client_close(_client);

}