#include "rfid_driver.h"

RFIDDriver::RFIDDriver(
    pn532_io_t& handle
):
    _handle(handle)
{}

esp_err_t RFIDDriver::init()
{
    esp_err_t err;
    err = pn532_init(&_handle);
    return err;
}

void RFIDDriver::start(void *args)
{
    auto* it = static_cast<RFIDDriver*>(args);
    uint8_t uid[7];
    uint8_t uidLen;

    while (true) {
        if (pn532_read_passive_target_id(&it->_handle, 0x00, uid, &uidLen, 1000) == ESP_OK) {
            ESP_LOGI("RFID", "Kartu Terdeteksi!");
            ESP_LOG_BUFFER_HEX("UID", uid, uidLen);
        } else {
            ESP_LOGD("RFID", "Menunggu kartu...");
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    free(uid);
}