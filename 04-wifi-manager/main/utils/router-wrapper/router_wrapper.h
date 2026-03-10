#pragma once

#include "esp_http_server.h"
#include "router_context.h"

typedef esp_err_t (*RouterHandler)(void*, httpd_req_t*);

esp_err_t routerWrapper(httpd_req_t* req);