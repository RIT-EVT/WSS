#include "EVT/dev/RTC.hpp"
#include "EVT/manager.hpp"
#include <DEV/hallSensor.hpp>
#include <EVT/dev/RTCTimer.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/io/GPIO.hpp>

using namespace EVT::core::IO;
using namespace EVT::core::DEV;

namespace hallSensor {

    RTC& clock = getRTC();

    HallSensor::HallSensor(GPIO* gpio, uint32_t wheelRadius) {
        this->gpio = gpio;
        this->pulseCount = 0;
        this->wheelSpeed = 0;
        this->state = WheelSpeedState::STOP;
        this->wheelRadius = wheelRadius;
        this->prevTime = 0;
        this->timer = RTCTimer(clock, 1000);
    }

    void HallSensor::begin() {
        // Set the GPIO pin as input
    }

    bool HallSensor::hasCompletedRotation() {
        return calculateSpecifiedTime() >= timeForOneRotation;
    }

    void HallSensor::update() {
        GPIO::State pinState = gpio->readPin();

        if (pinState == GPIO::State::HIGH){
            if (state == WheelSpeedState::STOP) {
                // First pulse, set the state to MAINTAIN
                state = WheelSpeedState::MAINTAIN;
            }
            pulseCount++;
            wheelSpeed = calculateSpeed();
            // Check if a full rotation is completed
            if (hasCompletedRotation()) {
                // Restart the timer for the next rotation
                timer.reloadTimer();
            }
        } else {
            // Implement linear decay if no rotation happens within the specified time
            if (state != WheelSpeedState::STOP && !hasCompletedRotation()) {
                state = WheelSpeedState::DECAY;
                wheelSpeed--; // Perform linear decay
                if (wheelSpeed < 0) {
                    wheelSpeed = 0; // Ensure speed does not go negative
                }
            }
        }
    }

    uint32_t HallSensor::getSpeed() {
        return this->wheelSpeed;
    }

    uint32_t HallSensor::calculateSpecifiedTime() {
        return timer.getTime() - prevTime;
    }

    uint32_t HallSensor::calculateSpeed() {
        // Calculate speed based on pulse count and time elapsed
        uint32_t specifiedTime = calculateSpecifiedTime();
        double rotations = pulseCount / double(360); // Assuming 1 pulse = 1 rotation
        double distance = 2 * 3.14159 * wheelRadius * rotations; // Circumference = 2 * pi * radius
        double speed = distance / specifiedTime; // Speed = Distance / Time
        return static_cast<uint32_t>(speed);
    }

} // namespace hallSensor
