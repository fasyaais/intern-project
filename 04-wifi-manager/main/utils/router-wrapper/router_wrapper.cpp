#include "router_wrapper.h"

esp_err_t routerWrapper(httpd_req_t* req){
    auto* ctx = static_cast<router_context_t*>(req->user_ctx);
    return (ctx->controller->*(ctx->handler))(req);
}