#include <Arduino.h>
#include <AUnit.h>
#include <NissanR51.h>

namespace NissanR51 {

using namespace aunit;
using ::Canny::Frame;

test(ClimateTemperatureStateTest, IgnoreIncorrectID) {
    Frame f(0x54C, 0, {0x3C, 0x3E, 0x7F, 0x80, 0x3C, 0x41, 0x00, 0x58});
    ClimateTemperatureState state;
    assertFalse(state.handle(f));
}

test(ClimateTemperatureStateTest, IgnoreIncorrectSize) {
    Frame f(0x54A, 0, {0x3C, 0x3E, 0x7F, 0x80, 0x3C, 0x41});
    ClimateTemperatureState state;
    assertFalse(state.handle(f));
}

test(ClimateTemperatureStateTest, HandleTemperatures) {
    Frame f(0x54A, 0, {0x3C, 0x3E, 0x7F, 0x80, 0x3C, 0x41, 0x00, 0x58});
    
    ClimateTemperatureState state;
    assertTrue(state.handle(f));
    assertEqual(state.units(), UNITS_US);
    assertEqual(state.driver_temp(), 60);
    assertEqual(state.passenger_temp(), 65);
    assertEqual(state.outside_temp(), 88);
    assertFalse(state.handle(f));
}

test(ClimateSystemStateTest, HandleOff) {
    Frame f(0x54B, 0, {0xF2, 0x00, 0x02, 0x24, 0x00, 0x00, 0x00, 0x02});

    ClimateSystemState state;
    assertTrue(state.handle(f));
    assertEqual(state.system(), CLIMATE_SYSTEM_OFF);
    assertEqual(state.vents(), CLIMATE_VENTS_CLOSED);
    assertEqual(state.ac(), false);
    assertEqual(state.dual(), false);
    assertEqual(state.recirculate(), false);
    assertEqual(state.fan_speed(), 1);
    assertFalse(state.handle(f));
}

test(ClimateSystemStateTest, HandleAuto) {
    Frame f(0x54B, 0, {0x59, 0x8C, 0x05, 0x24, 0x00, 0x00, 0x00, 0x02});

    ClimateSystemState state;
    assertTrue(state.handle(f));
    assertEqual(state.system(), CLIMATE_SYSTEM_AUTO);
    assertEqual(state.vents(), CLIMATE_VENTS_FEET);
    assertEqual(state.ac(), true);
    assertEqual(state.dual(), false);
    assertEqual(state.recirculate(), false);
    assertEqual(state.fan_speed(), 3);
    assertFalse(state.handle(f));
}

test(ClimateSystemStateTest, HandleAutoDual) {
    Frame f(0x54B, 0, {0x59, 0x08, 0x02, 0xE2, 0x00, 0x00, 0x00, 0x02});

    ClimateSystemState state;
    assertTrue(state.handle(f));
    assertEqual(state.system(), CLIMATE_SYSTEM_AUTO);
    assertEqual(state.vents(), CLIMATE_VENTS_FACE_FEET);
    assertEqual(state.ac(), true);
    assertEqual(state.dual(), true);
    assertEqual(state.recirculate(), false);
    assertEqual(state.fan_speed(), 1);
    assertFalse(state.handle(f));
}

test(ClimateSystemStateTest, HandleManual) {
    Frame f(0x54B, 0, {0x5A, 0x08, 0x02, 0xD2, 0x00, 0x00, 0x00, 0x00});

    ClimateSystemState state;
    assertTrue(state.handle(f));
    assertEqual(state.system(), CLIMATE_SYSTEM_MANUAL);
    assertEqual(state.vents(), CLIMATE_VENTS_FACE_FEET);
    assertEqual(state.ac(), true);
    assertEqual(state.dual(), true);
    assertEqual(state.recirculate(), true);
    assertEqual(state.fan_speed(), 1);
    assertFalse(state.handle(f));
}

test(ClimateSystemStateTest, HandleDefrost) {
    Frame f(0x54B, 0, {0x5A, 0x34, 0x06, 0x24, 0x00, 0x00, 0x00, 0x02});

    ClimateSystemState state;
    assertTrue(state.handle(f));
    assertEqual(state.system(), CLIMATE_SYSTEM_DEFROST);
    assertEqual(state.vents(), CLIMATE_VENTS_WINDSHIELD);
    assertEqual(state.ac(), true);
    assertEqual(state.dual(), false);
    assertEqual(state.recirculate(), false);
    assertEqual(state.fan_speed(), 3);
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
