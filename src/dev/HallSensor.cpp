#include <EVT/io/GPIO.hpp>
#include <HALf3/stm32f3xx_hal.h>
#include <dev/HallSensor.hpp>

namespace IO = EVT::core::IO;

constexpr uint32_t THRESHOLD = 1000;// Threshold for wheel speed
constexpr uint32_t WHEELTHRESHOLD = 10; // Threshold for wheel speed
constexpr uint32_t THETA = 2;     // Constant for wheel speed calculation

namespace WSS::DEV {

HallSensor::HallSensor(IO::GPIO& gpio, uint32_t wheelRadius) : gpio((IO::GPIO&) gpio), wheelRadius(wheelRadius) {
    this->prevTime = 0;
    this->wheelSpeed = 0;
    this->state = WheelSpeedState::STOP;
    this->isActive = false;
}

uint32_t HallSensor::update() {
    uint32_t timeDiff = 0;

    switch (state) {
    case WheelSpeedState::STOP:
        if (gpio.readPin() == EVT::core::IO::GPIO::State::LOW){
            if (isActive) {
                break;
            } else {
                prevTime = HAL_GetTick();
                isActive = true;
                state = WheelSpeedState::INITIALIZING;
            }
        } else {
            isActive = false;
        }
        break;
    case WheelSpeedState::INITIALIZING:
        if (gpio.readPin() == EVT::core::IO::GPIO::State::LOW) {
            if (isActive) {
                if (HAL_GetTick() - prevTime > THRESHOLD) {
                    state = WheelSpeedState::STOP;
                    prevTime = 0;
                }
            } else {
                uint32_t now = HAL_GetTick();
                lastInterval = now - prevTime;
                prevTime = now;
                isActive = true;
                state = WheelSpeedState::MAINTAIN;
            }
        } else {
            isActive = false;
            if (HAL_GetTick() - prevTime > THRESHOLD) {
                state = WheelSpeedState::STOP;
                prevTime = 0;
            }
        }
        break;
    case WheelSpeedState::MAINTAIN:
        if (gpio.readPin() == EVT::core::IO::GPIO::State::LOW) {
            if(isActive) {
                break;
            } else {
                uint32_t now = HAL_GetTick();
                lastInterval = now - prevTime;
                prevTime = now;
                isActive = true;
                state = WheelSpeedState::MAINTAIN;
            }
        } else {
            timeDiff = HAL_GetTick() - prevTime;
            uint32_t possibleSpeed = getSpeed(timeDiff);
            if (possibleSpeed < WHEELTHRESHOLD) {
                state = WheelSpeedState::STOP;
                isActive = false;
                prevTime = 0;
                wheelSpeed = 0;
            }
            else if (possibleSpeed > wheelSpeed) {
                wheelSpeed = possibleSpeed;
            }
            else {}
        }
        break;
    }
    return timeDiff;
}

uint32_t HallSensor::getSpeed(uint32_t timeDiff) {
    return (THETA * wheelRadius) * timeDiff;
}

}// namespace WSS::DEV