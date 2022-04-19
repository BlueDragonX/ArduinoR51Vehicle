#ifndef _R51_VEHICLE_MACROS_H_
#define _R51_VEHICLE_MACROS_H_

#define SET_ON_CHANGE(property, value) ({\
    auto tmp = value;\
    if (property != tmp) {\
        property = tmp;\
        changed = true;\
    }\
})

#endif  // _R51_VEHICLE_MACROS_H_
