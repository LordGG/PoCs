#define AUDIO_OUTPUT_PIN  13

int minRead = 1023;
int maxRead = 0;

void setup() {
  pinMode(AUDIO_OUTPUT_PIN, OUTPUT);
}

void loop() {
  int sensorValue = analogRead(A0);

  // calibrate min and max sensor values
  // ==> move throttle stick to min and max for calibration
  if (sensorValue < minRead){
    minRead = sensorValue;
  }
  if (sensorValue > maxRead){
    maxRead = sensorValue;
  }

  // calculate sensorValue for 3% of throttle stick
  int minThrottleValue = maxRead - ((maxRead - minRead)*3)/100;

  // higher sensor value ==> lower throttle value (so we reverse sensor range)
  int frequency = map(sensorValue, maxRead, minRead, 150, 700);

  // if throttle stick is lower than 3%, then no sound is played
  if (sensorValue > minThrottleValue){
    noTone(AUDIO_OUTPUT_PIN);
  }
  else {
    tone(AUDIO_OUTPUT_PIN, frequency);
  }
}
