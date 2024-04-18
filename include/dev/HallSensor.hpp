#pragma once

#include <EVT/io/GPIO.hpp>

namespace IO = EVT::core::IO;

namespace WSS::DEV {

class HallSensor {

public:
    enum WheelSpeedState {
        STOP = 0,    // First pulse or wheel is dead
        INITIALIZING = 1, // Setting speed based on first reading
        MAINTAIN = 2,// Wheel is spinning at a constant speed or speeding up
    };

    // Constructor (take a GPIO instance and the radius of the wheel)
    HallSensor(IO::GPIO& gpio, uint32_t wheelRadius);

    void update();// Update the wheel speed

    // Get the current speed of the wheel
    uint32_t getSpeed();

private:
    IO::GPIO& gpio;       // GPIO instance for the sensor
    uint32_t wheelRadius; // Radius of the wheel
    uint32_t prevTime;    // Previous time for calculating delta time
    uint32_t lastInterval; // Last fully measured interval
    WheelSpeedState state;// Current state of the wheel
    bool magnetInLastRead;          // Flag to check if the sensor is high

    IO::GPIO::State MAGNET_DETECTED_STATE = IO::GPIO::State::LOW;
};

}// namespace WSS::DEV