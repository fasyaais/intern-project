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
    int n = 0;

    uart_packet_t state = PACKET_HEAD;
    int sum = 0;
    std::vector<uint8_t> data_string;
    int data_length = 0;
    int count = 0;
    
    while (1) {
        if(xQueueReceive(self->_uart_queue, &event, 20/ portTICK_PERIOD_MS) == pdTRUE){
            if (event.type == UART_DATA)
            {
                int len = uart_read_bytes(self->_uart_num,&data,event.size,portMAX_DELAY);

                // if(state == PACKET_DATA) state = PACKET_LENGTH;
                
                for (size_t i = 0; i < len; i++)
                {
                    switch (state)
                    {
                    case PACKET_HEAD:
                        if(data[i] == 0x02){
                            data_length = 0;
                            count = 0;
                            data_string.clear();
                            sum = 0;
                            state = PACKET_LENGTH_HIGH;
                        };
                        break;
                    case PACKET_LENGTH_HIGH :
                        data_length = (data[i] << 8);
                        state = PACKET_LENGTH_LOW;
                        break;
                    case PACKET_LENGTH_LOW :
                        data_length |= data[i];
                        state = PACKET_DATA;
                        break;
                    case PACKET_DATA:
                        data_string.push_back(data[i]);
                        sum ^= data[i];
                        count++;
                        ESP_LOGW("response","i = %d",i);
                        if(count >= data_length) state = PACKET_CHECKSUM;
                        break;
                    case PACKET_CHECKSUM:
                        if(sum == data[i]){
                            ESP_LOGW("response","Valid checksum");
                            state = PACKET_TAIL;
                        }else {
                            state = PACKET_HEAD;
                        }
                        break;
                    case PACKET_TAIL:
                        if(data[i] == 0x03){
                            printf("%.*s\n",data_string.size(),data_string.data());
                            // ESP_LOGI("response","data = %.*s",data_string.size(),data_string.data());
                        }    
                        state = PACKET_HEAD;
                        break;
                    default:
                        break;
                    }
                    // ESP_LOGI("leNGTH","length = %d, hex = %02x",len,data[i]);
                }
                // n += len;
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