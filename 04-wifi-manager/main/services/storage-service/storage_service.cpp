#include "storage_service.h"

StorageService::StorageService(NVSConfig &nvs, TimeService &time) : _nvs_cfg(nvs), _time_service(time)
{
}

StorageService::~StorageService()
{
}

void StorageService::init()
{
    _event_args.queue_size = 10;
    _event_args.task_name = "storage_loop_task";
    _event_args.task_stack_size = 3072;
    _event_args.task_priority = uxTaskPriorityGet(nullptr);
    _event_args.task_core_id = tskNO_AFFINITY;

    ESP_ERROR_CHECK(esp_event_loop_create(&_event_args, &_event_task));
    esp_event_handler_register_with(_event_task, STORAGE_EVENT, STORAGE_WRITE, _event_handler, this);
}

void StorageService::deinit()
{
    esp_event_handler_unregister_with(_event_task, STORAGE_EVENT, STORAGE_WRITE, _event_handler);
    _event_task = nullptr;
}

void StorageService::_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    auto *self = static_cast<StorageService *>(arg);
    gpio_t *gpioData = static_cast<gpio_t *>(data);
    auto raw_data_ptr = self->_nvs_cfg.read("data");
    if (!raw_data_ptr)
    {
        cJSON *arr = cJSON_CreateArray();
        cJSON *obj = cJSON_CreateObject();
        cJSON_AddNumberToObject(obj, "pin", gpioData->pin);
        cJSON_AddBoolToObject(obj, "state", gpioData->state);
        cJSON_AddNumberToObject(obj, "last_click_at", gpioData->last_click_at);
        cJSON_AddItemToArray(arr, obj);
        std::string str = cJSON_PrintUnformatted(arr);
        self->_nvs_cfg.write("data", str.c_str());
        cJSON_Delete(arr);

        return;
    };
    std::string raw_data = raw_data_ptr.get();
    std::string str;

    auto json = cJSON_Parse(raw_data.c_str());
    if (!json)
    {
        return;
    }

    cJSON *element = nullptr;
    bool found = false;

    cJSON_ArrayForEach(element, json)
    {
        cJSON *pin = cJSON_GetObjectItemCaseSensitive(element, "pin");
        if (pin->valueint == gpioData->pin)
        {
            cJSON_ReplaceItemInObjectCaseSensitive(element, "state", cJSON_CreateBool(gpioData->state));
            cJSON_ReplaceItemInObjectCaseSensitive(element, "last_click_at", cJSON_CreateNumber(gpioData->last_click_at));
            found = true;
            break;
        }
    }

    if (!found)
    {
        auto obj = cJSON_CreateObject();
        cJSON_AddNumberToObject(obj, "pin", gpioData->pin);
        cJSON_AddBoolToObject(obj, "state", gpioData->state);
        cJSON_AddNumberToObject(obj, "last_click_at", gpioData->last_click_at);
        cJSON_AddItemToArray(json, obj);
    }
    str = cJSON_PrintUnformatted(json);
    self->_nvs_cfg.write("data", str.c_str());
    ESP_LOGW("STRING","data : %s",str.c_str());
    cJSON_Delete(json);
}

esp_event_loop_handle_t StorageService::getHandle()
{
    return _event_task;
}