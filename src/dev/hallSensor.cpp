#include <dev/hallSensor.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/dev/RTCTimer.hpp>


namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

namespace hallSensor{

    hallSensor::hallSensor(GPIO* gpio, uint32_t wheelRadius) {
        this->gpio = gpio;
        this->pulseCount = 0;
        this->wheelSpeed = 0;
        this->state = WheelSpeedState::STOP;
        this->wheelRadius = wheelRadius;
        this->prevTime = 0;
        this->timer = DEV::getRTC();
    }

    void hallSensor::begin() {
        // Set the GPIO pin as input
    }

    bool hallSensor::hasCompletedRotation() {
        return calculateSpecifiedTime() >= timeForOneRotation;
    }


    void hallSensor::update(){
        State pinState = gpio->readPin();

        if (pinState == State::HIGH) {
            pulseCount++;
            wheelSpeed = calculateSpeed();
            // Check if a full rotation is completed
            if (hasCompletedRotation()) {
                // Calculate wheel speed
                wheelSpeed = calculateSpeed();
                // Reset pulse count
                pulseCount = 0;
                // Restart the timer for the next rotation
                timer.reloadTimer();
            }
        } else {
            // Implement linear decay if no rotation happens within the specified time
            if (!hasCompletedRotation()) {
                wheelSpeed--; // Perform linear decay
                if (wheelSpeed < 0) {
                    wheelSpeed = 0; // Ensure speed does not go negative
                }
            }
        }
    }

    uint32_t hallSensor::getSpeed(){
        return this->wheelSpeed;
    }

    uint32_t hallSensor::calculateSpecifiedTime() {
        // Use RTC Timer to get elapsed time
        return timer.getTime();
    }

    uint32_t hallSensor::calculateSpeed() {
        // Calculate speed based on pulse count and time elapsed
        uint32_t specifiedTime = calculateSpecifiedTime();
        double rotations = pulseCount / double(360); // Assuming 1 pulse = 1 rotation
        double distance = 2 * 3.14159 * wheelRadius * rotations; // Circumference = 2 * pi * radius
        double speed = distance / specifiedTime; // Speed = Distance / Time
        return static_cast<uint32_t>(speed);
    }





}
