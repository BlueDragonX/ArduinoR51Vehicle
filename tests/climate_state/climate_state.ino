#include <Arduino.h>
#include <AUnit.h>
#include <NissanR51.h>

using namespace aunit;
using ::Canny::Frame;
using ::NissanR51::ClimateSystem;
using ::NissanR51::ClimateTemperature;

test(ClimateTemperatureTest, Handle54A) {
    Frame f(0x54A, 0, {0x3C, 0x3E, 0x7F, 0x80, 0x3C, 0x41, 0x00, 0x58});
    
    ClimateTemperature state;
    assertTrue(state.handle(f));
    assertEqual(state.units, ClimateTemperature::UNITS_US);
    assertEqual(state.driver_temp, 60);
    assertEqual(state.passenger_temp, 65);
    assertEqual(state.outside_temp, 88);
    assertFalse(state.handle(f));
}

test(ClimateSystemTest, Handle54BOff) {
    Frame f(0x54B, 0, {0xF2, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x02});

    ClimateSystem state;
    state.system = ClimateSystem::SYSTEM_MANUAL;
    assertTrue(state.handle(f));
    assertEqual(state.system, ClimateSystem::SYSTEM_OFF);
    assertEqual(state.vents, ClimateSystem::VENTS_CLOSED);
    assertEqual(state.ac, false);
    assertEqual(state.dual, false);
    assertEqual(state.recirculate, false);
    assertEqual(state.fan_speed, 0);
    assertFalse(state.handle(f));
}

test(ClimateSystemTest, Handle54BAuto) {
    Frame f(0x54B, 0, {0x59, 0x8C, 0x05, 0x24, 0x00, 0x00, 0x00, 0x02});

    ClimateSystem state;
    assertTrue(state.handle(f));
    assertEqual(state.system, ClimateSystem::SYSTEM_AUTO);
    assertEqual(state.vents, ClimateSystem::VENTS_FEET);
    assertEqual(state.ac, true);
    assertEqual(state.dual, false);
    assertEqual(state.recirculate, false);
    assertEqual(state.fan_speed, 3);
    assertFalse(state.handle(f));
}

test(ClimateSystemTest, Handle54BAutoDual) {
    Frame f(0x54B, 0, {0x59, 0x08, 0x02, 0xE2, 0x00, 0x00, 0x00, 0x02});

    ClimateSystem state;
    assertTrue(state.handle(f));
    assertEqual(state.system, ClimateSystem::SYSTEM_AUTO);
    assertEqual(state.vents, ClimateSystem::VENTS_FACE_FEET);
    assertEqual(state.ac, true);
    assertEqual(state.dual, true);
    assertEqual(state.recirculate, false);
    assertEqual(state.fan_speed, 1);
    assertFalse(state.handle(f));
}

test(ClimateSystemTest, Handle54BManual) {
    Frame f(0x54B, 0, {0x5A, 0x08, 0x02, 0xD2, 0x00, 0x00, 0x00, 0x00});

    ClimateSystem state;
    assertTrue(state.handle(f));
    assertEqual(state.system, ClimateSystem::SYSTEM_MANUAL);
    assertEqual(state.vents, ClimateSystem::VENTS_FACE_FEET);
    assertEqual(state.ac, true);
    assertEqual(state.dual, true);
    assertEqual(state.recirculate, true);
    assertEqual(state.fan_speed, 1);
    assertFalse(state.handle(f));
}

test(ClimateSystemTest, Handle54BDefrost) {
    Frame f(0x54B, 0, {0x5A, 0x34, 0x06, 0x24, 0x00, 0x00, 0x00, 0x02});

    ClimateSystem state;
    assertTrue(state.handle(f));
    assertEqual(state.system, ClimateSystem::SYSTEM_DEFROST);
    assertEqual(state.vents, ClimateSystem::VENTS_WINDSHIELD);
    assertEqual(state.ac, true);
    assertEqual(state.dual, false);
    assertEqual(state.recirculate, false);
    assertEqual(state.fan_speed, 3);
    assertFalse(state.handle(f));
}

// Test boilerplate.
void setup() {
#ifdef ARDUINO
    delay(1000);
#endif
    SERIAL_PORT_MONITOR.begin(115200);
    while(!SERIAL_PORT_MONITOR);
}

void loop() {
    TestRunner::run();
    delay(1);
}
