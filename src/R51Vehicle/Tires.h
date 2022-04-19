#ifndef _R51_VEHICLE_TIRES_H
#define _R51_VEHICLE_TIRES_H

#include <Arduino.h>
#include <Canny.h>
#include "Handler.h"

namespace R51 {

// Track tire pressure as reported in the 0x385 CAN frame.
class TirePressureState : public Handler {
    public:
        TirePressureState() : tire_pressure_1_(0), tire_pressure_2_(0),
                tire_pressure_3_(0), tire_pressure_4_(0) {}

        // Handle 0x385 tire pressure state frames. Returns true if the state
        // changed as a result of handling the frame.
        bool handle(const Canny::Frame& frame) override;

        // Return the pressure of tire 1 in PSI. Return -1 if unreported.
        double tire_pressure_1() { return tire_pressure_1_; }

        // Return the pressure of tire 2 in PSI. Return -1 if unreported.
        double tire_pressure_2() { return tire_pressure_2_; }

        // Return the pressure of tire 3 in PSI. Return -1 if unreported.
        double tire_pressure_3() { return tire_pressure_3_; }

        // Return the pressure of tire 4 in PSI. Return -1 if unreported.
        double tire_pressure_4() { return tire_pressure_4_; }
    private:
        double tire_pressure_1_;
        double tire_pressure_2_;
        double tire_pressure_3_;
        double tire_pressure_4_;
};

}

#endif  // _R51_VEHICLE_TIRES_H
