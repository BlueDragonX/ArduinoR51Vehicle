#include <AUnit.h>
#include <Arduino.h>
#include <Faker.h>
#include <R51Test.h>
#include <R51Vehicle.h>

namespace R51 {

using namespace aunit;
using ::Canny::Frame;
using ::Faker::FakeClock;

test(IPDMTest, IgnoreIncorrectID) {
    FakeYield yield;
    Frame f(0x624, 0, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDM ipdm;
    ipdm.handle(f);
    ipdm.emit(yield);
    assertSize(yield, 0);
}

test(IPDMTest, IgnoreIncorrectSize) {
    FakeYield yield;
    Frame f(0x625, 0, {0x00, 0x00, 0x00});

    IPDM ipdm;
    ipdm.handle(f);
    ipdm.emit(yield);
    assertSize(yield, 0);
}

test(IPDMTest, Tick) {
    FakeClock clock;
    FakeYield yield;

    IPDM ipdm(200, &clock);
    ipdm.emit(yield);
    assertSize(yield, 0);

    SystemEvent expect(Event::BODY_POWER_STATE, {0x00});
    clock.set(200);
    ipdm.emit(yield);
    assertSize(yield, 1);
    assertIsSystemEvent(yield.messages()[0], expect);
}

test(IPDMTest, Defog) {
    FakeYield yield;
    Frame f(0x625, 0, {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDM ipdm;
    ipdm.handle(f);
    ipdm.emit(yield);
    
    SystemEvent expect(Event::BODY_POWER_STATE, {0x40});
    assertSize(yield, 1);
    assertIsSystemEvent(yield.messages()[0], expect);
}

test(IPDMTest, HighBeams) {
    FakeYield yield;
    Frame f(0x625, 0, {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDM ipdm;
    ipdm.handle(f);
    ipdm.emit(yield);

    SystemEvent expect(Event::BODY_POWER_STATE, {0x01});
    assertSize(yield, 1);
    assertIsSystemEvent(yield.messages()[0], expect);
}

test(IPDMTest, LowBeams) {
    FakeYield yield;
    Frame f(0x625, 0, {0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDM ipdm;
    ipdm.handle(f);
    ipdm.emit(yield);

    SystemEvent expect(Event::BODY_POWER_STATE, {0x02});
    assertSize(yield, 1);
    assertIsSystemEvent(yield.messages()[0], expect);
}

test(IPDMTest, FogLights) {
    FakeYield yield;
    Frame f(0x625, 0, {0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDM ipdm;
    ipdm.handle(f);
    ipdm.emit(yield);

    SystemEvent expect(Event::BODY_POWER_STATE, {0x08});
    assertSize(yield, 1);
    assertIsSystemEvent(yield.messages()[0], expect);
}

test(IPDMTest, RunningLights) {
    FakeYield yield;
    Frame f(0x625, 0, {0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDM ipdm;
    ipdm.handle(f);
    ipdm.emit(yield);

    SystemEvent expect(Event::BODY_POWER_STATE, {0x04});
    assertSize(yield, 1);
    assertIsSystemEvent(yield.messages()[0], expect);
}

test(IPDMTest, ACCompressor) {
    FakeYield yield;
    Frame f(0x625, 0, {0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDM ipdm;
    ipdm.handle(f);
    ipdm.emit(yield);

    SystemEvent expect(Event::BODY_POWER_STATE, {0x80});
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
