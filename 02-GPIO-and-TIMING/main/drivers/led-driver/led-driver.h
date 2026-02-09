#ifndef LED_DRIVER_H
#define LED_DRIVER_H



class LedDriver
{
private:
    bool _pin;
public:
    bool isOn();
    LedDriver(/* args */);
};

#endif