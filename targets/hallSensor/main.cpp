#include <EVT/io/UART.hpp>
#include <EVT/manager.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/io/GPIO.hpp>
#include <DEV/hallSensor.hpp>

namespace IO = EVT::core::IO;

int main() {
   // Initialize system
   EVT::core::platform::init();

   // Setup UART
   IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    // Setup GPIO
    IO::GPIO& gpio = IO::getGPIO<IO::Pin::PA_0>();

    // Setup Hall Sensor
    constexpr uint32_t WHEEL_RADIUS = 10;
    hallSensor::HallSensor hallSensor(gpio, WHEEL_RADIUS);

    // Main loop
    while (1) {
        uint32_t timeDiff = hallSensor.update();
        uart.printf("Wheel speed: %d\n", hallSensor.getSpeed(timeDiff));
        uart.printf("Time Diff: %d\n", timeDiff);
    }
}
