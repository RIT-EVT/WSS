#include <EVT/io/GPIO.hpp>
#include <EVT/utils/log.hpp>
#include <HALf3/stm32f3xx_hal.h>
#include <dev/HallSensor.hpp>

namespace IO = EVT::core::IO;

namespace WSS::DEV {

HallSensor::HallSensor(IO::GPIO& gpio, uint32_t wheelRadius, uint32_t numberOfMagnets) : gpio((IO::GPIO&) gpio), wheelRadius(wheelRadius), numberOfMagnets(numberOfMagnets) {
    this->prevTime = 0;
    this->lastInterval = 0;
    this->state = WheelSpeedState::STOP;
    this->magnetInLastRead = false;
}

void HallSensor::update() {
    uint32_t now = HAL_GetTick();
    uint32_t elapsedTime = now - prevTime;

    // The magnet is detected if the previous gpio pin reading is the same as the current gpio
    // pin reading, and if the pin is LOW
    const bool magnetReadLow = gpio.readPin() == MAGNET_DETECTED_STATE;
    const bool magnetDetected = magnetReadLow && !magnetInLastRead;
    magnetInLastRead = magnetReadLow;

    switch (state) {
    case WheelSpeedState::STOP:
        if (magnetDetected) {
            prevTime = now;
            state = WheelSpeedState::INITIALIZING;
        }
        break;

    case WheelSpeedState::INITIALIZING:
        if (magnetDetected) {
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
            lastInterval = 0;
        }
        break;

    case WheelSpeedState::MAINTAIN:
        if (magnetDetected) {
            // If the magnet is newly detected, record the time and interval
            lastInterval = elapsedTime;
            prevTime = now;

            // If the magnet hasn't been detected for a significant amount of time, go back to the stop
            // state
        } else if (elapsedTime > THRESHOLD) {
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
    if (state != WheelSpeedState::MAINTAIN) {
        // The wheel isn't moving, so its speed is zero.
        return 0;
    }

    /*
     * The speed of the wheel in revolutions per minute.
     * lastInterval is the time that it takes for one magnet detection to the next
     * in milliseconds, so converting that to RPM is 60000 by lastInterval and then
     * dividing again by numberOfMagnets.
     */
    const uint32_t rpm = 60000 / lastInterval / numberOfMagnets;
    /*
     * The speed of the bike in miles per hour.
     * RPM * (circumference in inches (2 * pi * wheelRadius) / 1 rotation) * (1 mile / 63360 inches) * (60 minutes / hour)
     * is what calculates the speed in mph.
     */
    const uint32_t speed = rpm * wheelRadius * 2 * 3142 * 60 / 63360000;

    return speed;
}

}// namespace WSS::DEV