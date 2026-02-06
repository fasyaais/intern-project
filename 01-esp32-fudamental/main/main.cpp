#include <cstdio>
#include <cstdint>
#include "esp_mac.h"
#include <cJSON.h>
#include <json_generator.h>
#include <esp_system.h>
#include <esp_chip_info.h>
#include <esp_flash.h>
#include <memory>
#include <array>
#include <etl/array.h>
#include <etl/memory.h>

struct JsonString_d {
    void operator()(char* p) const {if(p) cJSON_free(p);}
};

struct JsonObject_d {
    void operator()(cJSON* p) const {if(p) cJSON_Delete(p);}
};

extern "C" void app_main(void){
    etl::array<uint8_t, 6> mac;
    etl::array<char,32> msg;

    etl::unique_ptr<cJSON, JsonObject_d> root(cJSON_CreateObject());
    esp_chip_info_t info_;
    uint32_t flash_size_;
    
    esp_flash_get_size(nullptr,&flash_size_);
    esp_chip_info(&info_);
    esp_efuse_mac_get_default(mac.data());
    uint32_t free_heap_size_ = esp_get_free_heap_size();

    snprintf(msg.data(),msg.size(),"%02x:%02x:%02x:%02x:%02x:%02x",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
    
    cJSON_AddStringToObject(root.get(),"chip_number",msg.data());
    cJSON_AddNumberToObject(root.get(),"number_of_memory",info_.cores);
    cJSON_AddNumberToObject(root.get(),"free_heap_size",free_heap_size_);
    cJSON_AddNumberToObject(root.get(),"total_flash",static_cast<double>(flash_size_));
    etl::unique_ptr<char,JsonString_d> json_string(cJSON_Print(root.get()));
    printf("%s",json_string.get());

}