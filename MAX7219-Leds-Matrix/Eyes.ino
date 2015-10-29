#include "LedControl.h"
 
LedControl lc=LedControl(12,11,10,2);  // Pins: DIN,CLK,CS, # of Display connected

// Put values in arrays
byte eyeOff[] =
{
   B00000000,
   B00000000,
   B00000000,
   B00000000,
   B00000000,
   B00000000,
   B00000000,
   B00000000
};

byte eyeOpened[] =
{
   B00111100,
   B01100110,
   B01000010,
   B11011011,
   B11011011,
   B01000010,
   B01100110,
   B00111100
};
 
byte eyeClosed[] =
{
  B00011000,
  B00011000,
  B00111100,
  B00111100,
  B00111100,
  B00111100,
  B00011000,
  B00011000
};
 
byte eyeToLeft[] =
{
   B00111100,
   B01100110,
   B01000010,
   B11000011,
   B11011011,
   B01011010,
   B01100110,
   B00111100
};
 
byte eyeToRight[] =
{
   B00111100,
   B01100110,
   B01011010,
   B11011011,
   B11000011,
   B01000010,
   B01100110,
   B00111100
};

byte love[] =
{
   B00001100,
   B00011110,
   B00111110,
   B01111100,
   B01111100,
   B00111110,
   B00011110,
   B00001100
};

byte happy[] =
{
   B00111100,
   B01000010,
   B10010101,
   B10100001,
   B10100001,
   B10010101,
   B01000010,
   B00111100
};

byte eye2Opened[] =
{
   B00111100,
   B01111110,
   B11111111,
   B11100111,
   B11100111,
   B11111111,
   B01111110,
   B00111100
};
 
byte eye2Closed[] =
{
  B00011000,
  B00011000,
  B00111100,
  B00111100,
  B00111100,
  B00111100,
  B00011000,
  B00011000
};
 
byte eye2ToUpperLeft[] =
{
   B00111100,
   B01111110,
   B11111111,
   B11111111,
   B11111111,
   B11110011,
   B01110010,
   B00111100
};
 
byte eye2ToRight[] =
{
   B00111100,
   B01100110,
   B11100111,
   B11111111,
   B11111111,
   B11111111,
   B01111110,
   B00111100
};
 
void setup()
{
  lc.shutdown(0,false);  // Wake up displays
  lc.shutdown(1,false);
  lc.setIntensity(0,1);  // Set intensity levels
  lc.setIntensity(1,1);
  lc.clearDisplay(0);  // Clear Displays
  lc.clearDisplay(1);
}
 
void SetLeftEye(byte* mood)
{
  for (int i = 0; i < 8; i++)  
  {
    lc.setRow(1,i,mood[i]);
  }
}

void SetRightEye(byte* mood)
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,mood[i]);
  }
}
 
void loop()
{
    SetLeftEye(eyeOpened);
    SetRightEye(eyeOpened);
    delay(2000);
    SetLeftEye(eyeClosed);
    SetRightEye(eyeClosed);
    delay(400);
    SetLeftEye(eyeOpened);
    SetRightEye(eyeOpened);
    delay(2000); 
    SetLeftEye(eyeToLeft);
    SetRightEye(eyeToLeft);
    delay(1000);
    SetLeftEye(eyeOpened);
    SetRightEye(eyeOpened);
    delay(1000);
    SetLeftEye(eyeToRight);
    SetRightEye(eyeToRight);
    delay(1000);
    SetLeftEye(eyeOpened);
    SetRightEye(eyeOpened);
    delay(1000);
    SetRightEye(eyeClosed);
    delay(200); 
    SetLeftEye(eyeOpened);
    SetRightEye(eyeOpened);
    delay(1000);

    SetLeftEye(eyeOff);
    SetRightEye(eyeOff);
    delay(1000);

    SetLeftEye(love);
    SetRightEye(love);
    delay(2000);
    SetLeftEye(happy);
    SetRightEye(happy);
    delay(2000);

    SetLeftEye(eye2Opened);
    SetRightEye(eye2Opened);
    delay(2000);
    SetLeftEye(eye2Closed);
    SetRightEye(eye2Closed);
    delay(400);
    SetLeftEye(eye2Opened);
    SetRightEye(eye2Opened);
    delay(2000); 
    SetLeftEye(eye2ToUpperLeft);
    SetRightEye(eye2ToUpperLeft);
    delay(1000);
    SetLeftEye(eye2Opened);
    SetRightEye(eye2Opened);
    delay(1000);
    SetLeftEye(eye2ToRight);
    SetRightEye(eye2ToRight);
    delay(1000);
    SetLeftEye(eye2Opened);
    SetRightEye(eye2Opened);
    delay(1000);
    SetRightEye(eye2Closed);
    delay(200); 
    SetLeftEye(eye2Opened);
    SetRightEye(eye2Opened);
    delay(1000);

    SetLeftEye(eyeOff);
    SetRightEye(eyeOff);
    delay(1000);
}
