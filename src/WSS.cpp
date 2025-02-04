#include <EVT/utils/log.hpp>
#include <HALf3/stm32f3xx_hal.h>
#include <WSS.hpp>
#include <cstdlib>
#include <string>

namespace log = EVT::core::log;

namespace WSS {

WSS::WSS(DEV::HallSensor& hallSensor1, DEV::HallSensor& hallSensor2)
    : hallSensors{&hallSensor1, &hallSensor2}, debugPrintTime(HAL_GetTick()) {}

CO_OBJ_T* WSS::getObjectDictionary() {
    return objectDictionary;
}

void WSS::process() {
    for (uint8_t i = 0; i < NUM_HALLSENSORS; i++) {
        hallSensors[i]->update();
        wheelSpeeds[i] = hallSensors[i]->getSpeed();
    }

    // Prints out the wheel speed in miles per hour every 5 times a second
    if (HAL_GetTick() - debugPrintTime > 1000) {
        debugPrintTime = HAL_GetTick();
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Wheelspeed[0]:%d", wheelSpeeds[0]);
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Wheelspeed[1]:%d", wheelSpeeds[1]);
    }
}

uint8_t WSS::getNodeID() {
    return NODE_ID;
}

uint8_t WSS::getNumElements() {
    return OBJECT_DICTIONARY_SIZE;
}

}// namespace WSS
