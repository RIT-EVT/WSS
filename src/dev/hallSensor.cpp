#include "EVT/dev/RTC.hpp"
#include "EVT/manager.hpp"
#include <DEV/hallSensor.hpp>
#include <EVT/io/GPIO.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;

constexpr uint32_t THRESHOLD = 100; // Threshold for wheel speed
constexpr uint32_t THETA = 2; // Constant for wheel speed calculation

namespace hallSensor {

    DEV::RTC& clock = DEV::getRTC();


    HallSensor::HallSensor(IO::GPIO& gpio, uint32_t wheelRadius): gpio((IO::GPIO&) gpio), wheelRadius(wheelRadius) {
        this->prevTime = 0;
        this->wheelSpeed = 0;
        this->state = WheelSpeedState::STOPPED;
        this->isHigh = false;
    }

    void HallSensor::update(){
        switch (state) {
            case WheelSpeedState::STOPPED:
                if (isHigh) {
                    // should never get here
                    break;
                }
                else{
                    prevTime = clock.getTime();
                    isHigh = true;
                    state = WheelSpeedState::MAINTAIN;
                }
            case WheelSpeedState::MAINTAIN:
                if (gpio.readPin() == EVT::core::IO::GPIO::State::HIGH) {
                    if(isHigh) {
                        break;
                    }
                    else{
                        uint32_t timeDiff = clock.getTime() - prevTime;
                        uint32_t possibleSpeed = getSpeed(timeDiff);
                        if (possibleSpeed > THRESHOLD){
                            state = WheelSpeedState::STOPPED;
                            prevTime = 0;
                            wheelSpeed = 0;
                            isHigh = false;
                        }
                        else if (possibleSpeed < wheelSpeed){
                            wheelSpeed = possibleSpeed;
                        }
                        else{
                            //do nothing
                        }
                    }
                }

        }
    }

    uint32_t HallSensor::getSpeed(uint32_t timeDiff){
        return THETA * wheelRadius / timeDiff;
    }


} // namespace hallSensor
