#include "IPDM.h"

#include <Arduino.h>
#include <Canny.h>
#include <Caster.h>
#include <R51Core.h>
#include "Macros.h"

namespace R51 {

void IPDM::handle(const Message& msg) {
    if (msg.type() != Message::CAN_FRAME ||
            msg.can_frame().id() != 0x625 ||
            msg.can_frame().size() < 6) {
        return;
    }
    uint8_t state = 0x00;
    // high beams
    setBit(&state, 0, 0, getBit(msg.can_frame().data(), 1, 4));
    // low beams
    setBit(&state, 0, 1, getBit(msg.can_frame().data(), 1, 5));
    // running lights
    setBit(&state, 0, 2, getBit(msg.can_frame().data(), 1, 6));
    // fog lights
    setBit(&state, 0, 3, getBit(msg.can_frame().data(), 1, 3));
    // defog heaters
    setBit(&state, 0, 6, getBit(msg.can_frame().data(), 0, 0));
    // a/c compressor
    setBit(&state, 0, 7, getBit(msg.can_frame().data(), 1, 7));


    if (state != event_.data[0]) {
        event_.data[0] = state;
        changed_ = true;
    }
}

void IPDM::emit(const Caster::Yield<Message>& yield) {
    if (changed_ || ticker_.active()) {
        ticker_.reset();
        yield(event_);
        changed_ = false;
    }
}

}  // namespace R51
