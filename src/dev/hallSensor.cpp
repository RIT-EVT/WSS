#include <EVT/io/GPIO.hpp>
#include <HALf3/stm32f3xx_hal.h>
#include <dev/hallSensor.hpp>

namespace IO = EVT::core::IO;

constexpr uint32_t THRESHOLD = 10;// Threshold for wheel speed
constexpr uint32_t THETA = 2;     // Constant for wheel speed calculation

namespace hallSensor {

hallSensor::hallSensor(IO::GPIO& gpio, uint32_t wheelRadius) : gpio((IO::GPIO&) gpio), wheelRadius(wheelRadius) {
    this->prevTime = 0;
    this->wheelSpeed = 0;
    this->state = WheelSpeedState::STOP;
    this->isHigh = false;
}

uint32_t hallSensor::update() {
    uint32_t timeDiff = 0;

    switch (state) {
    case WheelSpeedState::STOP:
        if (isHigh) {
            break;
        } else {
            prevTime = HAL_GetTick();
            isHigh = true;
            state = WheelSpeedState::MAINTAIN;
        }
    case WheelSpeedState::MAINTAIN:
        if (gpio.readPin() == EVT::core::IO::GPIO::State::LOW) {
            if (isHigh) {
                break;
            } else {
                timeDiff = HAL_GetTick() - prevTime;
                uint32_t possibleSpeed = getSpeed(timeDiff);
                if (possibleSpeed > THRESHOLD) {
                    state = WheelSpeedState::STOP;
                    prevTime = 0;
                    wheelSpeed = 0;
                    isHigh = false;
                } else if (possibleSpeed < wheelSpeed) {
                    wheelSpeed = possibleSpeed;
                }
            }
        }
        return timeDiff;
    }
    return timeDiff;
}

uint32_t hallSensor::getSpeed(uint32_t timeDiff) {
    return (THETA * wheelRadius) * timeDiff;
}

}// namespace hallSensor