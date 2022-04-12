#ifndef _NISSAN_R51_ECM_H_
#define _NISSAN_R51_ECM_H_

#include <Arduino.h>
#include <Canny.h>
#include "Handler.h"

namespace NissanR51 {

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

// Track reported oil temperatrure from the ECM via the 0x580 CAN frame.
class ECMOilState : public Handler {
    public:
        ECMOilState() : oil_temp_(0) {}

        // Handle ECM 0x551 state frames. Returns true if the state changed as
        // a result of handling the frame.
        bool handle(const Canny::Frame& frame) override;

        // Return the engine oil temperature in Celsius.
        int16_t oil_temp() const { return oil_temp_; }
    private:
        int16_t oil_temp_;
};

}  // namespace NissanR51

#endif  // _NISSAN_R51_ECM_H_
