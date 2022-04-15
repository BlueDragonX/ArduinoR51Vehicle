#ifndef _NISSAN_R51_UNITS_H_
#define _NISSAN_R51_UNITS_H_

#include <Arduino.h>

namespace NissanR51 {

// Measurement units.
enum Units : uint8_t {
    UNITS_METRIC,
    UNITS_US,
};

}  // namespace NissanR51

#endif  // _NISSAN_R51_UNITS_H_
