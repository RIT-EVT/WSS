#pragma once

#include <EVT/io/GPIO.hpp>
#include <EVT/DEV/RTCTimer.hpp>

using namespace EVT::core::IO;
using namespace EVT::core::DEV;

namespace hallSensor {

class HallSensor {
    // Define the states of the wheel speed
    enum WheelSpeedState {
        STOP = 0,    // First pulse or wheel is dead
        MAINTAIN = 1,// Wheel is spinning at a constant speed or speeding up
        DECAY = 2    // Wheel is slowing down
    };

public:
    // Constructor (take a GPIO instance and the radius of the wheel)
    HallSensor(GPIO* gpio, uint32_t wheelRadius);

    // Initialize the sensor
    void begin();

    // Update the sensor state and calculate speed
    void update();

    // Get the current speed
    uint32_t getSpeed();

private:
    GPIO* gpio;            // GPIO instance for the sensor
    uint32_t wheelRadius;  // Radius of the wheel

    uint32_t prevTime;     // Previous time for calculating delta time
    uint32_t pulseCount;   // Number of pulses counted
    uint32_t wheelSpeed;   // Current speed of the wheel
    WheelSpeedState state; // Current state of the wheel speed
    RTCTimer timer;        // Timer for calculating time

    // Calculate the specified time based on the rotation period
    uint32_t calculateSpecifiedTime();

    // Calculate the speed based on the pulse count and time elapsed
    uint32_t calculateSpeed();

    // Check if a full rotation is completed
    bool hasCompletedRotation();
    RTCTimer timer;
};

} // namespace hallSensor
