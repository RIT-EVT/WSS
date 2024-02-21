#pragma once

#include "EVT/dev/RTCTimer.hpp"
#include <EVT/dev/RTCTimer.hpp>
#include <EVT/io/GPIO.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/time.hpp>

using namespace EVT::core::IO;
using namespace EVT::core::DEV;

namespace hallSensor {

    class hallSensor {
            // Define the states of the wheel speed
            enum WheelSpeedState {
                STOP = 0,    // First pulse or wheel is dead
                MAINTAIN = 1,// Wheel is spinning at a constant speed or speeding up
                DECAY = 2    // Wheel is slowing down
            };

        public:
            // Constructor (take a GPIO instance and the radius of the wheel)
            hallSensor(GPIO* gpio, uint32_t wheelRadius);

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
            DEV::RTCTimer timer;   // RTC Timer for time based operations

            // Calculate the specified time based on the rotation period
            uint32_t calculateSpecifiedTime();

            // Calculate the speed based on the pulse count and time elapsed
            uint32_t calculateSpeed();

            // Check if a full rotation is completed
            bool hasCompletedRotation();
        };
    }
} // namespace hallSensor