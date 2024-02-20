#include <dev/hallSensor.hpp>

// Constructor
WheelSpeedSensor::WheelSpeedSensor(uint8_t gpioPin, uint32_t wheelRadius) {
    gpioPin = gpioPin;
    wheelRadius = wheelRadius;
    prevTime = 0;
    specifiedTime = 0;
    pulseCount = 0;
    wheelSpeed = 0.0;
    state = STOP;
}

// Initialize the sensor
void WheelSpeedSensor::begin() {
    pinMode(gpioPin);
}

// Update the sensor state and calculate speed
void WheelSpeedSensor::update() {
    uint32_t currentTime = RTC::millis();

    // Read the GPIO pin
    bool pulse = digitalRead(gpioPin);

    // Handle state transitions and speed calculations
    handleState();

    // Update the previous time
    prevTime = currentTime;
}

// Get the current speed
uint32_t WheelSpeedSensor::getSpeed() {
    return wheelSpeed;
}

// Calculate the specified time based on the rotation period
void WheelSpeedSensor::calculateSpecifiedTime() {
    // Here, you would calculate the specified time based on the rotation period
    // This could involve some calibration or measurement of the wheel's rotation period
    // For the pseudocode, we'll just assume a fixed time for simplicity
    specifiedTime = 1000; // Placeholder value in milliseconds
}

// Calculate the speed based on the pulse count and time elapsed
void WheelSpeedSensor::calculateSpeed() {
    // Calculate the angular velocity (omega) using the formula: omega = delta(theta) / delta(t)
    // In this pseudocode, we're using a simplified version where delta(t) is the specified time
    float angularVelocity = pulseCount / (specifiedTime / 1000.0); // Convert milliseconds to seconds

    // Calculate the linear velocity (V) using the formula: V = radius * omega
    wheelSpeed = wheelRadius * angularVelocity;
}

// Handle the state transitions and speed adjustments
void WheelSpeedSensor::handleState() {
    // Read the GPIO pin
    bool pulse = digitalRead(gpioPin);

    // Check the current state and update accordingly
    switch (state) {
    case STOP:
        if (pulse) {
            state = MAINTAIN;
            pulseCount = 1; // Start counting pulses
            calculateSpecifiedTime();
        }
        break;

    case MAINTAIN:
        if (pulse) {
            pulseCount++; // Increment pulse count
        } else {
            state = DECAY;
        }
        break;

    case DECAY:
        if (!pulse) {
            state = STOP;
            calculateSpeed();
        }
        break;
    }
}
