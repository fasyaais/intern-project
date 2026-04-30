#include "led_service.h"

LedService::LedService(TimeService& timeService,GPIOManager& gpioManager,HTTPClientService& httpClientService,StorageService& storage):_timeService(timeService),_gpioManager(gpioManager),_httpClientService(httpClientService),_storageService(storage){}

// LedService::LedService(LedDriver& ledDriver,ButtonDriver& buttonDriver,TimeService& timeService,HTTPClientService& httpClientService):_ledDriver(ledDriver),_buttonDriver(buttonDriver),_timeService(timeService),_httpClientService(httpClientService){}

void LedService::init(){
    for(;;){
        if(_button->click()){
            if(_led->isON()){
                _led->turnOFF();
            }else{
                _led->turnON();
            }
            auto* json= cJSON_CreateObject();
            gpio_t data = {
                .pin = _led->getPin(),
                .state = _led->isON(),
                .last_click_at = _timeService.getNow() * 1000,
            };
            esp_event_post_to(_storageService.getHandle(),STORAGE_EVENT,STORAGE_WRITE,&data,sizeof(data),portMAX_DELAY);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void LedService::blink(){
    for(;;){
        if(_led->isON()) _led->turnOFF();
        else _led->turnON();

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// void LedService::init(){
//     for(;;){
//         if(_buttonDriver.click()){
//             if(_ledDriver.isON()){
//                 _ledDriver.turnOFF();
//             }else{
//                 _ledDriver.turnON();
//             }
//             // cJSON* json = cJSON_CreateObject();
//             auto json = std::unique_ptr<cJSON,decltype(&cJSON_Delete)>(cJSON_CreateObject(),cJSON_Delete);
//             auto jsonState = std::unique_ptr<cJSON,decltype(&cJSON_Delete)>(cJSON_CreateObject(),cJSON_Delete);
//             cJSON_AddStringToObject(json.get(),"state",_ledDriver.isON() ?"ON":"OFF");
//             cJSON_AddNumberToObject(json.get(),"timestamp",_timeService.getNow());
//             cJSON_AddNumberToObject(jsonState.get(),"pin",_ledDriver.getPin());
//             cJSON_AddBoolToObject(jsonState.get(),"state",_ledDriver.isON());
//             _httpClientService.post(json.get());
//             _httpClientService.postGPIO(jsonState.get(),"http://"PATH":3000/api/gpio/state");
//         }
//         vTaskDelay(pdMS_TO_TICKS(10));
//     }
// }

// void LedService::blink(){
//     for(;;){
//         if(_ledDriver.isON()) _ledDriver.turnOFF();
//         else _ledDriver.turnON();

//         vTaskDelay(pdMS_TO_TICKS(500));
//     }
// }

