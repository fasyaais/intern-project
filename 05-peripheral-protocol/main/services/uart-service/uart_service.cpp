#include "uart_service.h"

UartService::UartService(uart_port_t num):_uart_num(num)
{
}

UartService::~UartService()
{
}

void UartService::start(void *args)
{
    auto self = static_cast<UartService*>(args);
    uart_event_t event;
    std::array<uint8_t,1024>data;
    
    while (1) {
        if(xQueueReceive(self->_uart_queue,&event, 20/ portTICK_PERIOD_MS) == pdTRUE){
            if (event.type == UART_DATA)
            {
                int len = uart_read_bytes(self->_uart_num,&data,event.size,portMAX_DELAY);
                uart_packet_t state = PACKET_HEAD;
                int data_length = 0;
                int sum = 0;
                int count = 0;
                std::vector<uint8_t> data_string;
                
                for (size_t i = 0; i < len; i++)
                {
                    switch (state)
                    {
                    case PACKET_HEAD:
                        if(data[i] == 0x02) state = PACKET_LENGTH;
                        break;
                    case PACKET_LENGTH :
                        data_length = data[i];
                        state = PACKET_DATA;
                        break;
                    case PACKET_DATA:
                        data_string.push_back(data[i]);
                        sum ^= data[i];
                        count++;
                        // ESP_LOGW("response","data = %.*s",data[i]);
                        if(count >= data_length) state = PACKET_CHECKSUM;
                        break;
                    case PACKET_CHECKSUM:
                        // ESP_LOGW("response","sum = %02X, chc = %02X",sum,data[i]);
                        if(sum == data[i]) state = PACKET_TAIL;
                        break;
                    case PACKET_TAIL:
                        if(data[i] == 0x03){
                            state = PACKET_HEAD;
                            ESP_LOGI("response","data = %.*s",data_string.size(),data_string.data());
                        }    
                        break;
                    default:
                        break;
                    }
                    // ESP_LOGI("leNGTH","length = %d, hex = %02x",len,data[i]);
                }
                // printf("hello\n");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

QueueHandle_t* UartService::getQueueHandle(){
    return &_uart_queue;
}

uart_port_t UartService::getUartNum(){
    return _uart_num;
}