#ifndef STORAGE_EVENT_H
#define STORAGE_EVENT_H

#include "esp_event.h"

enum {
    STORAGE_WRITE
};

ESP_EVENT_DECLARE_BASE(STORAGE_EVENT);

#endif