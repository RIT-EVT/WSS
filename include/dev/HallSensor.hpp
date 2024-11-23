#ifndef HALLSENSOR_HPP
#define HALLSENSOR_HPP

#include <EVT/io/GPIO.hpp>

namespace IO = EVT::core::IO;

namespace WSS::DEV {

/**
 * This is the class for each individual front and back HallSensor
 */
class HallSensor {
public:
    /** The state of the wheel whether it is stopped, starting to spin, or spinning */
    enum class WheelSpeedState {
        STOP,         /** Wheel is not moving */
        INITIALIZING, /** Setting speed based on first reading */
        MAINTAIN,     /** Wheel is spinning at a constant speed or speeding up */
    };

    /** Constructor (take a GPIO instance and the radius of the wheel) */
    HallSensor(IO::GPIO& gpio, uint32_t wheelRadius, uint32_t numberOfMagnets);

    void update();// Update the sensor interval

    /** Get the current speed of the wheel */
    uint32_t getSpeed();

    /** Get the last interval of one full rotation of the wheel */
    uint32_t getRawInterval();

private:
    /** GPIO instance for the sensor */
    IO::GPIO& gpio;
    /** Radius of the wheel */
    uint32_t wheelRadius;
    /** Number of magnets in a wheel */
    uint32_t numberOfMagnets;
    /** Previous time for calculating delta time */
    uint32_t prevTime;
    /** Last fully measured interval */
    uint32_t lastInterval;
    /** Current state of the wheel */
    WheelSpeedState state;
    /** Flag to check if the sensor is high */
    bool magnetInLastRead;
    /** The magnet is detected if the pin is low */
    static constexpr IO::GPIO::State MAGNET_DETECTED_STATE = IO::GPIO::State::LOW;
    /**
     * The threshold in milliseconds for how long a magnet has not been detected before
     * setting the WheelSpeedState to STOP
     */
    static constexpr uint32_t THRESHOLD = 5000;
};

}// namespace WSS::DEV

#endif
