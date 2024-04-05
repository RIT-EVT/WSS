#pragma once

#include <EVT/io/CANDevice.hpp>
#include <EVT/io/CANOpenMacros.hpp>
#include <EVT/io/SPI.hpp>
#include <co_core.h>
#include <dev/hallSensor.hpp>

#define NUM_HALLSENSORS 2

using namespace EVT::core::IO;

namespace WSS {

/**
 * This is an example of a class for a board
 */
class WSS : public CANDevice {
public:
    WSS(hallSensor::hallSensor& hallSensor1, hallSensor::hallSensor& hallSensor2);

    static constexpr uint16_t NODE_ID = 8;

    /**
     * Gets the object dictionary
     *
     * @return an object dictionary
     */
    CO_OBJ_T* getObjectDictionary();

    /**
     * Updates the temperature values in an array and updates the error array from the TMU object.
     */
    void process();

    /**
     * Get the device's node ID
     *
     * @return The node ID of the can device.
     */
    uint8_t getNodeID() override;

    /**
     * Get the number of elements in the object dictionary.
     *
     * @return The number of elements in the object dictionary
     */
    uint8_t getNumElements() override;

private:
    hallSensor::hallSensor* hallSensors[NUM_HALLSENSORS];

    uint32_t timeDifference[NUM_HALLSENSORS] = {0, 0};
    uint32_t wheelSpeeds[NUM_HALLSENSORS] = {0, 0};

    /**
     * Object Dictionary Size
     */
    static constexpr uint16_t OBJECT_DICTIONARY_SIZE = 43;

    /**
     * CAN Open object dictionary
     */
    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {
        MANDATORY_IDENTIFICATION_ENTRIES_1000_1014,
        HEARTBEAT_PRODUCER_1017(2000),
        IDENTITY_OBJECT_1018,
        SDO_CONFIGURATION_1200,

        // TPDO 0 CONFIGURATION
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x00, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 2000),

        // TPDO 0 MAPPING
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0, 2),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 1, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 2, PDO_MAPPING_UNSIGNED16),

        // TPDO 0 DATA LINKS
        DATA_LINK_START_KEY_21XX(0, 2),
        DATA_LINK_21XX(0x00, 1, CO_TUNSIGNED16, (uintptr_t) wheelSpeeds[0]),
        DATA_LINK_21XX(0x00, 2, CO_TUNSIGNED16, (uintptr_t) wheelSpeeds[1]),

        CO_OBJ_DICT_ENDMARK,
    };

};

}// namespace BOARD_NAME
