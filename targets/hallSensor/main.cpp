#include <EVT/io/UART.hpp>
#include <EVT/manager.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/io/GPIO.hpp>
#include <dev/hallSensor.hpp>

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
   IO::GPIO& interruptGPIO2 = IO::getGPIO<EVT::core::IO::Pin::PA_0>(
       IO::GPIO::Direction::INPUT);

   uart.printf("GPIO Initialized\r\n");


   // Setup Hall Sensor
   // Wheel radius in inches
   constexpr uint32_t WHEEL_RADIUS = 15;
   constexpr uint32_t BACK_WHEEL_RADIUS = 15;

   uint32_t counter = 0;
   hallSensor::hallSensor hallSensor1(interruptGPIO, WHEEL_RADIUS);
   hallSensor::hallSensor hallSensor2(interruptGPIO2, BACK_WHEEL_RADIUS);

   // Main loop
   while (1) {
       //Check for GPIO READ TESTING
       if (interruptGPIO.readPin() == IO::GPIO::State::LOW) {
           uart.printf("GPIO PA_1 PIN LOW DETECTED\n\r");
           uart.printf("Counter: %d\n\r", counter);
           counter++;
       }

       if (interruptGPIO2.readPin() == IO::GPIO::State::LOW) {
           uart.printf("GPIO PA_0 PIN 2 LOW DETECTED\n\r");
           uart.printf("Counter: %d\n\r", counter);
           counter++;
       }


       uint32_t timeDiff = hallSensor1.update();
       uart.printf("Front Wheel speed (in/s): %d\n", hallSensor1.getSpeed(timeDiff));
       timeDiff = hallSensor2.update();
       uart.printf("Back Wheel speed (in/s): %d\n", hallSensor2.getSpeed(timeDiff));
       uart.printf("Time Diff: %d\n", timeDiff);
    }
}
