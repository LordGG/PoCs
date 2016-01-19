#include <Servo.h>
#include <NewPing.h>

#define TRIGGER_PIN  4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     6  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 20 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define BUZZER_PIN 8 // Arduino pin tied to buzzer
#define SERVO_PIN 2 // Arduino pin tied to servo

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
Servo myservo;

int pos = 0;    // variable to store the servo position
int distance = 0;
int previousDistance = 0;

int noteFreqArr[] = {
  // do   do#     ré      mib     mi      fa      fa#     sol     sol#    la    sib     si
  //0     1       2       3       4       5       6       7       8       9     10      11
  32.7,   34.65,  36.71,  38.89,  41.2,   43.65,  46.25,  49,     51.91,  55,   58.27,  61.74,
  //12    13      14      15      16      17      18      19      20      21    22      23
  65.41,  69.3,   73.42,  77.78,  82.41,  87.31,  92.5,   98,     103.83, 110,  116.54, 123.47,
  //24    25      26      27      28      29      30      31      32      33    34      35
  130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185,    196,    207.65, 220,  233.08, 246.94,
  //36    37      38      39      40      41      42      43      44      45    46      47
  261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392,    415.3,  440,  466.16, 493.88,
  //48    49      50      51      52      53      54      55      56      57    58      59
  523.25, 554.37, 587.33, 622.25, 659.26, 698.46, 739.99, 783.99, 830.61, 880,  932.33, 987.77
};

void setup() {
  pinMode(BUZZER_PIN, OUTPUT); // buzzer
  myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object
  myservo.write(90); // centrage du servo
  playSound();
}

void loop() {
  // put your main code here, to run repeatedly:
  previousDistance = distance;
  distance = sonar.ping_cm(); // Send ping, get distance in cm and print result (0 = outside set distance range)
  if (distance > 0 && distance <= 25 && previousDistance != 0)
  {
    // si distance inférieure a 5 cm, BB8 secoue la tete
    if (distance <= 5)
    {
      moveServo3();
      playSoundShort();
    }
    // entre 5 et 12cm
    else if (distance <= 12)
    {
      playSound2();
      moveServo2();
      playSoundShort();      
    }
    // entre 12 et 25 cm
    else
    {
      playSound();
      moveServo();
      delay(200);
      playSoundShort();
    }
  }
  delay(35);
}

void playNote(int noteInt, long length, long breath = 20) {
  length = length - breath;
  buzz(BUZZER_PIN, noteFreqArr[noteInt], length);
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

void playSound()
{
  playNote(24, 200);
  playNote(29, 100);
  playNote(28, 100);
  playNote(26, 100);
  playNote(43, 100);
  playNote(48, 200);
}

void playSound2()
{
  playNote(48, 200);
  playNote(32, 100);
  playNote(44, 100);
  playNote(29, 100);
  playNote(46, 100);
  playNote(24, 200);
}

void playSoundShort()
{
  playNote(55, 100);
  playNote(48, 100);
}

void moveServo()
{
  myservo.write(90);
  
  for(pos = 90; pos <= 170; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(5);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 170; pos>=15; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(5);                       // waits 15ms for the servo to reach the position 
  } 

  for(pos = 15; pos <= 90; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(5);                       // waits 15ms for the servo to reach the position 
  } 
  
  myservo.write(90);
}

void moveServo2()
{
  myservo.write(90);
  delay(50);
  playNote(48, 100);
  myservo.write(40);
  delay(300);
  playNote(39, 100);
  myservo.write(70);
  delay(300);
  playNote(46, 100);
  myservo.write(15);
  delay(300);
  playNote(24, 100);
  myservo.write(165);
  delay(300);
  playNote(32, 100);

  for(pos = 165; pos>=90; pos-=1)     // goes from 165 degrees to 90 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(5);                       // waits 15ms for the servo to reach the position 
  } 
  myservo.write(90);
}

void moveServo3()
{
  myservo.write(90);
  delay(50);
  
  for(int i = 0; i <= 3; i++)
  {
    myservo.write(75);
    delay(100);
    playNote(40, 100);
    myservo.write(105);
    delay(100);
    playNote(48, 100);
  }

  myservo.write(90);
}
