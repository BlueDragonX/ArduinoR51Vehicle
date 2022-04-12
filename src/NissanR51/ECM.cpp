#include "ECM.h"

#include <Arduino.h>
#include <Canny.h>

namespace NissanR51 {

bool ECMCoolantState::handle(const Canny::Frame& frame) {
    if (frame.id() != 0x551 || frame.size() < 1) {
        return false;
    }
    int16_t value = (int16_t)frame.data()[0] - 40;
    if (value != coolant_temp_) {
        coolant_temp_ = value;
        return true;
    }
    return false;
}

bool ECMOilState::handle(const Canny::Frame& frame) {
    if (frame.id() != 0x580 || frame.size() < 5) {
        return false;
    }
    int16_t value = (int16_t)frame.data()[4] - 40;
    if (value != oil_temp_) {
        oil_temp_ = value;
        return true;
    }
    return false;
}

}  // namespace NissanR51
