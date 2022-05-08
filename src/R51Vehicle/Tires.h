#ifndef _R51_VEHICLE_TIRES_H
#define _R51_VEHICLE_TIRES_H

#include <Arduino.h>
#include <Caster.h>
#include <R51Core.h>

namespace R51 {

// Track tire pressure as reported in the 0x385 CAN frame.
class TirePressureState : public Caster::Node<Message> {
    public:
        TirePressureState(uint32_t tick_ms = 0, Faker::Clock* clock = Faker::Clock::real()) :
            changed_(false), event_(Event::TIRE_PRESSURE_STATE,
            {0x00, 0x00, 0x00, 0x00}), ticker_(tick_ms, clock) {}

        // Handle 0x385 tire pressure state frames. Returns true if the state
        // changed as a result of handling the frame.
        void handle(const Message& msg) override;

        // Yield a TIRE_PRESSURE_STATE frame on change or tick.
        void emit(const Caster::Yield<Message>& yield) override;

    private:
        bool changed_;
        SystemEvent event_;
        Ticker ticker_;
};

}

#endif  // _R51_VEHICLE_TIRES_H
