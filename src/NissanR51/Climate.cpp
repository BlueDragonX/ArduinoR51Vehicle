#include "Climate.h"

#include <Arduino.h>
#include <Canny.h>
#include "Binary.h"

#define SET_ON_CHANGE(property, value) ({\
    auto tmp = value;\
    if (state->property != tmp) {\
        state->property = tmp;\
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

bool handle54A(ClimateState* state, const Canny::Frame& frame) {
    bool changed = false;
    SET_ON_CHANGE(units, frame.data()[3] == 0x40 ?
            ClimateState::UNITS_METRIC : ClimateState::UNITS_US);
    SET_ON_CHANGE(driver_temp, frame.data()[4]);
    SET_ON_CHANGE(passenger_temp, frame.data()[5]);
    SET_ON_CHANGE(outside_temp, frame.data()[7]);
    return changed;
}

bool handle54B(ClimateState* state, const Canny::Frame& frame) {
    bool changed = false;
    SET_ON_CHANGE(ac, getBit(frame.data(), 0, 3));
    SET_ON_CHANGE(dual, getBit(frame.data(), 3, 7));
    SET_ON_CHANGE(recirculate, getBit(frame.data(), 3, 4));
    SET_ON_CHANGE(fan_speed, (frame.data()[2] + 1) / 2);

    switch((Mode)frame.data()[1]) {
        case MODE_OFF:
            SET_ON_CHANGE(vents, ClimateState::VENTS_CLOSED);
            break;
        case MODE_FACE:
        case MODE_AUTO_FACE:
            SET_ON_CHANGE(vents, ClimateState::VENTS_FACE);
            break;
        case MODE_FACE_FEET:
        case MODE_AUTO_FACE_FEET:
            SET_ON_CHANGE(vents, ClimateState::VENTS_FACE_FEET);
            break;
        case MODE_FEET:
        case MODE_AUTO_FEET:
            SET_ON_CHANGE(vents, ClimateState::VENTS_FEET);
            break;
        case MODE_FEET_WINDSHIELD:
            SET_ON_CHANGE(vents, ClimateState::VENTS_FEET_WINDSHIELD);
            break;
        case MODE_WINDSHIELD:
            SET_ON_CHANGE(vents, ClimateState::VENTS_WINDSHIELD);
            break;
    }

    if (state->vents == ClimateState::VENTS_WINDSHIELD) {
        SET_ON_CHANGE(system, ClimateState::SYSTEM_DEFROST);
    } else if (getBit(frame.data(), 0, 7)) {
        SET_ON_CHANGE(system, ClimateState::SYSTEM_OFF);
    } else if (getBit(frame.data(), 0, 0)) {
        SET_ON_CHANGE(system, ClimateState::SYSTEM_AUTO);
    } else {
        SET_ON_CHANGE(system, ClimateState::SYSTEM_MANUAL);
    }

    return changed;
}

}  // namespace

bool ClimateState::handle(const Canny::Frame& frame) {
    if (frame.size() != 8) {
        return false;
    }
    switch (frame.id()) {
        case 0x54A:
            return handle54A(this, frame);
        case  0x54B:
            return handle54B(this, frame);
        default:
            return false;
    }
}

}  // namespace NissanR51
