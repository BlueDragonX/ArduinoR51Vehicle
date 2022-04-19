#ifndef _R51_VEHICLE_UNITS_H_
#define _R51_VEHICLE_UNITS_H_

#include <Arduino.h>

namespace R51 {

// Measurement units.
enum Units : uint8_t {
    UNITS_METRIC,
    UNITS_US,
};

}  // namespace R51

#endif  // _R51_VEHICLE_UNITS_H_
