#include <EVT/io/UART.hpp>
#include <EVT/manager.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/io/GPIO.hpp>
#include <DEV/hallSensor.hpp>

namespace IO = EVT::core::IO;

constexpr IO::Pin INTERRUPT_PIN = IO::Pin::PA_1;

int main() {
   // Initialize system
   EVT::core::platform::init();

   // Setup UART
   IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
   uart.printf("UART Initialized\r\n");


   // Setup GPIO
   IO::GPIO& interruptGPIO = IO::getGPIO<INTERRUPT_PIN>(
       IO::GPIO::Direction::INPUT);
   uart.printf("GPIO Initialized\r\n");

   // Setup the RTC
   DEV::RTC& rtc = DEV::getRTC();
   time::TimeStamp time;
   rtc.setTime(time);

   // Setup Hall Sensor
   // Wheel radius in inches
   constexpr uint32_t WHEEL_RADIUS = 15;
   hallSensor::HallSensor hallSensor(interruptGPIO, WHEEL_RADIUS, rtc);

   // Main loop
   while (1) {
       uint32_t timeDiff = hallSensor.update();
       uart.printf("Wheel speed (in/s): %d\n", hallSensor.getSpeed(timeDiff));
       uart.printf("Time Diff: %d\n", timeDiff);
    }
}
