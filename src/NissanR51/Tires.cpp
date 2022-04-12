#include "Tires.h"

#include <Arduino.h>
#include <Canny.h>
#include "Binary.h"
#include "Macros.h"

namespace NissanR51 {
namespace {

double getPressure(const Canny::Frame& frame, int tire) {
    if (getBit(frame.data(), 7, 7-tire)) {
        return frame.data()[2+tire]/4.0;
    }
    return -1;
}

}  // namespace

bool TirePressureState::handle(const Canny::Frame& frame) {
    if (frame.id() != 0x385 || frame.size() != 8) {
        return false;
    }
    bool changed = false;
    SET_ON_CHANGE(tire_pressure_1_, getPressure(frame, 0));
    SET_ON_CHANGE(tire_pressure_2_, getPressure(frame, 1));
    SET_ON_CHANGE(tire_pressure_3_, getPressure(frame, 2));
    SET_ON_CHANGE(tire_pressure_4_, getPressure(frame, 3));
    return changed;
}

}  // namespace NissanR51
