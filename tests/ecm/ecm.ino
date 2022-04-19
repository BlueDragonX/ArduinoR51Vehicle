#include <Arduino.h>
#include <AUnit.h>
#include <R51Vehicle.h>

namespace R51 {

using namespace aunit;
using ::Canny::Frame;

test(ECMCoolantStateTest, IgnoreIncorrectID) {
    Frame f(0x550, 0, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    ECMCoolantState state;
    assertFalse(state.handle(f));
}

test(ECMCoolantStateTest, IgnoreIncorrectSize) {
    Frame f(0x551, 0, {});
    ECMCoolantState state;
    assertFalse(state.handle(f));
}

test(ECMCoolantStateTest, MinTemp) {
    Frame f(0x551, 0, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    ECMCoolantState state;
    assertTrue(state.handle(f));
    assertEqual(state.coolant_temp(), -40);
    assertFalse(state.handle(f));
}

test(ECMCoolantStateTest, PositiveTemp) {
    Frame f(0x551, 0, {0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    ECMCoolantState state;
    assertTrue(state.handle(f));
    assertEqual(state.coolant_temp(), 1);
    assertFalse(state.handle(f));
}

test(ECMCoolantStateTest, MaxTemp) {
    Frame f(0x551, 0, {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    ECMCoolantState state;
    assertTrue(state.handle(f));
    assertEqual(state.coolant_temp(), 215);
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
