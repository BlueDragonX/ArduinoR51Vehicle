#include <Arduino.h>
#include <AUnit.h>
#include <NissanR51.h>

namespace NissanR51 {

using namespace aunit;
using ::Canny::Frame;

test(TirePressureState, IgnoreIncorrectID) {
    Frame f(0x384, 0, {0x84, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    TirePressureState state;
    assertFalse(state.handle(f));
}

test(TirePressureStateTest, IgnoreIncorrectSize) {
    Frame f(0x385, 0, {});
    TirePressureState state;
    assertFalse(state.handle(f));
}

test(TirePressureStateTest, NoneSet) {
    Frame f(0x385, 0, {0x84, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    TirePressureState state;
    assertTrue(state.handle(f));
    assertEqual(state.tire_pressure_1(), -1.0);
    assertEqual(state.tire_pressure_2(), -1.0);
    assertEqual(state.tire_pressure_3(), -1.0);
    assertEqual(state.tire_pressure_4(), -1.0);
    assertFalse(state.handle(f));
}

test(TirePressureStateTest, AllSet) {
    Frame f(0x385, 0, {0x84, 0x0C, 0x82, 0x84, 0x79, 0x77, 0x00, 0xF0});

    TirePressureState state;
    assertTrue(state.handle(f));
    assertEqual(state.tire_pressure_1(), 32.5);
    assertEqual(state.tire_pressure_2(), 33.0);
    assertEqual(state.tire_pressure_3(), 30.25);
    assertEqual(state.tire_pressure_4(), 29.75);
    assertFalse(state.handle(f));
}

test(TirePressureStateTest, Tire1Set) {
    Frame f(0x385, 0, {0x84, 0x0C, 0x82, 0x00, 0x00, 0x00, 0x00, 0x80});

    TirePressureState state;
    assertTrue(state.handle(f));
    assertEqual(state.tire_pressure_1(), 32.5);
    assertEqual(state.tire_pressure_2(), -1.0);
    assertEqual(state.tire_pressure_3(), -1.0);
    assertEqual(state.tire_pressure_4(), -1.0);
    assertFalse(state.handle(f));
}

test(TirePressureStateTest, Tire2Set) {
    Frame f(0x385, 0, {0x84, 0x0C, 0x00, 0xA0, 0x00, 0x00, 0x00, 0x40});

    TirePressureState state;
    assertTrue(state.handle(f));
    assertEqual(state.tire_pressure_1(), -1.0);
    assertEqual(state.tire_pressure_2(), 40.0);
    assertEqual(state.tire_pressure_3(), -1.0);
    assertEqual(state.tire_pressure_4(), -1.0);
    assertFalse(state.handle(f));
}

test(TirePressureStateTest, Tire3Set) {
    Frame f(0x385, 0, {0x84, 0x0C, 0x00, 0x00, 0x75, 0x00, 0x00, 0x20});

    TirePressureState state;
    assertTrue(state.handle(f));
    assertEqual(state.tire_pressure_1(), -1.0);
    assertEqual(state.tire_pressure_2(), -1.0);
    assertEqual(state.tire_pressure_3(), 29.25);
    assertEqual(state.tire_pressure_4(), -1.0);
    assertFalse(state.handle(f));
}

test(TirePressureStateTest, Tire4Set) {
    Frame f(0x385, 0, {0x84, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10});

    TirePressureState state;
    assertTrue(state.handle(f));
    assertEqual(state.tire_pressure_1(), -1.0);
    assertEqual(state.tire_pressure_2(), -1.0);
    assertEqual(state.tire_pressure_3(), -1.0);
    assertEqual(state.tire_pressure_4(), 0.0);
    assertFalse(state.handle(f));
}

}  // namespace NissanR51

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
