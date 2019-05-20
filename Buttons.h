/*
   buttons.h

   Created: April 3, 2019
   Author: Andrew Donatelli andrew@donatelli.net
   https://github.com/BipeFlyer/Troopduino-Code

*/


#include <Arduino.h>
#ifndef Buttons_h
#define Buttons_h


// ***** button callback functions


void glove1_Click();
void glove1_Doubleclick();
void glove1_LongPressStart();
void glove1_LongPress();
void glove1_LongPressStop();

void glove2_Click();
void glove2_Doubleclick();
void glove2_LongPressStart();
void glove2_LongPress();
void glove2_LongPressStop();

void glove3_Click();
void glove3_Doubleclick();
void glove3_LongPressStart();
void glove3_LongPress();
void glove3_LongPressStop();

void glove4_Click();
void glove4_Doubleclick();
void glove4_LongPressStart();
void glove4_LongPress();
void glove4_LongPressStop();


#endif // Buttons_h
