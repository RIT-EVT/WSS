#include <EVT/io/GPIO.hpp>
#include <HALf3/stm32f3xx_hal.h>
#include <dev/HallSensor.hpp>

namespace IO = EVT::core::IO;

// TODO Tune these values
constexpr uint32_t THRESHOLD = 10000;// Threshold for wheel speed

namespace WSS::DEV {

HallSensor::HallSensor(IO::GPIO& gpio, uint32_t wheelRadius) : gpio((IO::GPIO&) gpio), wheelRadius(wheelRadius) {
    this->prevTime = 0;
    this->lastInterval = 0;
    this->state = WheelSpeedState::STOP;
    this->magnetInLastRead = false;
}

void HallSensor::update() {
    uint32_t now = HAL_GetTick();
    uint32_t elapsedTime = now - prevTime;

    switch (state) {
    case WheelSpeedState::STOP:
        if (gpio.readPin() == MAGNET_DETECTED_STATE) {
            // If the magnet is detected for the first time, record the time and start initializing
            if (!magnetInLastRead) {
                prevTime = now;
                magnetInLastRead = true;
                state = WheelSpeedState::INITIALIZING;
            }
        } else {
            magnetInLastRead = false;
        }
        break;

    case WheelSpeedState::INITIALIZING:
        if (gpio.readPin() == MAGNET_DETECTED_STATE) {
            if (!magnetInLastRead) {
                // If the magnet is detected for the second time, record the time and the first
                // interval and start maintaining
                lastInterval = elapsedTime;
                prevTime = now;
                magnetInLastRead = true;
                state = WheelSpeedState::MAINTAIN;
            }
        } else {
            magnetInLastRead = false;
        }

        // If the magnet hasn't been detected for a significant amount of time, go back to the stop
        // state
        if (elapsedTime > THRESHOLD) {
            state = WheelSpeedState::STOP;
            prevTime = 0;
        }
        break;

    case WheelSpeedState::MAINTAIN:
        if (gpio.readPin() == MAGNET_DETECTED_STATE) {
            if (!magnetInLastRead) {
                // If the magnet is newly detected, record the time and interval
                lastInterval = elapsedTime;
                prevTime = now;
                magnetInLastRead = true;
            }
        } else {
            magnetInLastRead = false;
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
    const uint32_t speed = static_cast<uint32_t>(rpm * wheelRadius * 2 * 3.1415926535 * 60) / 63360;
    return speed;
}

}// namespace WSS::DEV