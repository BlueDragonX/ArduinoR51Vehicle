#include "ClimateState.h"

#include <Arduino.h>
#include <Canny.h>
#include "Binary.h"
#include "Macros.h"

namespace NissanR51 {
namespace {

enum Mode : uint8_t {
    CLIMATE_MODE_OFF = 0x00,
    CLIMATE_MODE_FACE = 0x04,
    CLIMATE_MODE_FACE_FEET = 0x08,
    CLIMATE_MODE_FEET = 0x0C,
    CLIMATE_MODE_FEET_WINDSHIELD = 0x10,
    CLIMATE_MODE_WINDSHIELD = 0x34,
    CLIMATE_MODE_AUTO_FACE = 0x84,
    CLIMATE_MODE_AUTO_FACE_FEET = 0x88,
    CLIMATE_MODE_AUTO_FEET = 0x8C,
};

}  // namespace

bool ClimateTemperatureState::handle(const Canny::Frame& frame) {
    if (frame.id() != 0x54A || frame.size() != 8) {
        return false;
    }
    bool changed = false;
    SET_ON_CHANGE(units_, frame.data()[3] == 0x40 ? UNITS_METRIC : UNITS_US);
    SET_ON_CHANGE(driver_temp_, frame.data()[4]);
    SET_ON_CHANGE(passenger_temp_, frame.data()[5]);
    SET_ON_CHANGE(outside_temp_, frame.data()[7]);
    return changed;
}

bool ClimateSystemState::handle(const Canny::Frame& frame) {
    if (frame.id() != 0x54B || frame.size() != 8) {
        return false;
    }
    bool changed = false;
    SET_ON_CHANGE(ac_, getBit(frame.data(), 0, 3));
    SET_ON_CHANGE(dual_, getBit(frame.data(), 3, 7));
    SET_ON_CHANGE(recirculate_, getBit(frame.data(), 3, 4));
    SET_ON_CHANGE(fan_speed_, (frame.data()[2] + 1) / 2);

    switch((Mode)frame.data()[1]) {
        case CLIMATE_MODE_OFF:
            SET_ON_CHANGE(vents_, CLIMATE_VENTS_CLOSED);
            break;
        case CLIMATE_MODE_FACE:
        case CLIMATE_MODE_AUTO_FACE:
            SET_ON_CHANGE(vents_, CLIMATE_VENTS_FACE);
            break;
        case CLIMATE_MODE_FACE_FEET:
        case CLIMATE_MODE_AUTO_FACE_FEET:
            SET_ON_CHANGE(vents_, CLIMATE_VENTS_FACE_FEET);
            break;
        case CLIMATE_MODE_FEET:
        case CLIMATE_MODE_AUTO_FEET:
            SET_ON_CHANGE(vents_, CLIMATE_VENTS_FEET);
            break;
        case CLIMATE_MODE_FEET_WINDSHIELD:
            SET_ON_CHANGE(vents_, CLIMATE_VENTS_FEET_WINDSHIELD);
            break;
        case CLIMATE_MODE_WINDSHIELD:
            SET_ON_CHANGE(vents_, CLIMATE_VENTS_WINDSHIELD);
            break;
    }

    if (vents_ == CLIMATE_VENTS_WINDSHIELD) {
        SET_ON_CHANGE(system_, CLIMATE_SYSTEM_DEFROST);
    } else if (getBit(frame.data(), 0, 7)) {
        SET_ON_CHANGE(system_, CLIMATE_SYSTEM_OFF);
    } else if (getBit(frame.data(), 0, 0)) {
        SET_ON_CHANGE(system_, CLIMATE_SYSTEM_AUTO);
    } else {
        SET_ON_CHANGE(system_, CLIMATE_SYSTEM_MANUAL);
    }

    return changed;
}

}  // namespace NissanR51
