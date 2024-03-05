#include <DEV/hallSensor.hpp>
#include <EVT/dev/RTC.hpp>
#include <EVT/io/GPIO.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;

constexpr uint32_t THRESHOLD = 10; // Threshold for wheel speed
constexpr uint32_t FRONT_THETA = 2; // Distance Between Magnets for Front Wheel
constexpr uint32_t REAR_THETA = 2; // Distance Between Magnets for Rear Wheel

namespace hallSensor {

    HallSensor::HallSensor(IO::GPIO& gpio, uint32_t wheelRadius, DEV::RTC& clock) : gpio(gpio), wheelRadius(wheelRadius), clock(clock) {
        state = WheelSpeedState::STOPPED;
        prevTime = 0;
        wheelSpeed = 0;
        gpio.registerIRQ(IO::GPIO::TriggerEdge::RISING, risingEdgeHandler);
    }

    void HallSensor::risingEdgeHandler(IO::GPIO* pin) {
        uint32_t currentTime = clock.getTime();
        uint32_t timeDiff = currentTime - prevTime;

        // Update previous time
        prevTime = currentTime;

        // Calculate speed
        uint32_t currentSpeed = getSpeed(timeDiff);

        // Update wheel speed if it exceeds the threshold
        if (currentSpeed >= THRESHOLD) {
            wheelSpeed = currentSpeed;
            state = WheelSpeedState::MAINTAIN;
        } else {
            state = WheelSpeedState::STOPPED;
            wheelSpeed = 0;
        }
    }

    HallSensor::WheelSpeedState HallSensor::update(){
        //process the wheel speed, focus on if a rising edge is deteced or a falling edge


    };

    uint32_t HallSensor::getSpeed(uint32_t timeDiff){
        return (FRONT_THETA * wheelRadius) / timeDiff;
    }

    HallSensor::WheelSpeedState HallSensor::getState(){
        return state;
    }

} // namespace hallSensor
