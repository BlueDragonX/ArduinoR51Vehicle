#include "ECM.h"

#include <Arduino.h>
#include <Canny.h>

namespace R51 {

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

}  // namespace R51
