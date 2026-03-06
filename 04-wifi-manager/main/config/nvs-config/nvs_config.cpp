#include "nvs_config.h"

NVSConfig::~NVSConfig(){
    ESP_LOGI(_TAG,"Close nvs");
    nvs_close(_handler);
    return;
}

esp_err_t NVSConfig::open(){
    ESP_LOGI(_TAG,"Open nvs");
    _err = nvs_open("storage",NVS_READWRITE,&_handler);
    if(_err != ESP_OK){
        ESP_LOGW(_TAG,"Failed to open nvs : ",esp_err_to_name(_err));
        return ESP_FAIL;
    }
    return ESP_OK;
}
/**
 * @brief Membaca nilai dari nvs
 * @param key string
 * @return
 * - ESP_OK jika berhasil membaca data
 * - nullptr jika gagal membaca data
 */
std::unique_ptr<char[]> NVSConfig::read(const char* key){
    size_t required_size = 0;
    esp_err_t err;
    ESP_LOGI("nvs storage", "Read string to nvs");
    err = nvs_get_str(_handler, key, nullptr, &required_size);

    if(err != ESP_OK){
        return nullptr;
    }

    auto message = std::make_unique<char[]>(required_size);
    err = nvs_get_str(_handler, key, message.get(), &required_size);
    if(err != ESP_OK){
        return nullptr;
    }

    ESP_LOGI(_TAG, "Success read string");
    return message;
}

/**
 * @brief Menulis string ke nvs
 * * @param key string
 * @param message string
 * * @returns
 * - ESP_OK jika berhasil menulis string ke nvs <br>
 * - ESP_FAIL jika gagal menulis ke nvs
 */
esp_err_t NVSConfig::write(const char* key,const char* message){
    ESP_LOGI(_TAG, "Writing string to NVS...");
    esp_err_t err = nvs_set_str(_handler, key, message);
    if (err != ESP_OK) {
        ESP_LOGE("nvs storage", "Failed to write string");
        return ESP_FAIL;
    }
    return nvs_commit(_handler);
}