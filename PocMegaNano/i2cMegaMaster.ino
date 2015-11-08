#include <U8glib.h> // pour l ecran OLED
#include <Encoder.h> // pour l encodeur sur le moteur
#include <LedControl.h> // pour la matrice de LEDs MAX7219
#include <Wire.h> // pour l i2c

#define ADDRESS             0x68                                   // Address du TPA81 (tableau thermique)
#define SOFTREG             0x00                                   // Octet pour stocker la version du TPA81
#define AMBIANT             0x01                                   // Octet pour stocker la température ambiante

int leds[] = {30, 31, 32, 33, 34, 35, 36, 37}; // Tableau contenant les pins de mes 8 LEDs
int temperature[] = {0,0,0,0,0,0,0,0}; // Tableau pour stocker les 8 temperatures relevees 
int highestTemperatureCell = 0; // index de la cellule ayant releve la plus haute température
int highestTemperature = 0; // plus haute temperature relevee lors d une analyse

LedControl lc=LedControl(50,51,53,1);  // Pins: DIN-MISO,CLK-MOSI,CS-SS, # nombre de matrices de LEDs connectees

Encoder motorEncoder(2, 3);
long motorPosition  = -999;

int sonar1Distance = 0;
int sonar2Distance = 0;

int ldrPin = A0;
int ldrValueThreshold = 700;
bool isLightDetected = false;

unsigned long lastMillisValue = millis();
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK); // constructeur pour l ecran OLED 128*64 SSD1306
int delayBetweenOledDisplayUpdates = 200; // delai entre 2 mises à jour de l ecran OLED en ms


// icone coeur pour matrice de LEDs
byte love[] =
{
   B00110000,
   B01111000,
   B01111100,
   B00111110,
   B00111110,
   B01111100,
   B01111000,
   B00110000
};

// smiley pour matrice de LEDs
byte happy[] =
{
   B00111100,
   B01000010,
   B10101001,
   B10000101,
   B10000101,
   B10101001,
   B01000010,   
   B00111100
};

// icone ampoule pour matrice de LEDs
byte light[] =
{
   B01111000,
   B11001000,
   B10000110,
   B10100101,
   B10100101,
   B10000110,
   B11001000,   
   B01111000
};

void setup() {
  Serial.begin(115200); // connexion au moniteur serie 
  Serial.println("Initialisation...");

  // declaration des pins raccordes aux LEDs (du bargraphe) en tant que OUTPUT
  for(int i = 0; i < 8; i++){
    pinMode(leds[i], OUTPUT);
  }

  Wire.begin(); // ecoute sur le bus i2c

  //----- initialisation matrice de LEDs MAX7219 -----//
  lc.shutdown(0,false);  // reveille lla matrice de LEDs
  lc.setIntensity(0,1);  // intensite (brillance) de la matrice de LEDs
  lc.clearDisplay(0);  // ecran vierge

  //----- initialisation ecran OLED i2c 128x64 -----//
  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  
  Serial.println("OK, Demarrage...");
  blinkLeds(200, 3);  
}

void loop() {
  checkLight(); // verifie le niveau de luminosite
  lightHottestCell(); // allume la LED du bargraphe correspondant a la cellule la plus chaude du TPA81 et recupere la temperature la plus elevee du bargraphe
  callToNanoI2cSlaveForDistances(); // on recupere les distances mesurees par les capteurs du Nano connecte en i2c
  readMotorPosition(); // recupere le nombre de ticks comptes par l encodeur
  // on met a jour l ecran toutes les 200 ms
  if ((millis() - lastMillisValue) > delayBetweenOledDisplayUpdates)
  {
    drawOledScreen(); // dessine l ecran OLED
    lastMillisValue = millis();
  }
    
  // affiche l icone voulu
  setMatrix();
}

// allume toutes les LEDSs
void allLedsOn()
{
  for(int i = 0; i < 8; i++){                            
    digitalWrite(leds[i], HIGH);
  }
}

// eteint toutes les LEDs
void allLedsOff()
{
  for(int i = 0; i < 8; i++){                            
    digitalWrite(leds[i], LOW);
  }
}

// fait clignoter le bargraphe x fois, avec un delai de y ms
void blinkLeds(int delayDuration, int times)
{
  for (int i = 0; i < times; i++)
  {
    allLedsOff();
    allLedsOn();
    delay(delayDuration);
    allLedsOff();
    delay(delayDuration);
  }
}

// Fonction pour recevoir un octet de donnees du TPA81
byte getDataFromThermalArray(byte reg){                                   
  Wire.beginTransmission(ADDRESS);                        // demarrage communication avec le TPA81
  Wire.write(reg);                                        // declaration du registre a retourner
  Wire.endTransmission();
  Wire.requestFrom(ADDRESS, 1);                           // demande de 1 octet
  while(Wire.available() < 1);                            // on attend que l octet arrive
  byte data = Wire.read();                                // recuperation de l octet
  return(data);                                           
}

// allume la LED du bargraphe correspondant a la cellule la plus chaude du TPA81
void lightHottestCell()
{
  for(int i = 0; i < 8; i++){                             // remplissage du tableau avec les valeurs des 8 registres du TPA81
    temperature[i] = getDataFromThermalArray(i+2);
  }
  
  String temperaturesDisplay = "";
  highestTemperature = 0;
  for(int x = 0; x < 8; x++){                            // lecture des 8 temperatures relevees
    // on identifie la plus elevee ainsi que son index
    if (temperature[x] > highestTemperature)
    {
      highestTemperature = temperature[x];
      highestTemperatureCell = x;
    }
    // on genere la ligne a afficher sur l ecran OLED
    temperaturesDisplay = temperaturesDisplay + temperature[x] + " ";
  }
  allLedsOff();
  digitalWrite(leds[highestTemperatureCell], HIGH);       // on allume la LEDs correspondante 
}

// lecture des distances retournees par le Nano (esclave sur le bus i2c)
void callToNanoI2cSlaveForDistances()
{
  Wire.requestFrom(8, 4);    // on demande 4 octets au peripherique esclave d adresse 8 (le Nano) (recupere 2 entiers, donc 4 octets)
  int i = 0;
  byte result[] = {0,0,0,0};
  while (Wire.available()) { // slave may send less than requested
    result[i] = Wire.read();
    i++;
  }

  // on transforme le tableau de 4 octets en tableau de 2 entiers
  unsigned int* pData = (unsigned int*)result;
  sonar1Distance = pData[0];
  sonar2Distance = pData[1];
}

// affiche un motif sur la matrice de LEDs
void setMatrixIcon(byte* mood)
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,mood[i]);
  }
}

// affiche le bon icone en fonction des variables globales
void setMatrix()
{
  if (isLightDetected == true)
  {
    setMatrixIcon(light);
    return;
  }
    // si temperature max superieure a 50 degres, on affiche un coeur, sinon un smiley
  if (highestTemperature > 50)
  {
    setMatrixIcon(love);
  }
  else
  {
    setMatrixIcon(happy);
  }
}

// dessine tout l ecran OLEDs en faisant appel a la methode drawOledScreenInLoop (plus d infos : https://github.com/olikraus/u8glib/wiki/tpictureloop)
void drawOledScreen()
{
  u8g.firstPage();  
  do {
    drawOledScreenInLoop();
  } while( u8g.nextPage() );
}

// est executee en boucle jusqu a ce qu elle ait dessine tout l ecran (plus d infos : https://github.com/olikraus/u8glib/wiki/tpictureloop)
void drawOledScreenInLoop(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont); // choix de la police
  //u8g.setFont(u8g_font_osb21);

  // Affichage de la temperature
  String displayedMaxTemp = "Max Temp: ";
  displayedMaxTemp = displayedMaxTemp + highestTemperature + " oC ";
  // conversion de la string a afficher en tableau de char
  char displayMaxTempBuffer[displayedMaxTemp.length()]; 
  displayedMaxTemp.toCharArray(displayMaxTempBuffer, displayedMaxTemp.length());
  u8g.drawStr(0, 20, displayMaxTempBuffer); // affichage de la string (coordonnees : X : 0, Y: 20, contenu)

  // Affichage de la position du moteur
  String displayedMotorPosition = "Moteur: ";
  displayedMotorPosition = displayedMotorPosition + motorPosition;
  // conversion de la string a afficher en tableau de char
  char displayedMotorPositionBuffer[displayedMotorPosition.length()]; 
  displayedMotorPosition.toCharArray(displayedMotorPositionBuffer, displayedMotorPosition.length());
  u8g.drawStr(0, 40, displayedMotorPositionBuffer);

  // affiche de la distance mesuree par le premier sonar
  String displaySonar1Distance = "S1:";
  displaySonar1Distance = displaySonar1Distance + sonar1Distance + "cm ";
  // conversion de la string a afficher en tableau de char
  char displaySonar1DistanceBuffer[displaySonar1Distance.length()]; 
  displaySonar1Distance.toCharArray(displaySonar1DistanceBuffer, displaySonar1Distance.length());
  u8g.drawStr(0, 60, displaySonar1DistanceBuffer);

  // affiche de la distance mesuree par le second sonar
  String displaySonar2Distance = "S2:";
  displaySonar2Distance = displaySonar2Distance + sonar2Distance + "cm ";
  // conversion de la string a afficher en tableau de char
  char displaySonar2DistanceBuffer[displaySonar2Distance.length()]; 
  displaySonar2Distance.toCharArray(displaySonar2DistanceBuffer, displaySonar2Distance.length());
  u8g.drawStr(64, 60, displaySonar2DistanceBuffer);
}

// lit la position du moteur (le nombre de ticks comptes par l encodeur)
void readMotorPosition()
{
  long newMotorPosition;
  newMotorPosition = motorEncoder.read();
  if (newMotorPosition != motorPosition) {
    motorPosition = newMotorPosition;
  }
}

// verifie si la lumiere est allumee
void checkLight()
{
  int ldrValue = analogRead(ldrPin);
  if (ldrValue > ldrValueThreshold)
  {
    isLightDetected = true;
  }
  else
  {
    isLightDetected = false;
  }
}
