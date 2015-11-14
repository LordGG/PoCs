#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVO_0_MIN  140 // (gauche PoV) this is the 'minimum' pulse length count (out of 4096)
#define SERVO_0_MAX  610 // (droite PoV) this is the 'maximum' pulse length count (out of 4096)
#define SERVO_0_CENTER 376

#define SERVO_1_MIN  135 // this is the 'minimum' pulse length count (out of 4096)
#define SERVO_1_MAX  515 // (bas) this is the 'maximum' pulse length count (out of 4096)
#define SERVO_1_CENTER 360

void setup() {
  Serial.begin(9600);
  Serial.println("16 channel Servo test!");

  pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}

void loop() {
  pwm.setPWM(0, 0, 376);
  pwm.setPWM(1, 0, 360);
  delay(2000);
  // Drive each servo one at a time
  for (uint16_t pulselen = SERVO_0_MIN; pulselen < SERVO_0_MAX; pulselen++) {
    pwm.setPWM(0, 0, pulselen);
    delay(10);
  }
  for (uint16_t pulselen = SERVO_0_MAX; pulselen > SERVO_0_MIN; pulselen--) {
    pwm.setPWM(0, 0, pulselen);
    delay(10);
  }
  delay(1000);
  pwm.setPWM(0, 0, 376);
  
  delay(2000);

  for (uint16_t pulselen = SERVO_1_MIN; pulselen < SERVO_1_MAX; pulselen++) {
    pwm.setPWM(1, 0, pulselen);
    delay(10);
  }
  for (uint16_t pulselen = SERVO_1_MAX; pulselen > SERVO_1_MIN; pulselen--) {
    pwm.setPWM(1, 0, pulselen);
    delay(10);
  }
  delay(1000);
  pwm.setPWM(1, 0, 360);
  delay(1000);
}
