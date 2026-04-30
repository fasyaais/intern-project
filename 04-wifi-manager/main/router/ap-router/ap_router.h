#ifndef AP_ROUTER_H
#define AP_ROUTER_H

#include "http_service.h"
#include "ap_controller.h"
#include "router_context.h"
#include "router_wrapper.h"

class APRouter
{
private:
    APController& _controller; 
    static constexpr const char* _TAG = "ap router";
public:
    APRouter(APController& controller);
    ~APRouter();
    void registerRouter(httpd_handle_t server);
    void registerRouterStation(httpd_handle_t server);
};

#endif