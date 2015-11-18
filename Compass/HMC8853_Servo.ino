// adaptation of Adafruit sample of code
// https://github.com/adafruit/Adafruit_HMC5883_Unified/blob/master/examples/magsensor/magsensor.ino

#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
Servo myservo;
int servoPos = 85; //
int compassCorrection = 0; // set to 0 (not used)

void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");  
  Serial.println("------------------------------------");
  Serial.println("");
  myservo.attach(9);
  delay(500);
}

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("HMC5883 Magnetometer Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
  myservo.write(85);
  delay(1000);
}

void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);
 
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
//  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
//  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
//  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");

  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  //float declinationAngle = 0.22;
  //heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 
  
  int headingDegreesInteger = headingDegrees;
  // not used here, since correction is set to 0
  if ((headingDegreesInteger - compassCorrection) >= 0)
  {
    headingDegreesInteger = headingDegreesInteger  - compassCorrection;
  }
  else
  {
    headingDegreesInteger = headingDegreesInteger - compassCorrection + 360;
  }
  
  Serial.print("Heading (degrees): "); Serial.println(headingDegrees);

  // Calculate corresponding servo position
  if (headingDegreesInteger > 90 && headingDegreesInteger < 270)
  {
    if (headingDegreesInteger <= 180)
    {
      servoPos = 180;
    }
    else
    {
      servoPos = 10;
    }
  }
  else
  {
    if (headingDegreesInteger <= 90)
    {
      servoPos = map(headingDegreesInteger, 0, 90, 85, 10);
    }
    else
    {
      servoPos = map(headingDegreesInteger, 360, 270, 85, 180);
    }
  }
  myservo.write(servoPos);
  delay(10);
}
