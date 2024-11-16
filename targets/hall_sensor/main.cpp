/**
 * This is a test program for the Hall Effect sensor.
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

    WSS::DEV::HallSensor hallSensor(interruptGPIO, WHEEL_RADIUS, NUM_OF_MAGNETS);

    uint32_t debugPrintTime = HAL_GetTick();

    // Main loop
    while (1) {
        hallSensor.update();

        // Prints out the wheel speed in miles per hour every 5 times a second
        if (HAL_GetTick() - debugPrintTime > 200) {
            debugPrintTime = HAL_GetTick();
            uart.printf("Wheel speed (miles/hour): %d\r\n", hallSensor.getSpeed());
        }
    }
}
