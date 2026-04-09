// #include "circular_buffer_service.h"

template <typename T>
CircularBufferService<T>::CircularBufferService(uint8_t size):_max_size(size)
{
    _mu = xSemaphoreCreateMutex();

    circ = {};
    circ.isFull = false;
    circ.r = 0;
    circ.w = 0;
}

template <typename T>
CircularBufferService<T>::~CircularBufferService()
{
}

// overwrite item if buffer is full.
template <typename T>
void CircularBufferService<T>::put(T item)
{
    if (xSemaphoreTake(_mu, pdMS_TO_TICKS(100)) == pdTRUE)
    {
        _buff[circ.w] = item;

        if (circ.isFull)
        {
            circ.r = (circ.r + 1) % _max_size;
        }

        circ.w = (circ.w + 1) % _max_size;
        circ.isFull = circ.w == circ.r;

        xSemaphoreGive(_mu);
    }
}

template <typename T>
std::optional<T> CircularBufferService<T>::get(){
    if(xSemaphoreTake(_mu,pdMS_TO_TICKS(1000)) == pdTRUE){
        if(isEmpty()){
            ESP_LOGW(_TAG,"buffer is full");
            xSemaphoreGive(_mu);
            return std::nullopt;
        }
        
        T val = _buff[circ.r];
        circ.isFull = false;
        circ.r = (circ.r + 1) % _max_size;
        xSemaphoreGive(_mu);
        return val;
    }
}


template <typename T>
bool CircularBufferService<T>::isFull()
{
    return circ.isFull;
}

template <typename T>
bool CircularBufferService<T>::isEmpty()
{
    return !circ.isFull && (circ.r == circ.w);
}