#ifndef _R51_VEHICLE_ECM_H_
#define _R51_VEHICLE_ECM_H_

#include <Arduino.h>
#include <Canny.h>
#include "Handler.h"

namespace R51 {

// Track reported coolant temperature from the ECM via the 0x551 CAN frame.
class ECMCoolantState : public Handler {
    public:
        ECMCoolantState() : coolant_temp_(0) {}

        // Handle ECM 0x551 state frames. Returns true if the state changed as
        // a result of handling the frame.
        bool handle(const Canny::Frame& frame) override;

        // Return the engine coolant temperature in Celsius.
        int16_t coolant_temp() const { return coolant_temp_; }
    private:
        int16_t coolant_temp_;
};

}  // namespace R51

#endif  // _R51_VEHICLE_ECM_H_
