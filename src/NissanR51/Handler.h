#ifndef _NISSAN_R51_HANDLER_H_
#define _NISSAN_R51_HANDLER_H_

#include <Canny.h>

namespace NissanR51 {

// Handle CAN state update frames.
class Handler {
    public:
        Handler() = default;
        virtual ~Handler() = default;

        // Handle a state update CAN frame. Return true if the frame modified
        // the current state.
        virtual bool handle(const Canny::Frame& frame) = 0;
};

}  // namespace NissanR51

#endif  // _NISSAN_R51_HANDLER_H_
