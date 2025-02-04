#ifndef WSS_HPP
#define WSS_HPP

#include <EVT/io/CANDevice.hpp>
#include <EVT/io/CANOpenMacros.hpp>
#include <EVT/io/SPI.hpp>
#include <co_core.h>
#include <dev/HallSensor.hpp>

constexpr uint8_t NUM_HALLSENSORS = 2;

namespace IO = EVT::core::IO;

namespace WSS {

/**
 * This is the main class for the Wheel Speed Sensor
 */
class WSS : public CANDevice {
public:
    /**
    * Initialize WSS driver
    *
    * @param[in] hallSensor1 Front Hall sensor
    * @param[in] hallSensor2 Back Hall sensor
    */
    WSS(DEV::HallSensor& hallSensor1, DEV::HallSensor& hallSensor2);

    static constexpr uint16_t NODE_ID = 8;

    /** Updates the wheel speed values for all hallsensors in an array */
    void process();

    CO_OBJ_T* getObjectDictionary() override;

    uint8_t getNodeID() override;

    uint8_t getNumElements() override;

private:
    /** This is an array of the hall sensors for the front and back sensor */
    DEV::HallSensor* hallSensors[NUM_HALLSENSORS]{};

    /** This is an array of wheel speeds for the front and back wheel in miles per hour */
    uint16_t wheelSpeeds[NUM_HALLSENSORS] = {0, 0};

    /** Time in tick value that is used to print out wheel speeds five times a second */
    uint32_t debugPrintTime;

    /**
     * Object Dictionary Size
     */
    static constexpr uint16_t OBJECT_DICTIONARY_SIZE = 24;

    /**
     * CANopen object dictionary
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
        DATA_LINK_21XX(0x00, 1, CO_TUNSIGNED16, &wheelSpeeds[0]),
        DATA_LINK_21XX(0x00, 2, CO_TUNSIGNED16, &wheelSpeeds[1]),

        CO_OBJ_DICT_ENDMARK,
    };
};

}// namespace WSS

#endif