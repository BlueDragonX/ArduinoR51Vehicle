#ifndef _NISSAN_R51_SETTINGS_H_
#define _NISSAN_R51_SETTINGS_H_

#include <Arduino.h>
#include <Canny.h>
#include <Faker.h>
#include "Controller.h"
#include "Handler.h"

namespace NissanR51 {
namespace internal {

class SettingsImpl;

}  // namespace internal

class SettingsInit;
class SettingsRetrieve;
class SettingsUpdate;
class SettingsReset;

// Communicates with the BCM to retrieve and update body control settings.
class Settings : public Handler, public Controller {
    public:
        enum RemoteKeyResponseLights : uint8_t {
            LIGHTS_OFF = 0,
            LIGHTS_UNLOCK = 1,
            LIGHTS_LOCK = 2,
            LIGHTS_ON = 3,
        };

        enum AutoHeadlightOffDelay : uint8_t {
            DELAY_0S = 0,
            DELAY_30S = 2,
            DELAY_45S = 3,
            DELAY_60S = 4,
            DELAY_90S = 6,
            DELAY_120S = 8,
            DELAY_150S = 10,
            DELAY_180S = 12,
        };

        enum AutoReLockTime : uint8_t {
            RELOCK_OFF = 0,
            RELOCK_1M = 1,
            RELOCK_5M = 5,
        };

        Settings(Faker::Clock* clock = Faker::Clock::real());

        // Return true if a control frame is available to be sent. The frame()
        // getter should be used to retrieve the available frame. A settings
        // control frame should only be sent once. A subsequent call to
        // available() after frame() will prepare the next frame to be sent.
        bool available() override;

        // Return a reference to the current control frame. Resets available.
        const Canny::Frame& frame() override;

        // Handle BCM state frames 0x72E and 0x72F.
        bool handle(const Canny::Frame& frame) override;

        // Exchange init frames with BCM. 
        bool init();

        // Retrieve the current settings from the BCM.
        bool retrieveSettings();

        // Return true if auto interior illumination is enabled.
        bool autoInteriorIllumination() const;

        // Return the auto-headlight sensitivity setting. Returns a value from
        // 0 to 3 inclusive.
        uint8_t autoHeadlightSensitivity() const;

        // Return the auto-headlight off delay setting.
        AutoHeadlightOffDelay autoHeadlightOffDelay() const;

        // Return true if speed sensitive wiper interval is enabled.
        bool speedSensingWiperInterval() const;

        // Return true if the remote key response horn is enabled.
        bool remoteKeyResponseHorn() const;

        // Return the remote key response lights setting.
        RemoteKeyResponseLights remoteKeyResponseLights() const;

        // Return the auto re-lock time setting.
        AutoReLockTime autoReLockTime() const;

        // Return true if selective door unlock is enabled.
        bool selectiveDoorUnlock() const;

        // Return true if the "slide driver seat back on exit" setting is enabled.
        bool slideDriverSeatBackOnExit() const;

        // Toggle the auto interior illumination setting.
        bool toggleAutoInteriorIllumination();

        // Increase the auto healight sensitivity.
        bool nextAutoHeadlightSensitivity();

        // Decrease the auto healight sensitivity.
        bool prevAutoHeadlightSensitivity();

        // Increase the auto headlight off delay time.
        bool nextAutoHeadlightOffDelay();

        // Decrease the auto headlight off delay time.
        bool prevAutoHeadlightOffDelay();

        // Toggle the speed sensing wiper interval setting.
        bool toggleSpeedSensingWiperInterval();

        // Toggle the remote key response horn setting.
        bool toggleRemoteKeyResponseHorn();

        // Increase the remote key response light timeout.
        bool nextRemoteKeyResponseLights();

        // Decrease the remote key response light timeout.
        bool prevRemoteKeyResponseLights();

        // Increase the auto re-lock time.
        bool nextAutoReLockTime();

        // Decrease the auto re-lock time.
        bool prevAutoReLockTime();

        // Toggle the selective door unlock setting.
        bool toggleSelectiveDoorUnlock();

        // Toggle the "slide driver seat back on exit" setting.
        bool toggleSlideDriverSeatBackOnExit();

        // Reset all settings to factory default.
        bool resetSettingsToDefault();

    private:
        bool handleState(const Canny::Frame& frame);
        bool handleState05(const byte* data);
        bool handleState10(const byte* data);
        bool handleState21(const byte* data);
        bool handleState22(const byte* data);

        SettingsInit* initE_;
        SettingsRetrieve* retrieveE_;
        SettingsUpdate* updateE_;
        SettingsReset* resetE_;
        bool readyE() const;

        SettingsInit* initF_;
        SettingsRetrieve* retrieveF_;
        SettingsUpdate* updateF_;
        SettingsReset* resetF_;
        bool readyF() const;

        bool available_;
        Canny::Frame frame_;
        uint8_t state_[4];

        bool triggerAutoHeadlightSensitivity(uint8_t value);
        bool triggerRemoteKeyResponseLights(uint8_t value);

        bool setAutoInteriorIllumination(bool value);
        bool setAutoHeadlightSensitivity(uint8_t value);
        bool setAutoHeadlightOffDelay(AutoHeadlightOffDelay value);
        bool setSpeedSensingWiperInterval(bool value);
        bool setRemoteKeyResponseHorn(bool value);
        bool setRemoteKeyResponseLights(RemoteKeyResponseLights value);
        bool setAutoReLockTime(AutoReLockTime value);
        bool setSelectiveDoorUnlock(bool value);
        bool setSlideDriverSeatBackOnExit(bool value);
};

}  // namespace NissanR51

#endif  // _NISSAN_R51_SETTINGS_H_
