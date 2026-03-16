#include "led_service.h"

LedService::LedService(LedDriver& ledDriver,ButtonDriver& buttonDriver,TimeService& timeService,HTTPClientService& httpClientService):_ledDriver(ledDriver),_buttonDriver(buttonDriver),_timeService(timeService),_httpClientService(httpClientService){}

void LedService::init(){
    for(;;){
        if(_buttonDriver.click()){
            if(_ledDriver.isON()){
                _ledDriver.turnOFF();
            }else{
                _ledDriver.turnON();
            }
            // cJSON* json = cJSON_CreateObject();
            auto json = std::unique_ptr<cJSON,decltype(&cJSON_Delete)>(cJSON_CreateObject(),cJSON_Delete);
            cJSON_AddStringToObject(json.get(),"state",_ledDriver.isON() ?"ON":"OFF");
            cJSON_AddNumberToObject(json.get(),"timestamp",_timeService.getNow());
            _httpClientService.post(json.get());
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void LedService::blink(){
    for(;;){
        if(_ledDriver.isON()) _ledDriver.turnOFF();
        else _ledDriver.turnON();

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}