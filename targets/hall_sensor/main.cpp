/**
 * This is a test program for the hall sensor.
 */

#include <EVT/io/GPIO.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>
#include <dev/HallSensor.hpp>

namespace IO = EVT::core::IO;

constexpr IO::Pin INTERRUPT_PIN = IO::Pin::PA_1;

int main() {
    // Initialize system
    EVT::core::platform::init();

    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    uart.printf("UART Initialized\r\n");

    // Setup GPIO
    IO::GPIO& interruptGPIO = IO::getGPIO<IO::Pin::PA_1>(
        IO::GPIO::Direction::INPUT);
    IO::GPIO& interruptGPIO2 = IO::getGPIO<IO::Pin::PA_0>(
        IO::GPIO::Direction::INPUT);

    uart.printf("GPIO Initialized\r\n");

    // Setup Hall Sensor
    // Wheel radius in inches
    constexpr uint32_t WHEEL_RADIUS = 15;
    constexpr uint32_t NUM_OF_MAGNETS = 1;

    uint32_t counter = 0;
    WSS::DEV::HallSensor hallSensor(interruptGPIO, WHEEL_RADIUS, NUM_OF_MAGNETS);

    uint16_t i = 0;

    // Main loop
    while (1) {
        //Check for GPIO READ TESTING
        if (interruptGPIO.readPin() == IO::GPIO::State::LOW) {
            uart.printf("GPIO PA_1 PIN LOW DETECTED\r\n");
            uart.printf("Counter: %d\r\n", counter);
            counter++;
        }

        if (interruptGPIO2.readPin() == IO::GPIO::State::LOW) {
            uart.printf("GPIO PA_0 PIN 2 LOW DETECTED\r\n");
            uart.printf("Counter: %d\r\n", counter);
            counter++;
        }

        hallSensor.update();

        if (i++ == 0) {
            uart.printf("Wheel speed (in/s): %d\r\n", hallSensor.getRawInterval());
        }
    }
}
