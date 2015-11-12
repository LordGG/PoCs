int ledsPins[] = {7, 8, 9, 10, 11}; // bargraphe
int buzzerPin = 6;
int powerLedPin = 5;
int button1Pin = 3;
int button2Pin = 4;

int button1State = 0;
int button2State = 0;

int noteFreqArr[] = {
49.4, 52.3, 55.4, 58.7, 62.2, 65.9, 69.9, 74, 78.4, 83.1, 88, 93.2, 
98.8, 105, 111, 117, 124, 132, 140, 148, 157, 166, 176, 186, 
198, 209, 222, 235, 249, 264, 279, 296, 314, 332, 352, 373, 
395, 419, 444, 470, 498, 527, 559, 592, 627, 665, 704, 746, 
790, 837, 887, 940, 996, 1050, 1110, 1180, 1250, 1320, 1400, 1490, 
1580, 1670, 1770, 1870, 1990, 2100};

void setup() {
  // assignement du mode des pins digitaux
  // pour les pins des leds du bargraphe
  for (int i = 0; i < (sizeof(ledsPins)/sizeof(int)); i++){
    pinMode(ledsPins[i], OUTPUT);
  }

  // pour les autres pins
  pinMode(buzzerPin, OUTPUT);
  pinMode(powerLedPin, OUTPUT);
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);

  AllLedsOn();
  delay(500);
  AllLedsOff();
  delay(500);
  AllLedsOn();
  delay(500);
  AllLedsOff();
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  // on recupere l etat des 2 boutons
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);

  // si bouton 1 ON, on joue une anim sur le bargraphe
  if (button1State == HIGH)
  {
    animBargraph();
  }
  else
  {
    // si bouton 2 ON, ou joue le french cancan avec le buzzer
    if (button2State == HIGH)
    {
      playFrenchCanCan();
    }
  }
  digitalWrite(powerLedPin, HIGH);
  delay(100);
  digitalWrite(powerLedPin, LOW);
  delay(100);
}

void animBargraph()
{
  for (int i = 0; i < (sizeof(ledsPins)/sizeof(int)); i++)
  {
    digitalWrite(ledsPins[i], HIGH);
    delay(50);
  }
  for (int i = 0; i < (sizeof(ledsPins)/sizeof(int)); i++)
  {
    digitalWrite(ledsPins[i], LOW);
    delay(50);
  }
}

// allume toutes les leds du bargraphe
void AllLedsOn()
{
  for (int i = 0; i < (sizeof(ledsPins)/sizeof(int)); i++)
  {
    digitalWrite(ledsPins[i], HIGH);
  }
}

// eteint toutes les leds du bargraphe
void AllLedsOff()
{
  for (int i = 0; i < (sizeof(ledsPins)/sizeof(int)); i++)
  {
    digitalWrite(ledsPins[i], LOW);
  }
}

// Buzzer example function for the CEM-1203 buzzer (Sparkfun's part #COM-07950).
// by Rob Faludi
// http://www.faludi.com
void playNote(int noteInt, long length, long breath = 20) {
  length = length - breath;
  buzz(buzzerPin, noteFreqArr[noteInt], length);
  if(breath > 0) { //take a short pause or 'breath' if specified
    delay(breath);
  }
}

void buzz(int targetPin, long frequency, long length) {
  long delayValue = 1000000/frequency/2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length/ 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to 
  //// get the total number of cycles to produce
  for (long i=0; i < numCycles; i++){ // for the calculated length of time...
    digitalWrite(targetPin,HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin,LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait againf or the calculated delay value
  }
}

void playFrenchCanCan()
{
  playNote(24,500);  
  playNote(17,1000);
  playNote(19,250);
  playNote(22,250);
  playNote(21,250);
  playNote(19,250);
  playNote(24,500);
  playNote(24,500);
  playNote(24,250);
  playNote(26,250);
  playNote(21,250);
  playNote(22,250);
  playNote(19,500);
  playNote(19,500);
  playNote(19,250);
  playNote(22,250);
  playNote(21,250);
  playNote(19,250);
  playNote(17,250);
  playNote(29,250);
  playNote(28,250);
  playNote(26,250);
  playNote(24,250);
  playNote(22,250);
  playNote(21,250);
  playNote(19,250);  
  playNote(17,1000);
  playNote(19,250);
  playNote(22,250);
  playNote(21,250);
  playNote(19,250);
  playNote(24,500);
  playNote(24,500);
  playNote(24,250);
  playNote(26,250);
  playNote(21,250);
  playNote(22,250);
  playNote(19,500);
  playNote(19,500);
  playNote(19,250);
  playNote(22,250);
  playNote(21,250);
  playNote(19,250);
  playNote(17,250);
  playNote(24,250);
  playNote(19,250);
  playNote(21,250);
  playNote(17,250);
}
