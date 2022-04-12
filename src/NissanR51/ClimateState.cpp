#include "ClimateState.h"

#include <Arduino.h>
#include <Canny.h>
#include "Binary.h"

#define SET_ON_CHANGE(property, value) ({\
    auto tmp = value;\
    if (property != tmp) {\
        property = tmp;\
        changed = true;\
    }\
})

namespace NissanR51 {
namespace {

enum Mode : uint8_t {
    MODE_OFF = 0x00,
    MODE_FACE = 0x04,
    MODE_FACE_FEET = 0x08,
    MODE_FEET = 0x0C,
    MODE_FEET_WINDSHIELD = 0x10,
    MODE_WINDSHIELD = 0x34,
    MODE_AUTO_FACE = 0x84,
    MODE_AUTO_FACE_FEET = 0x88,
    MODE_AUTO_FEET = 0x8C,
};

}  // namespace

bool ClimateTemperatureState::handle(const Canny::Frame& frame) {
    if (frame.size() != 8) {
        return false;
    }
    bool changed = false;
    SET_ON_CHANGE(units_, frame.data()[3] == 0x40 ?
            ClimateTemperatureState::UNITS_METRIC : ClimateTemperatureState::UNITS_US);
    SET_ON_CHANGE(driver_temp_, frame.data()[4]);
    SET_ON_CHANGE(passenger_temp_, frame.data()[5]);
    SET_ON_CHANGE(outside_temp_, frame.data()[7]);
    return changed;
}

bool ClimateSystemState::handle(const Canny::Frame& frame) {
    if (frame.size() != 8) {
        return false;
    }
    bool changed = false;
    SET_ON_CHANGE(ac_, getBit(frame.data(), 0, 3));
    SET_ON_CHANGE(dual_, getBit(frame.data(), 3, 7));
    SET_ON_CHANGE(recirculate_, getBit(frame.data(), 3, 4));
    SET_ON_CHANGE(fan_speed_, (frame.data()[2] + 1) / 2);

    switch((Mode)frame.data()[1]) {
        case MODE_OFF:
            SET_ON_CHANGE(vents_, ClimateSystemState::VENTS_CLOSED);
            break;
        case MODE_FACE:
        case MODE_AUTO_FACE:
            SET_ON_CHANGE(vents_, ClimateSystemState::VENTS_FACE);
            break;
        case MODE_FACE_FEET:
        case MODE_AUTO_FACE_FEET:
            SET_ON_CHANGE(vents_, ClimateSystemState::VENTS_FACE_FEET);
            break;
        case MODE_FEET:
        case MODE_AUTO_FEET:
            SET_ON_CHANGE(vents_, ClimateSystemState::VENTS_FEET);
            break;
        case MODE_FEET_WINDSHIELD:
            SET_ON_CHANGE(vents_, ClimateSystemState::VENTS_FEET_WINDSHIELD);
            break;
        case MODE_WINDSHIELD:
            SET_ON_CHANGE(vents_, ClimateSystemState::VENTS_WINDSHIELD);
            break;
    }

    if (vents_ == ClimateSystemState::VENTS_WINDSHIELD) {
        SET_ON_CHANGE(system_, ClimateSystemState::SYSTEM_DEFROST);
    } else if (getBit(frame.data(), 0, 7)) {
        SET_ON_CHANGE(system_, ClimateSystemState::SYSTEM_OFF);
    } else if (getBit(frame.data(), 0, 0)) {
        SET_ON_CHANGE(system_, ClimateSystemState::SYSTEM_AUTO);
    } else {
        SET_ON_CHANGE(system_, ClimateSystemState::SYSTEM_MANUAL);
    }

    return changed;
}

}  // namespace NissanR51
