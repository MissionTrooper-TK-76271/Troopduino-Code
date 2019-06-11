
/*
   buttons.cpp

   Created: April 3, 2019
   Author: Andrew Donatelli andrew@donatelli.net
   https://github.com/BipeFlyer/Troopduino-Code

*/


#include "Buttons.h"
#include "Soundfont.h"
#include "ConfigMenu.h"
#include <DFPlayerMini_Fast.h>


extern void soundPlay(uint8_t track, uint16_t delayTime);
extern void soundPlayLoop(uint16_t track);

extern void saveConfig();
extern void savePTTMode();

extern SoundFont soundFont;
extern DFPlayerMini_Fast DFPlayer;

extern bool PTT;
extern bool PTTButtonPressed;
extern bool VoiceEffect;
extern bool SleepMode;
extern bool ChatterMode;
extern uint8_t EffectsVolume;
extern uint8_t Bitcrusher;
extern uint8_t Reverb;
extern uint8_t MicSensitivity;
extern bool VolumeSet;
extern int16_t value;
extern bool play;
extern bool BlasterMode;

extern ModeEnum CurrentMode;
extern ModeEnum PrevMode;
extern ConfigModeEnum ConfigMode;
extern ConfigModeEnum PrevConfigMode;
extern void NextConfigState();
extern void buttonSetClickTicks(uint8_t numButton, uint8_t clickTicks);



//********** GLOVE BUTTON 1
void glove1_Click() {

  if (CurrentMode == Mode_Running) {
    soundPlay(soundFont.sfGlove(1), 1500);
  }
  if (CurrentMode == Mode_Sleep) {
    void Sleep(bool StartSleep);
    Sleep(false);
  }

  if (CurrentMode == Mode_Config) {
    switch (ConfigMode) {
      case Config_EffectsVolume:
        play = true;
        confParseValue(EffectsVolume, 15, 30, 1);
        EffectsVolume = value;
        DFPlayer.volume(EffectsVolume);
        delay(20);

        soundPlay(soundFont.sfConfig(10), 100);
        break;
      case Config_VoiceChanger:
        confParseValue(VoiceEffect, 0, 1, 1);
        VoiceEffect = value;
        if (VoiceEffect == 1) {
          soundPlay(soundFont.sfConfig(13), 200);
        }
        else {
          soundPlay(soundFont.sfConfig(14), 200);
        }

        break;
      case Config_Bitcrusher:
        play = true;
        confParseValue(Bitcrusher, 1, 100, 1);
        Bitcrusher = value;
        soundPlay(soundFont.sfConfig(10), 100);
        break;
      case Config_Reverb:
        play = true;
        confParseValue(Reverb, 40, 220, 1);
        Reverb = value;
        soundPlay(soundFont.sfConfig(10), 100);
        break;
      case Config_MicSensitivity:
        play = true;
        confParseValue(MicSensitivity, 5, 98, 1);
        MicSensitivity = value;
        soundPlay(soundFont.sfConfig(10), 100);
        break;
      case Config_SleepStart:
        void Sleep(bool StartSleep);
        Sleep(true);
        break;
      case Config_BatteryLevel:
        //  BatLevel_ConfigEnter();
        break;

    }
  }

}

void glove1_Doubleclick() {
  Serial.println("1doubleclick");
  if (CurrentMode == Mode_Config) {
    void glove_MultiClick(uint8_t button, uint8_t clicks);
    glove_MultiClick(1, 5);
  }

  else {
    PTT = !PTT;
    savePTTMode();
    if (PTT) {
      soundPlay(soundFont.sfConfig(2), 1000);
    }
    else
    {
      soundPlay(soundFont.sfConfig(3), 1100);
    }
  }

}

void glove1_LongPressStart() {
  if (CurrentMode == Mode_Config) {
    NextConfigState();
  }
  if (CurrentMode == Mode_Running) {
    if (PTT) {
      PTTButtonPressed = true;
      soundPlay(soundFont.sfClick(), 200);
      soundPlay(soundFont.sfStatic(), 200);
    }
  }
}

void glove1_LongPress() {
}

void glove1_LongPressStop() {
  if (CurrentMode == Mode_Running) {
    PTTButtonPressed = false;
    if (PTT) {
      soundPlay(soundFont.sfClick(), 400);
    }
  }
}




//********** GLOVE BUTTON 2
void glove2_Click() {
  if (CurrentMode == Mode_Running) {
    soundPlay(soundFont.sfGlove(2), 1500);
  }
  if (CurrentMode == Mode_Config) {
    switch (ConfigMode) {
      case Config_EffectsVolume:
        play = true;
        confParseValue(EffectsVolume, 15, 30, -1);
        EffectsVolume = value;
        DFPlayer.volume(EffectsVolume);
        delay(20);
        soundPlay(soundFont.sfConfig(11), 100);
        break;
      case Config_VoiceChanger:
        confParseValue(VoiceEffect, 0, 1, -1);
        VoiceEffect = value;
        if (VoiceEffect == 1) {
          soundPlay(soundFont.sfConfig(13), 0);
        }
        else {
          soundPlay(soundFont.sfConfig(14), 0);

        }
        break;
      case Config_Bitcrusher:
        play = true;
        confParseValue(Bitcrusher, 1, 100, -1);
        Bitcrusher = value;
        soundPlay(soundFont.sfConfig(11), 100);
        break;
      case Config_Reverb:
        play = true;
        confParseValue(Reverb, 40, 220, -1);
        Reverb = value;
        soundPlay(soundFont.sfConfig(11), 100);
        break;
      case Config_MicSensitivity:
        play = true;
        confParseValue(MicSensitivity, 5, 98, -1);
        MicSensitivity = value;
        soundPlay(soundFont.sfConfig(11), 100);
        break;
      case Config_SleepStart:
        void Sleep(bool StartSleep);
        Sleep(true);
        break;
      case Config_BatteryLevel:
        //  BatLevel_ConfigEnter();
        break;

    }
  }


}

void glove2_Doubleclick() {
  if (CurrentMode == Mode_Config) {
    void glove_MultiClick(uint8_t button, uint8_t clicks);
    glove_MultiClick(2, 5);
  }



}

void glove2_LongPressStart() {

  if (CurrentMode == Mode_Config) {
#if defined debugMode
    Serial.println(F("Exit config mode"));
#endif
    saveConfig();
    soundPlay(soundFont.sfConfig(9), 400);
    delay(800);
    soundPlay(soundFont.sfConfig(18), 400);
    delay(500);

    CurrentMode = Mode_Running;
  }
  else if (CurrentMode == Mode_Running) {
#if defined debugMode
    Serial.println(F("Enter config mode"));
#endif
    PTT = false; // turn off PTT so we can test in config mode
    CurrentMode = Mode_Config;
    soundPlay(soundFont.sfConfig(9), 400);
    delay(1000);
    ConfigMode = static_cast<ConfigModeEnum>(-1);
    NextConfigState();

  }


}

void glove2_LongPress() {
}

void glove2_LongPressStop() {
}


//********** GLOVE BUTTON 3
void glove3_Click() {
  soundPlay(soundFont.sfGlove(3), 1500);
}

void glove3_Doubleclick() {



}


void glove3_LongPressStart() {// reset configuration

  ChatterMode = !ChatterMode;
  if (ChatterMode) {
    //soundPlayLoop(soundFont.sfLoop(1));
    soundPlay(soundFont.sfLoop(1), 1000);
    CurrentMode = Mode_Chatter;
  }
  else
  {
    soundPlay(soundFont.sfConfig(11), 140);
    soundPlay(soundFont.sfConfig(11), 140);
    CurrentMode = Mode_Running;
  }

}

void glove3_LongPress() {
}

void glove3_LongPressStop() {
}


//********** GLOVE BUTTON 4
void glove4_Click() {

  soundPlay(soundFont.sfGlove(3), 10);

}

void glove4_Doubleclick() {

}

void glove4_LongPressStart() {
  buttonSetClickTicks(4, 300); // Set delay back for button 4
  soundPlay(soundFont.sfGlove(6), 1126);

}

void glove4_LongPress() {

}

void glove4_LongPressStop() {
}

void glove_MultiClick(uint8_t button, uint8_t clicks) {
  if (ConfigMode == Config_Bitcrusher or ConfigMode == Config_Reverb or ConfigMode == Config_MicSensitivity) {
    for (int i = 1; i <= clicks; i++) {
      switch (button) {
        case 1:
          glove1_Click();
          break;
        case 2:
          glove2_Click();
          break;
        case 3:
          glove3_Click();
          break;
        case 4:
          glove4_Click();
          break;
      }

    }
  }
}

void Sleep(bool StartSleep) {

  if (StartSleep) {
#if defined debugMode
    Serial.println(F("Enter sleep mode"));
#endif
    CurrentMode = Mode_Sleep;
    soundPlay(soundFont.sfConfig(10), 500);

    soundPlay(soundFont.sfConfig(10), 500);

    soundPlay(soundFont.sfConfig(10), 500);

    DFPlayer.sleep();

  }
  else {
#if defined debugMode
    Serial.println(F("Exit sleep mode"));
#endif
    DFPlayer.wakeUp();
    delay(100);
    soundPlay(soundFont.sfBoot(), 0);

    CurrentMode = Mode_Running;
  }
}
