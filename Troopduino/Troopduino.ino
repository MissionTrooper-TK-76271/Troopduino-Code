/*
   Troopduino Voice Changer

   Created: April 3, 2019
   Author: Andrew Donatelli andrew@donatelli.net
   https://github.com/BipeFlyer/Troopduino-Code

   Portions of this code modified from
   KHM 2008 / Lab3/  Martin Nawrath nawrath@khm.de
   Kunsthochschule fuer Medien Koeln
   Academy of Media Arts Cologne

*/



#define debugMode



#include <Arduino.h>
#include <DFPlayerMini_Fast.h>
#include <SoftwareSerial.h>
#include <OneButton.h>
#include <EEPROM.h>


#include "Soundfont.h"
#include "Buttons.h"
#include "ConfigMenu.h"

// *********** Mode Variables

extern ModeEnum CurrentMode;
extern ModeEnum PrevMode;
extern ConfigModeEnum ConfigMode;
extern ConfigModeEnum PrevConfigMode;

int16_t value = 0;
bool play = false;

// ********************


//************** Begin ADC


#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))


int ledPin = 13;                 // LED connected to digital pin 13



// interrupt variables accessed globally
volatile boolean f_sample;

volatile uint8_t badc1;


uint16_t iBCCnt = 0;
int iw1;
bool bTalking = false;

int icnt;
int iw;
int soundOutput;
uint8_t dd[512];  // Audio Memory Array 8-Bit



// ********* main variables
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long clickAfterTalk = 500;
bool PTT = false; //Push to Talk on or off
bool PTTButtonPressed = false; //Push to Talk currently pressed
bool VoiceActivated = false; //Voice detected
bool VoiceEffect = true; //Enable or disable SoundEffect
bool SleepMode = false; //Enable or disable Sleep Mode
bool ChatterMode = false; //Enable or disable Chatter Mode
bool BlasterMode = false; //Enable or disable Blaster Mode
uint8_t EffectsVolume = 30;
uint8_t Bitcrusher = 1;
uint8_t Reverb = 40;
uint8_t MicSensitivity = 50;
bool VolumeSet = false;
//***********

//*********** EEPROM Constants
const uint8_t eePTTMode = 0;
const uint8_t eeVoiceChangerMode = 1;
const uint8_t eeEffectsVolume = 2;
const uint8_t eeBitcrusher = 3;
const uint8_t eeMicSensitivity = 4;
const uint8_t eeReverb = 5;
//***********


OneButton gloveButton1(5, true);
OneButton gloveButton2(6, true);
OneButton gloveButton3(7, true);
OneButton gloveButton4(8, true);



DFPlayerMini_Fast DFPlayer;


SoundFont soundFont;


SoftwareSerial DFPSerial(9, 10); // RX, TX




//***************************  SETUP
void setup()
{

  startMillis = millis();  //initial start time
  randomSeed(analogRead(0));
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
  Serial.begin(57600);        // connect to the serial port



  // set adc prescaler to 32 for 38 kHz sampling frequency
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);






  sbi(ADMUX, ADLAR); // 8-Bit ADC in ADCH Register
  sbi(ADMUX, REFS0); // VCC Reference
  cbi(ADMUX, REFS1);
  cbi(ADMUX, MUX0);  // Set Input Multiplexer to Channel 0



  // Timer2 PWM Mode set to fast PWM
  cbi (TCCR2A, COM2A0); //D11
  sbi (TCCR2A, COM2A1); //D11
  sbi (TCCR2A, WGM20);
  sbi (TCCR2A, WGM21);

  cbi (TCCR2B, WGM22);

  cbi (TCCR2A, COM2B0); //D3
  sbi (TCCR2A, COM2B1); //D3




  // Timer2 Clock Prescaler to : 1
  sbi (TCCR2B, CS20);
  cbi (TCCR2B, CS21);
  cbi (TCCR2B, CS22);


  // Timer2 PWM Port Enable
  pinMode(11, OUTPUT);
  pinMode(3, OUTPUT);


  sbi (TIMSK2, TOIE2);             // enable Timer2 Interrupt


  Serial.print("ADC offset=");     // MAX4466 mic should be around 96
  iw1 = badc1;
  Serial.println(iw1);


  loadConfig();
  // ***** DFPlayer

  DFPSerial.begin(9600);
  DFPlayer.begin(DFPSerial);
  DFPlayer.wakeUp();
  DFPlayer.volume(EffectsVolume);
  delay(20);
  soundPlay(soundFont.sfBoot(), 800);




  // ***** end DFPlayer



  // ***** Buttons

  gloveButton1.setClickTicks(300);
  gloveButton1.setPressTicks(600);
  gloveButton1.attachClick(glove1_Click);
  gloveButton1.attachDoubleClick(glove1_Doubleclick);
  gloveButton1.attachLongPressStart(glove1_LongPressStart);
  gloveButton1.attachLongPressStop(glove1_LongPressStop);
  gloveButton1.attachDuringLongPress(glove1_LongPress);

  gloveButton2.setClickTicks(300);
  gloveButton2.setPressTicks(600);
  gloveButton2.attachClick(glove2_Click);
  gloveButton2.attachDoubleClick(glove2_Doubleclick);
  gloveButton2.attachLongPressStart(glove2_LongPressStart);
  gloveButton2.attachLongPressStop(glove2_LongPressStop);
  gloveButton2.attachDuringLongPress(glove2_LongPress);

  gloveButton3.setClickTicks(300);
  gloveButton3.setPressTicks(600);
  gloveButton3.attachClick(glove3_Click);
  gloveButton3.attachDoubleClick(glove3_Doubleclick);
  gloveButton3.attachLongPressStart(glove3_LongPressStart);
  gloveButton3.attachLongPressStop(glove3_LongPressStop);
  gloveButton3.attachDuringLongPress(glove3_LongPress);

  gloveButton4.setClickTicks(300);
  gloveButton4.setPressTicks(600);
  gloveButton4.setDebounceTicks(3);
  gloveButton4.attachClick(glove4_Click);
  gloveButton4.attachDoubleClick(glove4_Doubleclick);
  gloveButton4.attachLongPressStart(glove4_LongPressStart);
  gloveButton4.attachLongPressStop(glove4_LongPressStop);
  gloveButton4.attachDuringLongPress(glove4_LongPress);

  // ***** end Buttons

  // **** Read Config Values



 


} // **** Setup



void loop()
{
 
  gloveButton1.tick();
  gloveButton2.tick();
  gloveButton3.tick();
  gloveButton4.tick();

  if (CurrentMode != Mode_Sleep) {
    while (!f_sample) {     // wait for Sample Value from ADC
    }                       // Cycle 15625 KHz = 64uSec


   // f_sample = false;
    //****** begin reverb
    if (Reverb > 40) {
      soundOutput = dd[icnt] ;            // read the delay buffer
      iw = 96 - soundOutput ;            // substract offset
      iw = iw * Reverb / 255;     // scale delayed sample

      iw1 = 96 - badc1;          // substract offset from new sample
      iw1 = iw1 + iw;             // add delayed sample and new sample
      if (iw1 < -96) iw1 = -96; // Audio limiter
      if (iw1 > 96) iw1 = 96;   // Audio limiter

      soundOutput = 127 + iw1;             // add offset
      dd[icnt] = soundOutput;              // store sample in audio buffer

      icnt++;
      icnt = icnt & 511;         // limit bufferindex 0..511

    }
    else
    {
      soundOutput = ADCH;
    }
    //****** end reverb


    if ((badc1 < 96 - MicSensitivity or badc1 > 96 + MicSensitivity) and !PTT) { //if threshold is met then start to allow audio through
      //Serial.println(badc1);
      VoiceActivated = true;
    }
    else
    {
      VoiceActivated = false;
    }


    if (VoiceActivated or PTTButtonPressed) {
      if (!bTalking) {
        soundPlay(soundFont.sfStatic(), 0);
      }

      startMillis = currentMillis;

      bTalking = true;

    }
    else {
      currentMillis = millis();


      if ((currentMillis - startMillis >= clickAfterTalk) and bTalking) {//if audio is below threshold for too long, turn off audio and play click

        bTalking = false;
        startMillis = currentMillis;
        if ( !PTT) {
          soundPlay(soundFont.sfClick(), 400);
        }

      }
    }


    // If talking play audio
    if (bTalking) {
      if (VoiceEffect) { //voice effect filter

        OCR2B = soundOutput;
      }
      else //Unfiltered voice
      {
        OCR2A = soundOutput;
      }

    }





  }


} // ***********  end loop

void soundPlay(uint8_t track, uint16_t delayTime) {
 // if (!ChatterMode) {
    DFPlayer.play(track);
    delay(delayTime);
//  }
}

void soundPlay(uint8_t = 0, int = 0);


void soundPlayLoop(uint16_t track) {
  DFPlayer.loop(track);
}


void loadConfig() {

  // *******  EEPROM Section
  uint8_t ConfigRead;
  ConfigRead = EEPROM.read(eePTTMode);
  if (ConfigRead != 255) {
    PTT = ConfigRead;
  }
#if defined debugMode
  Serial.print("PTT Mode: ");
  Serial.println(ConfigRead);
#endif

  ConfigRead = EEPROM.read(eeVoiceChangerMode);
  if (ConfigRead != 255) {
    VoiceEffect = ConfigRead;
  }
#if defined debugMode
  Serial.print("Voice Changer Mode: ");
  Serial.println(ConfigRead);
#endif


  ConfigRead = EEPROM.read(eeEffectsVolume);
  if (ConfigRead != 255) {
    EffectsVolume = ConfigRead;
  }
#if defined debugMode
  Serial.print("Effects Volume: ");
  Serial.println(ConfigRead);
#endif



  ConfigRead = EEPROM.read(eeBitcrusher);
  if (ConfigRead != 255) {
    Bitcrusher = ConfigRead;
  }
#if defined debugMode
  Serial.print("Bitcrusher: ");
  Serial.println(ConfigRead);
#endif


  ConfigRead = EEPROM.read(eeMicSensitivity);
  if (ConfigRead != 255) {
    MicSensitivity = ConfigRead;
  }
#if defined debugMode
  Serial.print("Mic Sensitivity: ");
  Serial.println(ConfigRead);
#endif

  ConfigRead = EEPROM.read(eeReverb);
  if (ConfigRead != 255) {
    Reverb = ConfigRead;
    if (ConfigRead < 10) {
      Reverb = 0;
    }

  }
#if defined debugMode
  Serial.print("Reverb: ");
  Serial.println(ConfigRead);
#endif
}


void saveConfig() {
  EEPROM.update(eeVoiceChangerMode, VoiceEffect);
  EEPROM.update(eeEffectsVolume, EffectsVolume);
  EEPROM.update(eeBitcrusher, Bitcrusher);
  EEPROM.update(eeMicSensitivity, MicSensitivity);
  EEPROM.update(eeReverb, Reverb);

}
void savePTTMode() {
  EEPROM.update(eePTTMode, PTT);
#if defined debugMode
  Serial.print("PTT Mode: ");
  Serial.println(PTT);
#endif
}

// *******  END EEPROM Section


void buttonSetClickTicks(uint8_t numButton, uint8_t clickTicks) {
  switch (numButton) {
    case 1:
      gloveButton1.setClickTicks(clickTicks);
      break;
    case 2:
      gloveButton2.setClickTicks(clickTicks);
      break;
    case 3:
      gloveButton3.setClickTicks(clickTicks);
      break;
    case 4:
      gloveButton4.setClickTicks(clickTicks);
      break;
  }


}

//******************************************************************
// Timer2 Interrupt Service at 62.5 KHz
ISR(TIMER2_OVF_vect) {

  PORTB = PORTB  | 1 ;


  cbi(ADMUX, MUX0); //A0
  badc1 = ADCH;
  iBCCnt++;

  if (iBCCnt == Bitcrusher) {

    iBCCnt = 0;
    f_sample = true;
  }

  sbi(ADCSRA, ADSC);             // start next conversion


}
