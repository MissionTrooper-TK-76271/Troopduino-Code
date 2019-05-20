
/*
   soundfont.h

   Created: April 3, 2019
   Author: Andrew Donatelli andrew@donatelli.net
   https://github.com/BipeFlyer/Troopduino-Code

*/


#define numConfigSounds 19
#define numClicks 12
#define numStatic 1
#define numGlove 6
#define numLoops 1

#define Click_Offset numConfigSounds + 1
#define Static_Offset Click_Offset + numClicks 
#define Glove_Offset Static_Offset + numStatic 
#define Loop_Offset Glove_Offset + numGlove 

#ifndef Soundfont_h
#define Soundfont_h

class SoundFont {


  public:


    const uint8_t sfBoot(uint8_t offset = 0) {
      return 1;
    }

    const uint8_t sfConfig(uint8_t offset = 0) {
      return offset;
    }

    const uint8_t sfClick() {
      return Click_Offset + random(0, numClicks);
    }

    const uint8_t sfStatic() {
      return Static_Offset + random(0, numStatic);
    }

    const uint8_t sfGlove(uint8_t offset = 1) {
      return Glove_Offset + offset -1;
    }

    const uint8_t sfLoop(uint8_t offset = 1) {
      return Loop_Offset + offset -1;
    }

  private:

};

#endif //Soundfont_h
