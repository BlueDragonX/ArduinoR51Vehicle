#include "IPDM.h"

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

bool IPDMState::handle(const Canny::Frame& frame) {
    if (frame.id() != 0x625 || frame.size() < 6) {
        return false;
    }
    bool changed = false;
    SET_ON_CHANGE(defog_, getBit(frame.data(), 0, 0));
    SET_ON_CHANGE(high_beams_, getBit(frame.data(), 1, 4));
    SET_ON_CHANGE(low_beams_, getBit(frame.data(), 1, 5));
    SET_ON_CHANGE(running_lights_, getBit(frame.data(), 1, 6));
    SET_ON_CHANGE(ac_compressor_, getBit(frame.data(), 1, 7));
    return changed;
}

}  // namespace NissanR51