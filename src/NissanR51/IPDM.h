#ifndef _NISSAN_R51_IPDM_H_
#define _NISSAN_R51_IPDM_H_

#include <Arduino.h>
#include <Canny.h>
#include "Handler.h"

namespace NissanR51 {

// Tracks IPDM state stored in the 0x625 CAN frame.
class IPDMState : public Handler {
    public:
        IPDMState() : defog_(false), high_beams_(false), low_beams_(false),
            running_lights_(false), ac_compressor_(false) {}

        // Handle a 0x625 IPDM state frame. Returns true if the state changed
        // as a result of handling the frame.
        bool handle(const Canny::Frame& frame) override;

        // Return true if the defog heaters are enabled.
        bool defog() const { return defog_; }

        // Return true if the headlamp high beams are on.
        bool high_beams() const { return high_beams_; }

        // Return true if the headlamp low beams are on.
        bool low_beams() const { return low_beams_; }

        // Return true if the running lights are on.
        bool running_lights() const { return running_lights_; }

        // Return true if the A/C compressor clutch is active.
        bool ac_compressor() const { return ac_compressor_; }

    private:
        bool defog_;
        bool high_beams_;
        bool low_beams_;
        bool running_lights_;
        bool ac_compressor_;
};

}  // namespace NissanR51

#endif  // _NISSAN_R51_IPDM_H_
