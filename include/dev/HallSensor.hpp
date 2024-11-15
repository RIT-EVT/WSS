#pragma once

#include <EVT/io/GPIO.hpp>

namespace IO = EVT::core::IO;

namespace WSS::DEV {

class HallSensor {

public:
    enum class WheelSpeedState {
        STOP,        // First pulse or wheel is dead
        INITIALIZING,// Setting speed based on first reading
        MAINTAIN,    // Wheel is spinning at a constant speed or speeding up
    };

    // Constructor (take a GPIO instance and the radius of the wheel)
    HallSensor(IO::GPIO& gpio, uint32_t wheelRadius);

    void update();// Update the wheel speed

    // Get the current speed of the wheel
    uint32_t getSpeed();

    // Get the last interval of one full rotation of the wheel
    uint32_t getRawInterval();

private:
    /** GPIO instance for the sensor */
    IO::GPIO& gpio;
    /** Radius of the wheel */
    uint32_t wheelRadius;
    /** Previous time for calculating delta time */
    uint32_t prevTime;
    /** Last fully measured interval */
    uint32_t lastInterval;
    /** Current state of the wheel */
    WheelSpeedState state;
    /** Flag to check if the sensor is high */
    bool magnetInLastRead;
    /** Flag to check whether the magnet was just detected */
    bool magnetWasDetected;
    /** The magnet is detected if the pin is low */
    const IO::GPIO::State MAGNET_DETECTED_STATE = IO::GPIO::State::LOW;
};

}// namespace WSS::DEV