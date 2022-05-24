#ifndef _R51_VEHICLE_CLIMATE_EVENTS_H_
#define _R51_VEHICLE_CLIMATE_EVENTS_H_

#include <Arduino.h>
#include <R51Core.h>
#include "Units.h"

namespace R51 {

enum ClimateSystemMode : uint8_t {
    CLIMATE_SYSTEM_OFF = 0,
    CLIMATE_SYSTEM_AUTO = 1,
    CLIMATE_SYSTEM_MANUAL = 2,
    CLIMATE_SYSTEM_DEFROST = 3,
};

// Climate temperature state event.
class ClimateTempStateEvent : public SystemEvent {
    public:
        ClimateTempStateEvent() : SystemEvent(Event::CLIMATE_TEMP_STATE, {0x00, 0x00, 0x00, 0x00}) {}

        SYSTEM_EVENT_PROPERTY(uint8_t, driver_temp, data[0], data[0] = value)
        SYSTEM_EVENT_PROPERTY(uint8_t, passenger_temp, data[1], data[1] = value)
        SYSTEM_EVENT_PROPERTY(uint8_t, outside_temp, data[2], data[2] = value)
        SYSTEM_EVENT_PROPERTY(Units, units, (Units)data[3], data[3] = (uint8_t)value)
};

// Climate airflow state event.
class ClimateAirflowStateEvent : public SystemEvent {
    public:
        ClimateAirflowStateEvent() : SystemEvent(Event::CLIMATE_AIRFLOW_STATE, {0x00, 0x00}) {}

        SYSTEM_EVENT_PROPERTY(uint8_t, fan_speed, data[0], data[0] = value)
        SYSTEM_EVENT_PROPERTY(bool, face,
                getBit(data, 1, 0),
                setBit(data, 1, 0, value))
        SYSTEM_EVENT_PROPERTY(bool, feet,
                getBit(data, 1, 1),
                setBit(data, 1, 1, value))
        SYSTEM_EVENT_PROPERTY(bool, windshield,
                getBit(data, 1, 2),
                setBit(data, 1, 2, value))
        SYSTEM_EVENT_PROPERTY(bool, recirculate,
                getBit(data, 1, 3),
                setBit(data, 1, 3, value))
};

// Climate system state event.
class ClimateSystemStateEvent : public SystemEvent {
    public:
        ClimateSystemStateEvent() : SystemEvent(Event::CLIMATE_SYSTEM_STATE, {0x00}) {}

        SYSTEM_EVENT_PROPERTY(ClimateSystemMode, mode,
                (ClimateSystemMode)(data[0] & 0x03),
                data[0] = ((data[0] & 0xFC) | (uint8_t)value))
        SYSTEM_EVENT_PROPERTY(bool, ac,
                getBit(data, 0, 2),
                setBit(data, 0, 2, value))
        SYSTEM_EVENT_PROPERTY(bool, dual,
                getBit(data, 0, 3),
                setBit(data, 0, 3, value))
};

}  // namespace R51

#endif  // _R51_VEHICLE_CLIMATE_EVENTS_H_
