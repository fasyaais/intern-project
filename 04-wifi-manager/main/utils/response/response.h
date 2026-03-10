#ifndef RESPONSE_UTIL_H
#define RESPONSE_UTIL_H

#include "esp_http_server.h"
#include <cJSON.h>
#include <string>
#include "esp_log.h"
#include "memory"

namespace response {

void sendJSON(httpd_req_t* req, cJSON* data);

cJSON* readJSON(httpd_req_t* req);

}

#endif