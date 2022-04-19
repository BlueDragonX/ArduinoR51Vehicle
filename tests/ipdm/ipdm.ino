#include <Arduino.h>
#include <AUnit.h>
#include <R51Vehicle.h>

namespace R51 {

using namespace aunit;
using ::Canny::Frame;

test(IPDMStateTest, IgnoreIncorrectID) {
    Frame f(0x624, 0, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    IPDMState state;
    assertFalse(state.handle(f));
}

test(IPDMStateTest, IgnoreIncorrectSize) {
    Frame f(0x625, 0, {0x00, 0x00, 0x00});
    IPDMState state;
    assertFalse(state.handle(f));
}

test(IPDMStateTest, Defog) {
    Frame f(0x625, 0, {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDMState state;
    assertTrue(state.handle(f));
    assertEqual(state.defog(), true);
    assertEqual(state.high_beams(), false);
    assertEqual(state.low_beams(), false);
    assertEqual(state.fog_lights(), false);
    assertEqual(state.running_lights(), false);
    assertEqual(state.ac_compressor(), false);
    assertFalse(state.handle(f));
}

test(IPDMStateTest, HighBeams) {
    Frame f(0x625, 0, {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDMState state;
    assertTrue(state.handle(f));
    assertEqual(state.defog(), false);
    assertEqual(state.high_beams(), true);
    assertEqual(state.low_beams(), false);
    assertEqual(state.fog_lights(), false);
    assertEqual(state.running_lights(), false);
    assertEqual(state.ac_compressor(), false);
    assertFalse(state.handle(f));
}

test(IPDMStateTest, LowBeams) {
    Frame f(0x625, 0, {0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDMState state;
    assertTrue(state.handle(f));
    assertEqual(state.defog(), false);
    assertEqual(state.high_beams(), false);
    assertEqual(state.low_beams(), true);
    assertEqual(state.fog_lights(), false);
    assertEqual(state.running_lights(), false);
    assertEqual(state.ac_compressor(), false);
    assertFalse(state.handle(f));
}

test(IPDMStateTest, FogLights) {
    Frame f(0x625, 0, {0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDMState state;
    assertTrue(state.handle(f));
    assertEqual(state.defog(), false);
    assertEqual(state.high_beams(), false);
    assertEqual(state.low_beams(), false);
    assertEqual(state.fog_lights(), true);
    assertEqual(state.running_lights(), false);
    assertEqual(state.ac_compressor(), false);
    assertFalse(state.handle(f));
}

test(IPDMStateTest, RunningLights) {
    Frame f(0x625, 0, {0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDMState state;
    assertTrue(state.handle(f));
    assertEqual(state.defog(), false);
    assertEqual(state.high_beams(), false);
    assertEqual(state.low_beams(), false);
    assertEqual(state.fog_lights(), false);
    assertEqual(state.running_lights(), true);
    assertEqual(state.ac_compressor(), false);
    assertFalse(state.handle(f));
}

test(IPDMStateTest, ACCompressor) {
    Frame f(0x625, 0, {0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDMState state;
    assertTrue(state.handle(f));
    assertEqual(state.defog(), false);
    assertEqual(state.high_beams(), false);
    assertEqual(state.low_beams(), false);
    assertEqual(state.fog_lights(), false);
    assertEqual(state.running_lights(), false);
    assertEqual(state.ac_compressor(), true);
    assertFalse(state.handle(f));
}

}  // namespace R51

// Test boilerplate.
void setup() {
#ifdef ARDUINO
    delay(1000);
#endif
    SERIAL_PORT_MONITOR.begin(115200);
    while(!SERIAL_PORT_MONITOR);
}

void loop() {
    aunit::TestRunner::run();
    delay(1);
}
