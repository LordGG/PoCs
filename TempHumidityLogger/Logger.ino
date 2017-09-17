#include "DHT.h" // pour le capteur de temperature/humdite DHT22
#include <SPI.h> // pour le lecteur de carte SD
#include <SD.h> // Idem
#include <Wire.h> // pour l'i2c (RTC et ecran)
#include <RTClib.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#define I2C_ADDRESS 0x3C
#define DHTPIN A0     // what digital pin we're connected to (DHT data pin)
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE); // temp/humidite
File logFile; // fichier sd card
RTC_DS1307 RTC; // date et heure
SSD1306AsciiWire oled; // ecran oled

const int button1Interval = 2;
const int button2Interval = 3;
const int button3Screen = 4;

const String LOG_FILE_NAME = "LOGS_T_H.CSV";
long intervalleMesures = 2000;
String intervalleMesuresTxt = "";
String temperature = "";
String humidity = "";
String dateTime = "";
String currentDate = "";
String currentTime = "";
boolean iSOnError = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Temperature et humidite logger !");

  pinMode(button1Interval, INPUT);
  pinMode(button2Interval, INPUT);
  pinMode(button3Screen, INPUT);

  // initialisation du capteur DHT22
  pinMode(DHTPIN, OUTPUT); // pin data pour le DHT22
  dht.begin();

  // initialisation du i2c
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // La ligne suivante permet de régler l'heure sur l'heure du systeme au moment de la compilation
    //RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  // Initialisation de la lib SD, le pin 10 (SS) est utilisé pour le ChipSelect du module de la carte SD
  if (!SD.begin(10)) {
    Serial.println("Erreur initialisation carte SD !");
    iSOnError = true;
  }
  else 
  {
    iSOnError = false;
  }

  // initialisation de l'ecran
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(ZevvPeep8x16);
  oled.clear();
}

void loop() {
  // Une mesure toutes les <intervalleMesures> secondes
  int button1IntervalState = digitalRead(button1Interval);
  int button1Interva2State = digitalRead(button2Interval);
  int button3ScreenState = digitalRead(button3Screen);

  if (button1IntervalState == HIGH && button1Interva2State == HIGH)
  {
    intervalleMesures = 60000;
    intervalleMesuresTxt = "1m";
  }
  else if (button1IntervalState == HIGH && button1Interva2State == LOW)
  {
    intervalleMesures = 900000;
    intervalleMesuresTxt = "15m";
  }
  else if (button1IntervalState == LOW && button1Interva2State == HIGH)
  {
    intervalleMesures = 1800000;
    intervalleMesuresTxt = "30m";
  }
  else if (button1IntervalState == LOW && button1Interva2State == LOW)
  {
    intervalleMesures = 3600000;
    intervalleMesuresTxt = "1h";
  }

  String logLine = GetTemperatureAndHumidityLog();
  if (logLine == "")
  {
    Serial.println("Erreur de lecture de la temperature et humidite !");
  }
  else
  {
    Serial.print(logLine);    
    if (button3ScreenState == HIGH)
    {
      drawOledScreen();
    }
    else
    {
      oled.clear();
    }
    WriteLineToSdCard(LOG_FILE_NAME, logLine);
  }

  delay(intervalleMesures);
}

String GetTemperatureAndHumidityLog()
{
  float h = dht.readHumidity();
  humidity = h;
  float t = dht.readTemperature();
  temperature = t;
  
  if (isnan(h) || isnan(t)) {
    return "";
  }
  
  String logLine = "";
  logLine += GetDate();
  logLine += ";";
  logLine += t;
  logLine += ";";
  logLine += h;
  logLine += + "\n";
  return logLine;
}

void WriteLineToSdCard(String fileName, String logLine)
{
  logFile = SD.open(fileName, FILE_WRITE);

  // if the file opened okay, write to it:
  if (logFile) {
    logFile.print(logLine);
    // close the file:
    logFile.close();
    iSOnError = false;
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening " + fileName);
    iSOnError = true;
  }
}

String GetDate()
{
    DateTime now = RTC.now();
    dateTime = "";
    currentDate = "";
    currentTime = "";
    currentDate += AddLeadingZero(now.day());
    currentDate += "/";
    currentDate += AddLeadingZero(now.month());
    currentDate += "/";
    currentDate += now.year();
    currentTime += AddLeadingZero(now.hour());
    currentTime += ":";
    currentTime += AddLeadingZero(now.minute());
    currentTime += ":";
    currentTime += AddLeadingZero(now.second());
    dateTime = currentDate + " " + currentTime;
    return dateTime;
}

String AddLeadingZero(uint8_t num)
{
  String res = "";
  res += num;
  if (num < 10)
  {
    return "0" + res;
  }
  else
  {
    return res;
  }  
}

// dessine tout l ecran OLEDs
void drawOledScreen()
{
  oled.clear();
  //oled.set2X();
  if(iSOnError)
  {
    oled.print("/!\\ ");
  }
  else
  {
    oled.print("    ");
  }
  oled.print(currentTime);
  oled.setFont(font5x7);
  oled.print(" #" + intervalleMesuresTxt);  
  oled.setFont(ZevvPeep8x16);
  oled.println("");

  oled.println("   " + currentDate);
  oled.println("Temp. : " + temperature + " oC" );
  oled.println("Humi. : " + humidity + " % ");
}
