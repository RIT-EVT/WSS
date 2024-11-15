#include <EVT/io/GPIO.hpp>
#include <EVT/utils/log.hpp>
#include <HALf3/stm32f3xx_hal.h>
#include <dev/HallSensor.hpp>
#include <string>
#include <valarray>

namespace IO = EVT::core::IO;

// TODO Tune these values
constexpr uint32_t THRESHOLD = 5000;// Threshold for wheel speed

namespace WSS::DEV {

HallSensor::HallSensor(IO::GPIO& gpio, uint32_t wheelRadius, uint32_t numberOfMagnets) : gpio((IO::GPIO&) gpio), wheelRadius(wheelRadius), numberOfMagnets(numberOfMagnets) {
    this->prevTime = 0;
    this->lastInterval = 0;
    this->state = WheelSpeedState::STOP;
    this->magnetInLastRead = false;
}

/**
 *
 * 1: Use a separate nucleo to send a 3.3V pulse to the nucleo running THIS every x amount of time to test
 * the algorithm reliably. by setting whatever pin to high and then to low (good luck).
 *  coded in the 'blink' part of rampup because i didn't know how to make new project thing configuration
 * 2: FIX THE ALGORITHM
 *
 */

void HallSensor::update() {
    uint32_t now = HAL_GetTick();
    uint32_t elapsedTime = now - prevTime;

    // The magnet is detected if the previous gpio pin reading is the same as the current gpio
    // pin reading
    const bool magnetDetected = gpio.readPin() == MAGNET_DETECTED_STATE;
    bool magnetDetectedNow = false;
    if (!magnetWasDetected && magnetDetected != magnetInLastRead && magnetDetected) {
        magnetWasDetected = true;
        magnetDetectedNow = true;
    } else if (magnetWasDetected && magnetDetected == magnetInLastRead) {
        magnetWasDetected = false;
    }
    magnetInLastRead = magnetDetected;

    switch (state) {
    case WheelSpeedState::STOP:
        if (magnetDetectedNow) {
            prevTime = now;
            state = WheelSpeedState::INITIALIZING;
        }
        break;

    case WheelSpeedState::INITIALIZING:
        if (magnetDetectedNow) {
            // If the magnet is detected for the second time, record the time and the first
            // interval and enter the maintain state
            lastInterval = elapsedTime;
            prevTime = now;
            state = WheelSpeedState::MAINTAIN;
        }

        // If the magnet hasn't been detected for a significant amount of time, go back to the stop
        // state
        if (elapsedTime > THRESHOLD) {
            state = WheelSpeedState::STOP;
            prevTime = 0;
        }
        break;

    case WheelSpeedState::MAINTAIN:
        if (magnetDetectedNow) {
            // If the magnet is newly detected, record the time and interval
            lastInterval = elapsedTime;
            prevTime = now;
        }

        // If the magnet hasn't been detected for a significant amount of time, go back to the stop
        // state
        if (elapsedTime > THRESHOLD) {
            state = WheelSpeedState::STOP;
            prevTime = 0;
            lastInterval = 0;
            // If the elapsed time since the magnet was last detected exceeds the last interval, record
            // the last interval as if the magnet was just detected because the bike is slowing
            // down, but the exact speed can't be calculated
        } else if (elapsedTime > lastInterval) {
            lastInterval = elapsedTime;
        }
        break;
    }
}

uint32_t HallSensor::getRawInterval() {
    return lastInterval;
}

uint32_t HallSensor::getSpeed() {
    if (state == WheelSpeedState::STOP) {
        // The wheel isn't moving, so its speed is zero.
        return 0;
    }

    /*
     * The speed of the wheel in revolutions per minute.
     * lastInterval is the time that it takes for one full revolution of the wheel
     * in milliseconds, so converting that to RPM is just dividing 60000 by lastInterval.
     */
    const uint32_t rpm = 60000 / lastInterval;
    /*
     * The speed of the bike in miles per hour.
     * RPM * (circumference in inches (2 * pi * wheelRadius) / 1 rotation) * (1 mile / 63360 inches) * (60 minutes / hour)
     * is what calculates the speed in mph.
     */
    const uint32_t speed = static_cast<uint32_t>(rpm * ((wheelRadius * 2 * 3.1415926535) / numberOfMagnets) * 60) / 63360;

    return speed;
}

}// namespace WSS::DEV