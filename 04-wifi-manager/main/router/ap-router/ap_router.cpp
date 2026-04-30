#include "ap_router.h"

APRouter::APRouter(APController& controller):_controller(controller){}

APRouter::~APRouter(){
    ESP_LOGI(_TAG,"Router close");
}

void APRouter::registerRouter(httpd_handle_t server){
    static router_context_t scanCtx {
        &_controller,
        &APController::apiScanAP
    };
    static httpd_uri_t api_scan = {
        .uri = "/api/scan",
        .method = HTTP_GET,
        .handler = routerWrapper,
        .user_ctx = &scanCtx
    };
    httpd_register_uri_handler(server,&api_scan);

    static router_context_t connectAPCtx {
        &_controller,
        &APController::connectAP
    };
    static httpd_uri_t connect_ap_api = {
        .uri = "/api/connect",
        .method = HTTP_POST,
        .handler = routerWrapper,
        .user_ctx = &connectAPCtx
    };
    httpd_register_uri_handler(server,&connect_ap_api);

    static router_context_t indexHTML {
        &_controller,
        &APController::indexHTML
    };
    static httpd_uri_t index_html_api = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = routerWrapper,
        .user_ctx = &indexHTML
    };
    httpd_register_uri_handler(server,&index_html_api);

    static router_context_t indexCSSCtx {
        &_controller,
        &APController::indexCSS
    };
    
    static httpd_uri_t index_css_api = {
        .uri = "/assets/index-Bfr4IOl2.css",
        .method = HTTP_GET,
        .handler = routerWrapper,
        .user_ctx = &indexCSSCtx
    };
    httpd_register_uri_handler(server,&index_css_api);

    static router_context_t appJsCtx {
        &_controller,
        &APController::appJS
    };
    static httpd_uri_t app_js_api = {
        .uri = "/assets/index-BFKvy6o_.js",
        .method = HTTP_GET,
        .handler = routerWrapper,
        .user_ctx = &appJsCtx
    };
    httpd_register_uri_handler(server,&app_js_api);

    return;
}

void APRouter::registerRouterStation(httpd_handle_t server){

    static router_context_t indexHTML {
        &_controller,
        &APController::appUIHTML
    };

    static httpd_uri_t index_html_api = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = routerWrapper,
        .user_ctx = &indexHTML
    };
    httpd_register_uri_handler(server,&index_html_api);

    static router_context_t indexCSSCtx {
        &_controller,
        &APController::appUICSS
    };
    
    static httpd_uri_t index_css_api = {
        .uri = "/assets/index-BXeNCRoy.css",
        .method = HTTP_GET,
        .handler = routerWrapper,
        .user_ctx = &indexCSSCtx
    };
    httpd_register_uri_handler(server,&index_css_api);

    static router_context_t appJsCtx {
        &_controller,
        &APController::appUIJS
    };
    static httpd_uri_t app_js_api = {
        .uri = "/assets/index-DalG0i6k.js",
        .method = HTTP_GET,
        .handler = routerWrapper,
        .user_ctx = &appJsCtx
    };
    httpd_register_uri_handler(server,&app_js_api);
    
    static router_context_t apiScanGPIO {
        &_controller,
        &APController::apiScanGPIO
    };
    static httpd_uri_t scan_gpio_uri = {
        .uri = "/api/gpio",
        .method = HTTP_GET,
        .handler = routerWrapper,
        .user_ctx = &apiScanGPIO
    };
    httpd_register_uri_handler(server,&scan_gpio_uri);

    static router_context_t apiUpdateGPIO {
        &_controller,
        &APController::updateGPIO
    };
    static httpd_uri_t updateGPIO = {
        .uri = "/api/gpio/state",
        .method = HTTP_POST,
        .handler = routerWrapper,
        .user_ctx = &apiUpdateGPIO
    };
    httpd_register_uri_handler(server,&updateGPIO);

    static router_context_t create_gpio_ctx {
        &_controller,
        &APController::createGPIO
    };

    static httpd_uri_t createGPIO = {
        .uri = "/api/gpio",
        .method = HTTP_POST,
        .handler = routerWrapper,
        .user_ctx = &create_gpio_ctx
    };
    httpd_register_uri_handler(server,&createGPIO);
    
    static router_context_t delete_gpio_ctx {
        &_controller,
        &APController::deleteGPIO
    };

    static httpd_uri_t delete_gpio_uri = {
        .uri = "/api/gpio",
        .method = HTTP_DELETE,
        .handler = routerWrapper,
        .user_ctx = &delete_gpio_ctx
    };
    httpd_register_uri_handler(server,&delete_gpio_uri);

    return;
}