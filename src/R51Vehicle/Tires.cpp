#include "Tires.h"

#include <Arduino.h>
#include <Canny.h>
#include <R51Core.h>

namespace R51 {
namespace {

uint8_t getPressureValue(const Canny::Frame& frame, int tire) {
    if (getBit(frame.data(), 7, 7-tire)) {
        return frame.data()[2+tire];
    }
    return 0;
}

}  // namespace

void TirePressureState::handle(const Message& msg) {
    if (msg.type() != Message::CAN_FRAME ||
            msg.can_frame().id() != 0x385 ||
            msg.can_frame().size() != 8) {
        return;
    }

    for (int i = 0; i < 4; i++) {
        uint8_t value = getPressureValue(msg.can_frame(), i);
        if (event_.data[i] != value) {
            event_.data[i] = value;
            changed_ = true;
        }
    }
}

void TirePressureState::emit(const Caster::Yield<Message>& yield) {
    if (changed_ || ticker_.active()) {
        ticker_.reset();
        yield(event_);
        changed_ = false;
    }
}

}  // namespace R51
