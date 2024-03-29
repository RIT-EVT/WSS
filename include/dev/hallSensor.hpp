#pragma once

#include <EVT/io/GPIO.hpp>

namespace IO = EVT::core::IO;

namespace hallSensor {

class hallSensor {

public:
    enum WheelSpeedState {
        STOP = 0,    // First pulse or wheel is dead
        MAINTAIN = 1,// Wheel is spinning at a constant speed or speeding up
    };

    // Constructor (take a GPIO instance and the radius of the wheel)
    hallSensor(IO::GPIO& gpio, uint32_t wheelRadius);

    uint32_t update();// Update the wheel speed

    // Get the current speed of the wheel
    uint32_t getSpeed(uint32_t timeDiff);

private:
    IO::GPIO& gpio;       // GPIO instance for the sensor
    uint32_t wheelRadius; // Radius of the wheel
    uint32_t prevTime;    // Previous time for calculating delta time
    uint32_t wheelSpeed;  // Current speed of the wheel
    WheelSpeedState state;// Current state of the wheel
    bool isHigh;          // Flag to check if the sensor is high
};

}// namespace hallSensor