#ifndef APP_CONTEXT_H
#define APP_CONTEXT_H

#include "esp_http_server.h"
#include "nvs_flash.h"

typedef struct
{
    httpd_handle_t server_handler;
    nvs_handle_t nvs_handler;
} app_context_t;


#endif