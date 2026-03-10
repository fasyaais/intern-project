#pragma once
#include "ap_controller.h"

using APHandler = esp_err_t (APController::*)(httpd_req_t*);

struct RouteContext
{
    APController* controller;
    APHandler handler;
};