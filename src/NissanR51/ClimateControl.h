#ifndef _NISSAN_R51_CLIMATE_CONTROL_H_
#define _NISSAN_R51_CLIMATE_CONTROL_H_

#include <Arduino.h>
#include <Canny.h>
#include "Controller.h"

namespace NissanR51 {

// Manages changes to the 0x540 CAN frame for controlling climate state. This
// works by toggling bits in the control frame. Independent changes can be made
// before sending the frame. However, changing the same setting twice without
// sending a frame is a noop for that setting.
//
// The control frame starts in an init state. It must sends the init frame a
// few times before becoming ready.
class ClimateSystemControl : public Controller {
    public:
        // Construct a climate system control object. If ready is true then the
        // control frame will immediately be placed into the ready state.
        ClimateSystemControl(bool ready=false);

        // Return true if the frame is available to be sent. Returns true if
        // called after construction. Cleared after a call to frame().
        bool available() override { return available_; }

        // Return a reference to the control frame.
        const Canny::Frame& frame() override;

        // Place the control frame into the ready state. This is a noop if
        // ready() was already called.
        void ready();

        // Turn off the climate control system.
        // This is a noop until ready() is called.
        void turnOff();

        // Toggle auto mode.
        // This is a noop until ready() is called.
        void toggleAuto();

        // Toggle A/C compressor.
        // This is a noop until ready() is called.
        void toggleAC();

        // Toggle dual zone mode.
        // This is a noop until ready() is called.
        void toggleDual();

        // Cycle modes.
        // This is a noop until ready() is called.
        void cycleMode();

        // Toggle defrost mode.
        // This is a noop until ready() is called.
        void toggleDefrost();

        // Increase driver zone temperature. Temperature changes should not be
        // sent while the climate control system is in the off state. 
        // This is a noop until ready() is called.
        void incDriverTemp();

        // Decrease driver zone temperature. Temperature changes should not be
        // sent while the climate control system is in the off state. 
        // This is a noop until ready() is called.
        void decDriverTemp();

        // Increase passenger zone temperature. Temperature changes should not
        // be sent while the climate control system is in the off state. 
        // This is a noop until ready() is called.
        void incPassengerTemp();

        // Decrease passenger zone temperature. Temperature changes should not
        // be sent while the climate control system is in the off state. 
        // This is a noop until ready() is called.
        void decPassengerTemp();
    private:
        Canny::Frame frame_;
        bool available_;
};

// Manages changes to the 0x541 CAN frame for controlling climate fan speed and
// recirculation. This works by toggling bits in the control frame. Independent
// changes can be made before sending the frame. However, changing the same
// setting twice without sending a frame is a noop for that setting.
//
// The control frame starts in an init state. It must sends the init frame a
// few times before becoming ready.
class ClimateFanControl : public Controller {
    public:
        // Construct a climate fan control object. If ready is true then the
        // control frame will immediately be placed into the ready state.
        ClimateFanControl(bool ready=false);

        // Return true if the frame is available to be sent. Returns true if
        // called after construction. Cleared after a call to frame().
        bool available() override { return available_; }

        // Return a reference to the control frame.
        const Canny::Frame& frame() override;

        // Place the control frame into the ready state. This is a noop if
        // ready() was already called.
        void ready();

        // Toggle cabin recirculation.
        // This is a noop until ready() is called.
        void toggleRecirculate();

        // Increase fan speed.
        // This is a noop until ready() is called.
        void incFanSpeed();

        // Decrease fan speed.
        // This is a noop until ready() is called.
        void decFanSpeed();
    private:
        Canny::Frame frame_;
        bool available_;
};

}  // namespace NissanR51

#endif  // _NISSAN_R51_CLIMATE_CONTROL_H_
