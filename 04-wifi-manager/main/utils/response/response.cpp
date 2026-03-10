#include "response.h"

static const char* TAG = "response util";

namespace response{
    void sendJSON(httpd_req_t* req,cJSON* data){
        auto json = std::unique_ptr<cJSON,decltype(&cJSON_Delete)>(cJSON_CreateObject(),cJSON_Delete); 
        cJSON_AddItemToObject(json.get(),"data",data);
        char* str = cJSON_PrintUnformatted(json.get());
        httpd_resp_set_type(req,"application/json");
        httpd_resp_send(req,str,strlen(str));
        free(str);
    }
    
    cJSON* readJSON(httpd_req_t* req){
        const size_t content_len = req->content_len;
        std::string buff(content_len,'\0');
        int ret = httpd_req_recv(req,buff.data(),content_len);
        if(ret <= 0){
            ESP_LOGE(TAG,"Failed to receive data");
        }
        cJSON* json = cJSON_Parse(buff.data());
        return json;
    }
}
