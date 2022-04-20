#ifndef _R51_VEHICLE_IPDM_H_
#define _R51_VEHICLE_IPDM_H_

#include <Arduino.h>
#include <Canny.h>
#include <Caster.h>
#include <Faker.h>
#include <R51Core.h>
#include "Handler.h"

namespace R51 {

// Tracks IPDM state stored in the 0x625 CAN frame.
class IPDM : public Caster::Node<Message> {
    public:
        IPDM(uint32_t tick_ms = 0, Faker::Clock* clock=Faker::Clock::real()) :
            changed_(false), event_(Event::BODY_POWER_STATE, {0x00}), ticker_(tick_ms, clock) {}

        // Handle a 0x625 IPDM state frame. Returns true if the state changed
        // as a result of handling the frame.
        void handle(const Message& msg) override;

        // Yield a BODY_POWER_STATE frame on change or tick.
        void emit(const Caster::Yield<Message>& yield) override;

    private:
        bool changed_;
        SystemEvent event_;
        Ticker ticker_;
};

}  // namespace R51

#endif  // _R51_VEHICLE_IPDM_H_
