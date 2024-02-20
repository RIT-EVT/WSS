#include <dev/hallSensor.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>

namespace IO = EVT::core::IO;

namespace hallSensor{

    hallSensor::hallSensor(uint8_t gpioPin, uint32_t wheelRadius){
        this->gpioPin = gpioPin;
        this->wheelRadius = wheelRadius;
        this->prevTime = 0;
        this->specifiedTime = 0;
        this->pulseCount = 0;
        this->wheelSpeed = 0;
        this->state = WheelSpeedState::STOP;
    }


}
