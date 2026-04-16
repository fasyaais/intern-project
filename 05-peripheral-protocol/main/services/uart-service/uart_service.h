#ifndef UART_SERVICE_H
#define UART_SERVICE_H

#include "freertos/FreeRTOS.h"
#include "driver/uart.h"
#include "vector"
#include "array"
#include "iostream"

enum uart_packet_t {
    PACKET_HEAD,
    PACKET_LENGTH_HIGH,
    PACKET_LENGTH_LOW,
    PACKET_DATA,
    PACKET_CHECKSUM,
    PACKET_TAIL,
} ;

class UartService
{
private:
    QueueHandle_t _uart_queue;
    uart_port_t _uart_num;
    static constexpr const char* _TAG = "uart_service";
public:
    UartService(uart_port_t num);
    ~UartService();

    static void start(void *args);
    QueueHandle_t* getQueueHandle();
    uart_port_t getUartNum();
};




#endif