#ifndef _NISSAN_R51_MACROS_H_
#define _NISSAN_R51_MACROS_H_

#define SET_ON_CHANGE(property, value) ({\
    auto tmp = value;\
    if (property != tmp) {\
        property = tmp;\
        changed = true;\
    }\
})

#endif  // _NISSAN_R51_MACROS_H_
