#include "ClimateControl.h"

#include <Arduino.h>
#include <Canny.h>
#include "Binary.h"

namespace R51 {
namespace {

bool isInit(const Canny::Frame& frame) {
    return frame.data()[0] == 0x80;
}

}  // namespace

ClimateSystemControl::ClimateSystemControl(bool ready) : frame_(0x540, 0, 8), available_(true) {
    frame_.data()[0] = 0x80;
    if (ready) {
        this->ready();
    }
}

const Canny::Frame& ClimateSystemControl::frame() {
    available_ = false;
    return frame_;
}

void ClimateSystemControl::ready() {
    if (!isInit(frame_)) {
        return;
    }
    available_ = true;
    frame_.data()[0] = 0x60;
    frame_.data()[1] = 0x40;
    frame_.data()[6] = 0x04;
}

void ClimateSystemControl::turnOff() {
    if (isInit(frame_)) {
        return;
    }
    available_ = true;
    toggleBit(frame_.data(), 6, 7);
}

void ClimateSystemControl::toggleAuto() {
    if (isInit(frame_)) {
        return;
    }
    available_ = true;
    toggleBit(frame_.data(), 6, 5);
}

void ClimateSystemControl::toggleAC() {
    if (isInit(frame_)) {
        return;
    }
    available_ = true;
    toggleBit(frame_.data(), 5, 3);
}

void ClimateSystemControl::toggleDual() {
    if (isInit(frame_)) {
        return;
    }
    available_ = true;
    toggleBit(frame_.data(), 6, 3);
}

void ClimateSystemControl::cycleMode() {
    if (isInit(frame_)) {
        return;
    }
    available_ = true;
    toggleBit(frame_.data(), 6, 0);
}

void ClimateSystemControl::toggleDefrost() {
    if (isInit(frame_)) {
        return;
    }
    available_ = true;
    toggleBit(frame_.data(), 6, 1);
}

void ClimateSystemControl::incDriverTemp() {
    if (isInit(frame_)) {
        return;
    }
    available_ = true;
    toggleBit(frame_.data(), 5, 5);
    frame_.data()[3]++;
}

void ClimateSystemControl::decDriverTemp() {
    if (isInit(frame_)) {
        return;
    }
    available_ = true;
    toggleBit(frame_.data(), 5, 5);
    frame_.data()[3]--;
}

void ClimateSystemControl::incPassengerTemp() {
    if (isInit(frame_)) {
        return;
    }
    available_ = true;
    toggleBit(frame_.data(), 5, 5);
    frame_.data()[4]++;
}

void ClimateSystemControl::decPassengerTemp() {
    if (isInit(frame_)) {
        return;
    }
    available_ = true;
    toggleBit(frame_.data(), 5, 5);
    frame_.data()[4]--;
}

ClimateFanControl::ClimateFanControl(bool ready) : frame_(0x541, 0, 8), available_(true) {
    frame_.data()[0] = 0x80;
    if (ready) {
        this->ready();
    }
}

const Canny::Frame& ClimateFanControl::frame() {
    available_ = false;
    return frame_;
}

void ClimateFanControl::ready() {
    if (!isInit(frame_)) {
        return;
    }
    available_ = true;
    frame_.data()[0] = 0x00;
}

void ClimateFanControl::toggleRecirculate() {
    if (isInit(frame_)) {
        return;
    }
    available_ = true;
    toggleBit(frame_.data(), 1, 6);
}

void ClimateFanControl::incFanSpeed() {
    if (isInit(frame_)) {
        return;
    }
    available_ = true;
    toggleBit(frame_.data(), 0, 5);
}

void ClimateFanControl::decFanSpeed() {
    if (isInit(frame_)) {
        return;
    }
    available_ = true;
    toggleBit(frame_.data(), 0, 4);
}

}  // namespace R51
