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
        Settings(bool init = true, Faker::Clock* clock = Faker::Clock::real());

        // Handle BCM state frames 0x72E and 0x72F.
        void handle(const Message& msg) override;

        // Yield CAN frames to communicate with the vehicle or SETTINGS_STATE
        // events to indicate a change to the stored settings.
        void emit(const Caster::Yield<Message>& yield) override;

    private:
        void handleEvent(const SystemEvent& event);
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

        // Exchange init frames with BCM. 
        bool init();

        // Request the current settings from the BCM.
        bool requestCurrent();

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

        // Helper for triggering headlight sensitivity setting changes.
        bool triggerAutoHeadlightSensitivity(uint8_t value);

        // Helper for triggering remote key repsonse lights setting changes.
        bool triggerRemoteKeyResponseLights(uint8_t value);
};

}  // namespace R51

#endif  // _R51_VEHICLE_SETTINGS_H_
