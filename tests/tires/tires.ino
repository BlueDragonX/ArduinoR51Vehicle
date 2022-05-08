#include <AUnit.h>
#include <Arduino.h>
#include <Faker.h>
#include <R51Test.h>
#include <R51Vehicle.h>

namespace R51 {

using namespace aunit;
using ::Canny::Frame;
using ::Faker::FakeClock;

test(TirePressureState, IgnoreIncorrectID) {
    FakeYield yield;
    Frame f(0x384, 0, {0x84, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    TirePressureState tire;
    tire.handle(f);
    tire.emit(yield);
    assertSize(yield, 0);
}

test(TirePressureStateTest, IgnoreIncorrectSize) {
    FakeYield yield;
    Frame f(0x385, 0, {});

    TirePressureState tire;
    tire.handle(f);
    tire.emit(yield);
    assertSize(yield, 0);
}

test(TirePressureStateTest, Tick) {
    FakeClock clock;
    FakeYield yield;

    TirePressureState tire(200, &clock);
    tire.emit(yield);
    assertSize(yield, 0);

    SystemEvent expect(Event::TIRE_PRESSURE_STATE, {0x00, 0x00, 0x00, 0x00});
    clock.set(200);
    tire.emit(yield);
    assertIsSystemEvent(yield.messages()[0], expect);
}

test(TirePressureStateTest, AllSet) {
    FakeYield yield;
    Frame f(0x385, 0, {0x84, 0x0C, 0x82, 0x84, 0x79, 0x77, 0x00, 0xF0});

    TirePressureState tire;
    tire.handle(f);
    tire.emit(yield);

    SystemEvent expect(Event::TIRE_PRESSURE_STATE, {0x82, 0x84, 0x79, 0x77});
    assertIsSystemEvent(yield.messages()[0], expect);
}

test(TirePressureStateTest, Tire1Set) {
    FakeYield yield;
    Frame f(0x385, 0, {0x84, 0x0C, 0x82, 0x00, 0x00, 0x00, 0x00, 0x80});

    TirePressureState tire;
    tire.handle(f);
    tire.emit(yield);

    SystemEvent expect(Event::TIRE_PRESSURE_STATE, {0x82, 0x00, 0x00, 0x00});
    assertIsSystemEvent(yield.messages()[0], expect);
}

test(TirePressureStateTest, Tire2Set) {
    FakeYield yield;
    Frame f(0x385, 0, {0x84, 0x0C, 0x00, 0xA0, 0x00, 0x00, 0x00, 0x40});

    TirePressureState tire;
    tire.handle(f);
    tire.emit(yield);

    SystemEvent expect(Event::TIRE_PRESSURE_STATE, {0x00, 0xA0, 0x00, 0x00});
    assertIsSystemEvent(yield.messages()[0], expect);
}

test(TirePressureStateTest, Tire3Set) {
    FakeYield yield;
    Frame f(0x385, 0, {0x84, 0x0C, 0x00, 0x00, 0x75, 0x00, 0x00, 0x20});


    TirePressureState tire;
    tire.handle(f);
    tire.emit(yield);

    SystemEvent expect(Event::TIRE_PRESSURE_STATE, {0x00, 0x00, 0x75, 0x00});
    assertIsSystemEvent(yield.messages()[0], expect);
}

test(TirePressureStateTest, Tire4Set) {
    FakeYield yield;
    Frame f(0x385, 0, {0x84, 0x0C, 0x00, 0x00, 0x00, 0x77, 0x00, 0x10});


    TirePressureState tire;
    tire.handle(f);
    tire.emit(yield);

    SystemEvent expect(Event::TIRE_PRESSURE_STATE, {0x00, 0x00, 0x00, 0x77});
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
