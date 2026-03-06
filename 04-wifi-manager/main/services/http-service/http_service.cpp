#include "http_service.h"

HTTPService::HTTPService(NVSConfig* nvsConfig):_nvsConfig(nvsConfig),_config(HTTPD_DEFAULT_CONFIG()){};

esp_err_t HTTPService::start(){
    return httpd_start(&server,&_config);
}

esp_err_t HTTPService::registerURI(httpd_uri_t* uri){
    return httpd_register_uri_handler(server,uri);
}