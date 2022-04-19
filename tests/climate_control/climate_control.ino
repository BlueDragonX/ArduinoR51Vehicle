#include <AUnit.h>
#include <Arduino.h>
#include <R51Vehicle.h>

namespace R51 {

using namespace aunit;
using ::Canny::Frame;

bool checkFrame(const Frame& a, const Frame& b) {
    if (a == b) {
        return true;
    }
    SERIAL_PORT_MONITOR.print(a);
    SERIAL_PORT_MONITOR.print(" != ");
    SERIAL_PORT_MONITOR.println(b);
    return false;
}

test(ClimateSystemControlTest, Init) {
    ClimateSystemControl c;
    Frame expect(0x540, 0, {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    assertTrue(c.available());
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());
}

test(ClimateSystemControlTest, Ready) {
    ClimateSystemControl c(true);
    Frame expect(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00});

    assertTrue(c.available());
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());
}

test(ClimateSystemControlTest, InitThenReady) {
    ClimateSystemControl c;

    assertTrue(c.available());
    Frame expect(0x540, 0, {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());

    c.ready();
    assertTrue(c.available());
    expect.data({0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());
}

test(ClimateSystemControlTest, TurnOff) {
    ClimateSystemControl c(true);
    c.frame(); // clear available

    c.turnOff();
    assertTrue(c.available());
    Frame expect(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x84, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());

    c.turnOff();
    assertTrue(c.available());
    expect.data({0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());
}

test(ClimateSystemControlTest, ToggleAuto) {
    ClimateSystemControl c(true);
    c.frame();

    c.toggleAuto();
    assertTrue(c.available());
    Frame expect(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());

    c.toggleAuto();
    assertTrue(c.available());
    expect.data({0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());
}

test(ClimateSystemControlTest, ToggleAC) {
    ClimateSystemControl c(true);
    c.frame();

    c.toggleAC();
    assertTrue(c.available());
    Frame expect(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x08, 0x04, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());

    c.toggleAC();
    assertTrue(c.available());
    expect.data({0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());
}

test(ClimateSystemControlTest, ToggleDual) {
    ClimateSystemControl c(true);
    c.frame();

    c.toggleDual();
    assertTrue(c.available());
    Frame expect(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());

    c.toggleDual();
    assertTrue(c.available());
    expect.data({0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());
}

test(ClimateSystemControlTest, CycleMode) {
    ClimateSystemControl c(true);
    c.frame();

    c.cycleMode();
    assertTrue(c.available());
    Frame expect(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());

    c.cycleMode();
    assertTrue(c.available());
    expect.data({0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());
}

test(ClimateSystemControlTest, ToggleDefrost) {
    ClimateSystemControl c(true);
    c.frame();

    c.toggleDefrost();
    assertTrue(c.available());
    Frame expect(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());

    c.toggleDefrost();
    assertTrue(c.available());
    expect.data({0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());
}

test(ClimateSystemControlTest, IncDriverTemp) {
    ClimateSystemControl c(true);
    c.frame();

    c.incDriverTemp();
    assertTrue(c.available());
    Frame expect(0x540, 0, {0x60, 0x40, 0x00, 0x01, 0x00, 0x20, 0x04, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());

    c.incDriverTemp();
    assertTrue(c.available());
    expect.data({0x60, 0x40, 0x00, 0x02, 0x00, 0x00, 0x04, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());
}

test(ClimateSystemControlTest, DecDriverTemp) {
    ClimateSystemControl c(true);
    c.frame();

    c.decDriverTemp();
    assertTrue(c.available());
    Frame expect(0x540, 0, {0x60, 0x40, 0x00, 0xFF, 0x00, 0x20, 0x04, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());

    c.decDriverTemp();
    assertTrue(c.available());
    expect.data({0x60, 0x40, 0x00, 0xFE, 0x00, 0x00, 0x04, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());
}

test(ClimateSystemControlTest, IncPassengerTemp) {
    ClimateSystemControl c(true);
    c.frame();

    c.incPassengerTemp();
    assertTrue(c.available());
    Frame expect(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x01, 0x20, 0x04, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());

    c.incPassengerTemp();
    assertTrue(c.available());
    expect.data({0x60, 0x40, 0x00, 0x00, 0x02, 0x00, 0x04, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());
}

test(ClimateSystemControlTest, DecPassengerTemp) {
    ClimateSystemControl c(true);
    c.frame();

    c.decPassengerTemp();
    assertTrue(c.available());
    Frame expect(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0xFF, 0x20, 0x04, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());

    c.decPassengerTemp();
    assertTrue(c.available());
    expect.data({0x60, 0x40, 0x00, 0x00, 0xFE, 0x00, 0x04, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());
}

test(ClimateFanControlTest, Init) {
    ClimateFanControl c;
    Frame expect(0x541, 0, {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    assertTrue(c.available());
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());
}

test(ClimateFanControlTest, Ready) {
    ClimateFanControl c(true);
    Frame expect(0x541, 0, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    assertTrue(c.available());
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());
}

test(ClimateFanControlTest, InitThenReady) {
    ClimateFanControl c;

    assertTrue(c.available());
    Frame expect(0x541, 0, {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());

    c.ready();
    assertTrue(c.available());
    expect.data({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());
}

test(ClimateFanControlTest, ToggleRecirculate) {
    ClimateFanControl c(true);
    c.frame();

    c.toggleRecirculate();
    assertTrue(c.available());
    Frame expect(0x541, 0, {0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());

    c.toggleRecirculate();
    assertTrue(c.available());
    expect.data({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());
}

test(ClimateFanControlTest, IncFanSpeed) {
    ClimateFanControl c(true);
    c.frame();

    c.incFanSpeed();
    assertTrue(c.available());
    Frame expect(0x541, 0, {0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());

    c.incFanSpeed();
    assertTrue(c.available());
    expect.data({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());
}

test(ClimateFanControlTest, DecFanSpeed) {
    ClimateFanControl c(true);
    c.frame();

    c.decFanSpeed();
    assertTrue(c.available());
    Frame expect(0x541, 0, {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());

    c.decFanSpeed();
    assertTrue(c.available());
    expect.data({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    assertTrue(checkFrame(c.frame(), expect));
    assertFalse(c.available());
}

}  // namespace 

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
