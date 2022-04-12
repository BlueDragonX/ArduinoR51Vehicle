#ifndef _NISSAN_R51_CLIMATE_H_
#define _NISSAN_R51_CLIMATE_H_

#include <Arduino.h>
#include <Canny.h>

namespace NissanR51 {

// Tracks temperatures reported by the climate control system.
// Handles CAN frame ID 0x54A.
struct ClimateTemperature {
    // Measurement units.
    enum Units : uint8_t {
        UNITS_METRIC,
        UNITS_US,
    };

    // The temperature units the system is using.
    enum Units units;
    // Requested driver temperature.
    uint8_t driver_temp;
    // Requested passenger temperature.
    uint8_t passenger_temp;
    // Current outside temperature.
    uint8_t outside_temp;

    // Construct an empty temperature struct.
    ClimateTemperature() : units(UNITS_METRIC), driver_temp(0),
            passenger_temp(0), outside_temp(0) {}

    // Handle a 0x54A climate temperature frame. Returns true if the climate
    // state changed as a result of handling the frame.
    bool handle(const Canny::Frame& frame);
};

// Tracks climate control state from CAN frames.
struct ClimateSystem {
    // Climate system operational state.
    enum System : uint8_t {
        SYSTEM_INIT,
        // System is off.
        SYSTEM_OFF,
        // System is in auto mode. The system is controlled automatically based
        // on selected zone temperatures, outside, and ambient air
        // temperatures.
        SYSTEM_AUTO,
        // System is being controlled manualy.
        SYSTEM_MANUAL,
        // System is in defrost mode. Vents and fan speed are optimized for
        // windshield and front window defrost.
        SYSTEM_DEFROST,
    };

    // Vent state.
    enum Vents : uint8_t {
        VENTS_CLOSED,
        VENTS_FACE,
        VENTS_FACE_FEET,
        VENTS_FEET,
        VENTS_FEET_WINDSHIELD,
        VENTS_WINDSHIELD,
    };

    // Current system state.
    enum System system;
    // Current vent state.
    enum Vents vents;
    // True if the A/C compressor is being requested.
    bool ac;
    // True if the system is in dual zone mode. Dual zone allows the driver and
    // passenger to select temperatures independently.
    bool dual;
    // True if the vents are set to recirculate.
    bool recirculate;
    // Current fan speed. Values are from 0 (off) to 7 (max).
    uint8_t fan_speed;

    ClimateSystem() : system(SYSTEM_OFF), vents(VENTS_CLOSED),
            ac(false), dual(false), recirculate(false), fan_speed(0) {}

    // Handle a frame. Returns true if the climate state changed as a
    // result of handling the frame.
    bool handle(const Canny::Frame& frame);
};

}  // namespace NissanR51

#endif  // _NISSAN_R51_CLIMATE_H_
