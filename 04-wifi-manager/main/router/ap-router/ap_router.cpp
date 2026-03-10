#include "ap_router.h"

APRouter::APRouter(APController& controller):_controller(controller){}

void APRouter::registerRouter(httpd_handle_t server){
    static RouteContext scanCtx {
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

    static RouteContext connectAPCtx {
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

    static RouteContext indexHTML {
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

    static RouteContext indexCSSCtx {
        &_controller,
        &APController::indexCSS
    };
    static httpd_uri_t index_css_api = {
        .uri = "/assets/index-CQOd6Co-.css",
        .method = HTTP_GET,
        .handler = routerWrapper,
        .user_ctx = &indexCSSCtx
    };
    httpd_register_uri_handler(server,&index_css_api);

    static RouteContext appJsCtx {
        &_controller,
        &APController::appJS
    };
    static httpd_uri_t app_js_api = {
        .uri = "/assets/index-19cTNh9z.js",
        .method = HTTP_GET,
        .handler = routerWrapper,
        .user_ctx = &appJsCtx
    };
    httpd_register_uri_handler(server,&app_js_api);

    return;
}