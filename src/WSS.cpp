#include <WSS.hpp>

namespace WSS {

WSS::WSS(hallSensor::hallSensor& hallSensor1, hallSensor::hallSensor& hallSensor2) {
    this->hallSensors[0] = &hallSensor1;
    this->hallSensors[1] = &hallSensor2;
}

CO_OBJ_T* WSS::getObjectDictionary() {
    return objectDictionary;
}

void WSS::process() {
    for (uint8_t i = 0; i < NUM_HALLSENSORS; i++) {
        timeDifference[i] = hallSensors[i]->update();
        wheelSpeeds[i] = hallSensors[i]->getSpeed(timeDifference[i]);
    }
}

uint8_t WSS::getNodeID() {
    return NODE_ID;
}

uint8_t WSS::getNumElements() {
    return OBJECT_DICTIONARY_SIZE;
}

} // namespace WSS

