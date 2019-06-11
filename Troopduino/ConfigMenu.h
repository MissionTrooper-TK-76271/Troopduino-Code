/*
   buttons.h

   Created: April 3, 2019
   Author: Andrew Donatelli andrew@donatelli.net
   https://github.com/BipeFlyer/Troopduino-Code

   Portions of this code modified from FX-SaberOS by Sebastien CAPOU (neskweek@gmail.com) and Andras Kun (kun.andras@yahoo.de)

*/

#ifndef ConfigMenu_h
#define ConfigMenu_h_

#include <Arduino.h>
enum ModeEnum {Mode_Running, Mode_Sleep, Mode_Config, Mode_Chatter}; 
enum ConfigModeEnum {Config_VoiceChanger, Config_Bitcrusher, Config_Reverb, Config_MicSensitivity, Config_EffectsVolume, Config_SleepStart, Config_Last, Config_BatteryLevel}; 

void confParseValue(uint16_t variable, uint16_t min, uint16_t max, short int multiplier);

void NextConfigState();

#endif // ConfigMenu_h
