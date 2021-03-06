#include <AUnit.h>
#include <Arduino.h>
#include <Faker.h>
#include <R51Test.h>
#include <R51Vehicle.h>

namespace R51 {

using namespace aunit;
using ::Canny::Frame;
using ::Faker::FakeClock;

test(EngineTempStateTest, IgnoreIncorrectID) {
    FakeYield yield;
    Frame f(0x550, 0, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    EngineTempState ecm;
    ecm.handle(f);
    ecm.emit(yield);
    assertSize(yield, 0);
}

test(EngineTempStateTest, IgnoreIncorrectSize) {
    FakeYield yield;
    Frame f(0x551, 0, {});

    EngineTempState ecm;
    ecm.handle(f);
    ecm.emit(yield);
    assertSize(yield, 0);
}

test(EngineTempStateTest, Tick) {
    FakeClock clock;
    FakeYield yield;

    EngineTempState ecm(200, &clock);
    ecm.emit(yield);
    assertSize(yield, 0);

    SystemEvent expect(Event::ENGINE_TEMP_STATE, {0x00});
    clock.set(200);
    ecm.emit(yield);
    assertSize(yield, 1);
    assertIsSystemEvent(yield.messages()[0], expect);
}

test(EngineTempStateTest, PositiveTemp) {
    FakeYield yield;
    Frame f(0x551, 0, {0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    EngineTempState ecm;
    ecm.handle(f);
    ecm.emit(yield);

    SystemEvent expect(Event::ENGINE_TEMP_STATE, {0x29});
    assertSize(yield, 1);
    assertIsSystemEvent(yield.messages()[0], expect);
}

test(EngineTempStateTest, MaxTemp) {
    FakeYield yield;
    Frame f(0x551, 0, {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    EngineTempState ecm;
    ecm.handle(f);
    ecm.emit(yield);

    SystemEvent expect(Event::ENGINE_TEMP_STATE, {0xFF});
    assertSize(yield, 1);
    assertIsSystemEvent(yield.messages()[0], expect);
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
