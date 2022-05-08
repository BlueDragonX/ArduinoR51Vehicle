#ifndef _R51_VEHICLE_SETTINGS_H_
#define _R51_VEHICLE_SETTINGS_H_

#include <Arduino.h>
#include <Canny.h>
#include <Caster.h>
#include <Faker.h>
#include <R51Core.h>

namespace R51 {

class SettingsInit;
class SettingsRetrieve;
class SettingsUpdate;
class SettingsReset;

// Communicates with the BCM to retrieve and update body control settings.
class Settings : public Caster::Node<Message> {
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

        // Handle BCM state frames 0x72E and 0x72F.
        void handle(const Message& msg) override;

        // Yield CAN frames to communicate with the vehicle or SETTINGS_STATE
        // events to indicate a change to the stored settings.
        void emit(const Caster::Yield<Message>& yield) override;

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
        void handleFrame(const Canny::Frame& frame);
        void handleState(const byte* data);
        void handleState05(const byte* data);
        void handleState10(const byte* data);
        void handleState21(const byte* data);
        void handleState22(const byte* data);

        SettingsInit* initE_;
        SettingsRetrieve* retrieveE_;
        SettingsUpdate* updateE_;
        SettingsReset* resetE_;

        SettingsInit* initF_;
        SettingsRetrieve* retrieveF_;
        SettingsUpdate* updateF_;
        SettingsReset* resetF_;

        bool available_;
        Canny::Frame frame_;
        SystemEvent event_;

        bool readyE() const;
        bool readyF() const;
        bool ready() const;

        bool triggerAutoHeadlightSensitivity(uint8_t value);
        bool triggerRemoteKeyResponseLights(uint8_t value);

        void setAutoInteriorIllumination(bool value);
        void setAutoHeadlightSensitivity(uint8_t value);
        void setAutoHeadlightOffDelay(AutoHeadlightOffDelay value);
        void setSpeedSensingWiperInterval(bool value);
        void setRemoteKeyResponseHorn(bool value);
        void setRemoteKeyResponseLights(RemoteKeyResponseLights value);
        void setAutoReLockTime(AutoReLockTime value);
        void setSelectiveDoorUnlock(bool value);
        void setSlideDriverSeatBackOnExit(bool value);
};

}  // namespace R51

#endif  // _R51_VEHICLE_SETTINGS_H_
