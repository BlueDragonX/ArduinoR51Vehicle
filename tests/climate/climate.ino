#include <AUnit.h>
#include <Arduino.h>
#include <Canny.h>
#include <Caster.h>
#include <Faker.h>
#include <R51Core.h>
#include <R51Test.h>
#include <R51Vehicle.h>

namespace R51 {

using namespace aunit;
using ::Canny::Frame;
using ::Faker::FakeClock;

#define assertYieldFrame(control, frame) ({\
    climate.handle(control); \
    climate.emit(yield); \
    assertSize(yield, 1); \
    assertIsCANFrame(yield.messages()[0], frame); \
    yield.clear(); \
})

#define assertNoYield(control) ({\
    climate.handle(control); \
    climate.emit(yield); \
    assertSize(yield, 0); \
})

class ClimateTest : public TestOnce {
    public:
        void setup() {
            TestOnce::setup();
            clock.set(0);
            yield.clear();
        }

        void initClimate(Climate* climate) {
            clock.set(1000);
            climate->emit(yield);
            yield.clear();
        }

        void enableClimate(Climate* climate) {
            Frame state54A(0x54A, 0, {0x3C, 0x3E, 0x7F, 0x80, 0x3C, 0x41, 0x00, 0x58});
            Frame state54B(0x54B, 0, {0x59, 0x8C, 0x05, 0x24, 0x00, 0x00, 0x00, 0x02});
            climate->handle(state54A);
            climate->handle(state54B);
            climate->emit(yield);
            yield.clear();
        }

        FakeClock clock;
        FakeYield yield;
};

testF(ClimateTest, Init) {
    Climate climate(0, &clock);

    Frame init540(0x540, 0, {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    Frame init541(0x541, 0, {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    Frame ready540(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00});
    Frame ready541(0x541, 0, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    for (int i = 0; i < 4; i++) {
        clock.delay(100);
        climate.emit(yield);
        assertSize(yield, 2);
        assertIsCANFrame(yield.messages()[0], init540);
        assertIsCANFrame(yield.messages()[1], init541);
        yield.clear();
    }
    clock.delay(100);
    climate.emit(yield);
    assertSize(yield, 2);
    assertIsCANFrame(yield.messages()[0], ready540);
    assertIsCANFrame(yield.messages()[1], ready541);
}

testF(ClimateTest, TurnOff) {
    Climate climate(0, &clock);
    initClimate(&climate);

    Frame expect;
    SystemEvent control(Event::CLIMATE_TURN_OFF);

    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x84, 0x00});
    assertYieldFrame(control, expect);

    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00});
    assertYieldFrame(control, expect);
}

testF(ClimateTest, ToggleAuto) {
    Climate climate(0, &clock);
    initClimate(&climate);

    SystemEvent control(Event::CLIMATE_TOGGLE_AUTO);
    Frame expect;

    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00});
    assertYieldFrame(control, expect);

    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00});
    assertYieldFrame(control, expect);
}

testF(ClimateTest, ToggleAc) {
    Climate climate(0, &clock);
    initClimate(&climate);

    SystemEvent control(Event::CLIMATE_TOGGLE_AC);
    Frame expect;

    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x08, 0x04, 0x00});
    assertYieldFrame(control, expect);

    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00});
    assertYieldFrame(control, expect);
}

testF(ClimateTest, ToggleDual) {
    Climate climate(0, &clock);
    initClimate(&climate);

    SystemEvent control(Event::CLIMATE_TOGGLE_DUAL);
    Frame expect;

    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00});
    assertYieldFrame(control, expect);

    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00});
    assertYieldFrame(control, expect);
}

testF(ClimateTest, CycleMode) {
    Climate climate(0, &clock);
    initClimate(&climate);

    SystemEvent control(Event::CLIMATE_CYCLE_AIRFLOW_MODE);
    Frame expect;

    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00});
    assertYieldFrame(control, expect);

    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00});
    assertYieldFrame(control, expect);
}

testF(ClimateTest, ToggleDefrost) {
    Climate climate(0, &clock);
    initClimate(&climate);

    SystemEvent control(Event::CLIMATE_TOGGLE_DEFROST);
    Frame expect;

    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00});
    assertYieldFrame(control, expect);

    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00});
    assertYieldFrame(control, expect);
}

testF(ClimateTest, ToggleRecirculate) {
    Climate climate(0, &clock);
    initClimate(&climate);

    SystemEvent control(Event::CLIMATE_TOGGLE_RECIRCULATE);
    Frame expect;

    expect = Frame(0x541, 0, {0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    assertYieldFrame(control, expect);

    expect = Frame(0x541, 0, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    assertYieldFrame(control, expect);
}

testF(ClimateTest, TriggerFanSpeedUp) {
    Climate climate(0, &clock);
    initClimate(&climate);

    SystemEvent control(Event::CLIMATE_INC_FAN_SPEED);
    Frame expect;

    expect = Frame(0x541, 0, {0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    assertYieldFrame(control, expect);

    expect = Frame(0x541, 0, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    assertYieldFrame(control, expect);
}

testF(ClimateTest, TriggerFanSpeedDown) {
    Climate climate(0, &clock);
    initClimate(&climate);

    SystemEvent control(Event::CLIMATE_DEC_FAN_SPEED);
    Frame expect;

    expect = Frame(0x541, 0, {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    assertYieldFrame(control, expect);

    expect = Frame(0x541, 0, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    assertYieldFrame(control, expect);
}

testF(ClimateTest, TriggerDriverTempWhenOn) {
    Climate climate(0, &clock);
    initClimate(&climate);
    enableClimate(&climate);

    SystemEvent control;
    Frame expect;

    // decrease temp
    control = SystemEvent(Event::CLIMATE_DEC_DRIVER_TEMP);
    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0xFF, 0x00, 0x20, 0x04, 0x00});
    assertYieldFrame(control, expect);

    // increase temp
    control = SystemEvent(Event::CLIMATE_INC_DRIVER_TEMP);
    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00});
    assertYieldFrame(control, expect);

    // increase temp
    control = SystemEvent(Event::CLIMATE_INC_DRIVER_TEMP);
    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0x01, 0x00, 0x20, 0x04, 0x00});
    assertYieldFrame(control, expect);

    // decrease temp
    control = SystemEvent(Event::CLIMATE_DEC_DRIVER_TEMP);
    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00});
    assertYieldFrame(control, expect);
}

testF(ClimateTest, TriggerDriverTempWhenOff) {
    Climate climate(0, &clock);
    initClimate(&climate);

    SystemEvent control;

    // increase temp
    control = SystemEvent(Event::CLIMATE_INC_DRIVER_TEMP);
    assertNoYield(control);

    // decrease temp
    control = SystemEvent(Event::CLIMATE_DEC_DRIVER_TEMP);
    assertNoYield(control);
}

testF(ClimateTest, TriggerPassengerTempWhenOn) {
    Climate climate(0, &clock);
    initClimate(&climate);
    enableClimate(&climate);

    SystemEvent control;
    Frame expect;

    // decrease temp
    control = SystemEvent(Event::CLIMATE_DEC_PASSENGER_TEMP);
    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0xFF, 0x20, 0x04, 0x00});
    assertYieldFrame(control, expect);

    // increase temp
    control = SystemEvent(Event::CLIMATE_INC_PASSENGER_TEMP);
    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00});
    assertYieldFrame(control, expect);

    // increase temp
    control = SystemEvent(Event::CLIMATE_INC_PASSENGER_TEMP);
    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x01, 0x20, 0x04, 0x00});
    assertYieldFrame(control, expect);

    // decrease temp
    control = SystemEvent(Event::CLIMATE_DEC_PASSENGER_TEMP);
    expect = Frame(0x540, 0, {0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00});
    assertYieldFrame(control, expect);
}

testF(ClimateTest, TriggerPassengerTempWhenOff) {
    Climate climate(0, &clock);
    initClimate(&climate);

    SystemEvent control;

    // increase temp
    control = SystemEvent(Event::CLIMATE_INC_PASSENGER_TEMP);
    assertNoYield(control);

    // decrease temp
    control = SystemEvent(Event::CLIMATE_DEC_PASSENGER_TEMP);
    assertNoYield(control);
}

testF(ClimateTest, TickOffState) {
    Climate climate(100, &clock);
    initClimate(&climate);
    enableClimate(&climate);

    Frame state54A(0x54A, 0, {0x3C, 0x3E, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x2C});
    Frame state54B(0x54B, 0, {0xF2, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x02});

    ClimateTempStateEvent temp;
    temp.outside_temp(0x2C);
    temp.units(UNITS_US);
    ClimateAirflowStateEvent airflow;
    ClimateSystemStateEvent system;
    system.mode(CLIMATE_SYSTEM_OFF);

    climate.handle(state54A);
    climate.handle(state54B);
    climate.emit(yield);
    assertSize(yield, 3);
    assertIsSystemEvent(yield.messages()[0], temp);
    assertIsSystemEvent(yield.messages()[1], system);
    assertIsSystemEvent(yield.messages()[2], airflow);
    yield.clear();

    clock.delay(100);
    climate.emit(yield);
    assertSize(yield, 3);
    assertIsSystemEvent(yield.messages()[0], temp);
    assertIsSystemEvent(yield.messages()[1], system);
    assertIsSystemEvent(yield.messages()[2], airflow);
    yield.clear();
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
