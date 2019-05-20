/*
   buttons.h

   Created: April 3, 2019
   Author: Andrew Donatelli andrew@donatelli.net
   https://github.com/BipeFlyer/Troopduino-Code

   Portions of this code modified from FX-SaberOS by Sebastien CAPOU (neskweek@gmail.com) and Andras Kun (kun.andras@yahoo.de)

*/


#include "ConfigMenu.h"
#include "Soundfont.h"
#include <DFPlayerMini_Fast.h>



// global Saber state and Sub State variables
ModeEnum CurrentMode;
ModeEnum PrevMode;

ConfigModeEnum ConfigMode;
ConfigModeEnum PrevConfigMode;

extern DFPlayerMini_Fast DFPlayer;

extern int8_t modification;
extern bool play;
extern int16_t value;


extern void soundPlay(uint8_t track, uint16_t delayTime);
extern void soundPlayLoop(uint16_t track);
extern SoundFont soundFont;
//extern DFPlayerMini_Fast DFPlayer;



// ====================================================================================
// ===           	  	 			CONFIG MODE FUNCTIONS	                		===
// ====================================================================================

// this function ensures that config menu items which have values between a min and a max value
// wrap back to min/max upon reaching max/min. It also plays a sound notifying the user if either min or max value has beeb reached.
// This function is also in charge of changing the actual value of a setting via the value global variable.
void confParseValue(uint16_t variable, uint16_t min, uint16_t max, short int multiplier) {

  value = variable + (multiplier * 1);


  if (value < (int) min) {
    value = max;
  } else if (value > (int) max) {
    value = min;
  } else if (value == (int) min and play) {
    play = false;
    soundPlay(soundFont.sfConfig(12), 200);
    delay(150);
  } else if (value == (int) max and play) {
    play = false;
    soundPlay(soundFont.sfConfig(12), 200);
    delay(150);
  }
#if defined debugMode
  Serial.print("value: ");
  Serial.println(value);
#endif
} //confParseValue

void NextConfigState() {

  ConfigMode = static_cast<ConfigModeEnum>(ConfigMode + 1); // change to next config state in the ordered list
  if (ConfigMode == Config_Last) {
    ConfigMode = static_cast<ConfigModeEnum>(0); // after the last config menu item go back to the first
  }
#if defined debugMode
  Serial.println(ConfigMode);
#endif


  switch (ConfigMode) {
    case Config_EffectsVolume:
#if defined debugMode
      Serial.print(F("Volume"));
#endif
      soundPlay(soundFont.sfConfig(6), 200);
      delay(500);
      break;
    case Config_VoiceChanger:
#if defined debugMode
      Serial.print(F("Voice Changer"));
#endif
      soundPlay(soundFont.sfConfig(4), 200);
      delay(500);
      break;
    case Config_Bitcrusher:
#if defined debugMode
      Serial.print(F("Bitcrusher"));
#endif
      soundPlay(soundFont.sfConfig(5), 200);
      delay(500);
      break;
    case Config_Reverb:
#if defined debugMode
      Serial.print(F("Reverb"));
#endif
      soundPlay(soundFont.sfConfig(7), 200);
      delay(500);
      break;

    case Config_MicSensitivity:
#if defined debugMode
      Serial.print(F("Mic Sensitivity"));
#endif
      soundPlay(soundFont.sfConfig(8), 200);
      delay(700);
      break;
    case Config_SleepStart:
#if defined debugMode
      Serial.print(F("Enter sleep mode"));
#endif
      soundPlay(soundFont.sfConfig(19), 200);
      delay(900);
      break;
    case Config_BatteryLevel:
#if defined debugMode
      Serial.print(F("Display battery level"));
#endif
      //  BatLevel_ConfigEnter();
      break;
  }
}
