#pragma once
class APController;

using APHandler = esp_err_t (APController::*)(httpd_req_t*);

struct router_context_t
{
    APController* controller;
    APHandler handler;
};