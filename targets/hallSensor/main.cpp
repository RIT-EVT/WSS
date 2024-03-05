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
    IO::GPIO& gpio = IO::getGPIO<IO::Pin::PA_1>();

    // Setup Hall Sensor
    constexpr uint32_t WHEEL_RADIUS = 10;
    DEV::RTC& clock = DEV::getRTC();
    hallSensor::HallSensor hallSensor(gpio, WHEEL_RADIUS, clock);

    // Main loop
    while (1) {
        uart.printf("GPIO State: %d\r\n", gpio.readPin());
        uint32_t timeDiff = hallSensor.update();
        uart.printf("Wheel speed: %d\r\n", hallSensor.getSpeed(timeDiff));
        uart.printf("Wheel State: %d\r\n", hallSensor.getState());
        uart.printf("Time Diff: %d\r\n", timeDiff);
        time::wait(1000);
    }
}
