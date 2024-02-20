#ifndef WHEEL_SPEED_SENSOR_H
#define WHEEL_SPEED_SENSOR_H

#include <cstdint>



class WheelSpeedSensor {
    // Define the states of the wheel speed
    enum WheelSpeedState {
        STOP,
        MAINTAIN,
        DECAY
    };

public:
    // Constructor
    WheelSpeedSensor(uint8_t gpioPin, uint32_t wheelRadius);

    // Initialize the sensor
    void begin();

    // Update the sensor state and calculate speed
    void update();

    // Get the current speed
    uint32_t getSpeed();

private:
    uint8_t gpioPin; // GPIO pin connected to the sensor
    uint32_t wheelRadius; // Radius of the wheel

    uint32_t prevTime; // Previous time for calculating delta time
    uint32_t specifiedTime; // Specified time for calculating speed
    uint32_t pulseCount; // Number of pulses counted
    uint32_t wheelSpeed; // Current speed of the wheel
    WheelSpeedState state; // Current state of the wheel speed

    // Calculate the specified time based on the rotation period
    void calculateSpecifiedTime();

    // Calculate the speed based on the pulse count and time elapsed
    void calculateSpeed();

    // Handle the state transitions and speed adjustments
    void handleState();
};

#endif
