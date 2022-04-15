#ifndef _NISSAN_R51_CLIMATE_STATE_H_
#define _NISSAN_R51_CLIMATE_STATE_H_

#include <Arduino.h>
#include <Canny.h>
#include "Handler.h"
#include "Units.h"

namespace NissanR51 {

// Climate system operational state.
enum ClimateSystem : uint8_t {
    // System is off.
    CLIMATE_SYSTEM_OFF,
    // System is in auto mode. The system is controlled automatically based
    // on selected zone temperatures, outside, and ambient air
    // temperatures.
    CLIMATE_SYSTEM_AUTO,
    // System is being controlled manualy.
    CLIMATE_SYSTEM_MANUAL,
    // System is in defrost mode. Vents and fan speed are optimized for
    // windshield and front window defrost.
    CLIMATE_SYSTEM_DEFROST,
};

// Climate system vent state.
enum ClimateVents : uint8_t {
    CLIMATE_VENTS_CLOSED,
    CLIMATE_VENTS_FACE,
    CLIMATE_VENTS_FACE_FEET,
    CLIMATE_VENTS_FEET,
    CLIMATE_VENTS_FEET_WINDSHIELD,
    CLIMATE_VENTS_WINDSHIELD,
};

// Tracks temperatures reported by the climate control system.
// Handles CAN frame ID 0x54A.
class ClimateTemperatureState : public Handler {
    public:
        // Construct an empty temperature struct.
        ClimateTemperatureState() : units_(UNITS_METRIC), driver_temp_(0),
                passenger_temp_(0), outside_temp_(0) {}

        // Handle a 0x54A climate temperature frame. Returns true if the state
        // changed as a result of handling the frame.
        bool handle(const Canny::Frame& frame) override;

        // The temperature units the system is using.
        enum Units units() const { return units_; }

        // Requested driver temperature.
        uint8_t driver_temp() const { return driver_temp_; }

        // Requested passenger temperature.
        uint8_t passenger_temp() const { return passenger_temp_; }

        // Current outside temperature.
        uint8_t outside_temp() const { return outside_temp_; }

    private:
        enum Units units_;
        uint8_t driver_temp_;
        uint8_t passenger_temp_;
        uint8_t outside_temp_;
};

// Tracks the system state reported by the climate control system.
// Handles CAN frame ID 0x54B.
class ClimateSystemState : public Handler {
    public:
        ClimateSystemState() : system_(CLIMATE_SYSTEM_OFF), vents_(CLIMATE_VENTS_CLOSED),
                ac_(false), dual_(false), recirculate_(false), fan_speed_(0) {}

        // Handle a 0x54B climate system frame. Returns true if the state changed
        // as a result of handling the frame.
        bool handle(const Canny::Frame& frame);

        // Current system state.
        enum ClimateSystem system() const { return system_; }

        // Current vent state.
        enum ClimateVents vents() const { return vents_; }

        // True if the A/C compressor is being requested.
        bool ac() const { return ac_; }

        // True if the system is in dual zone mode. Dual zone allows the driver and
        // passenger to select temperatures independently.
        bool dual() const { return dual_; }

        // True if the vents are set to recirculate.
        bool recirculate() const { return recirculate_; }

        // Current fan speed. Values are from 0 (off) to 7 (max).
        uint8_t fan_speed() const { return fan_speed_; }
    private:
        enum ClimateSystem system_;
        enum ClimateVents vents_;
        bool ac_;
        bool dual_;
        bool recirculate_;
        uint8_t fan_speed_;
};

}  // namespace NissanR51

#endif  // _NISSAN_R51_CLIMATE_STATE_H_
