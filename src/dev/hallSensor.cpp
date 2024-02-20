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

    void hallSensor::init(){
        this->hallSensorGPIO = IO::getGPIO<IO::Pin::PC_3>();
    }

    void hallSensor::update(){
        IO::GPIO::State state = this->hallSensorGPIO.readPin();
        if(state == IO::GPIO::State::HIGH){
            this->pulseCount++;
        }
    }

    void hallSensor::calculateSpeed(){
        this->specifiedTime = EVT::core::time::millis();
        if(this->specifiedTime - this->prevTime >= 1000){
            this->wheelSpeed = (this->pulseCount * 2 * 3.14 * this->wheelRadius) / (this->specifiedTime - this->prevTime);
            this->prevTime = this->specifiedTime;
            this->pulseCount = 0;
        }
    }



}
