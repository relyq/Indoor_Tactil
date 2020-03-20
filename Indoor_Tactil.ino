// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.
// Technical support:goodtft@163.com

#include "Adafruit_GFX.h"     // Core graphics library
#include "Adafruit_TFTLCD.h"  // Hardware-specific library
#include "TouchScreen.h"

#include <SPI.h>

#include <DHT.h>

#define DHTPIN 40  // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11  // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3  // Chip Select goes to Analog 3
#define LCD_CD A2  // Command/Data goes to Analog 2
#define LCD_WR A1  // LCD Write goes to Analog 1
#define LCD_RD A0  // LCD Read goes to Analog 0

#define LCD_RESET A4  // Can alternately just connect to Arduino's reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// Assign human-readable names to some common 16-bit color values:
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

// Color definitions
#define ILI9341_BLACK 0x0000       /*   0,   0,   0 */
#define ILI9341_NAVY 0x000F        /*   0,   0, 128 */
#define ILI9341_DARKGREEN 0x03E0   /*   0, 128,   0 */
#define ILI9341_DARKCYAN 0x03EF    /*   0, 128, 128 */
#define ILI9341_MAROON 0x7800      /* 128,   0,   0 */
#define ILI9341_PURPLE 0x780F      /* 128,   0, 128 */
#define ILI9341_OLIVE 0x7BE0       /* 128, 128,   0 */
#define ILI9341_LIGHTGREY 0xC618   /* 192, 192, 192 */
#define ILI9341_DARKGREY 0x7BEF    /* 128, 128, 128 */
#define ILI9341_BLUE 0x001F        /*   0,   0, 255 */
#define ILI9341_GREEN 0x07E0       /*   0, 255,   0 */
#define ILI9341_CYAN 0x07FF        /*   0, 255, 255 */
#define ILI9341_RED 0xF800         /* 255,   0,   0 */
#define ILI9341_MAGENTA 0xF81F     /* 255,   0, 255 */
#define ILI9341_YELLOW 0xFFE0      /* 255, 255,   0 */
#define ILI9341_WHITE 0xFFFF       /* 255, 255, 255 */
#define ILI9341_ORANGE 0xFD20      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0xAFE5 /* 173, 255,  47 */
#define ILI9341_PINK 0xF81F

/******************* UI details */
#define BUTTON_X 40
#define BUTTON_Y 100
#define BUTTON_W 60
#define BUTTON_H 30
#define BUTTON_SPACING_X 20
#define BUTTON_SPACING_Y 20
#define BUTTON_TEXTSIZE 3
#define BUTTON_TEXTSIZE1 2

// text box where numbers go
#define TEXT_X 10
#define TEXT_Y 10
#define TEXT_W 220
#define TEXT_H 50
#define TEXT_TSIZE 3
#define TEXT_TCOLOR ILI9341_MAGENTA
// the data (phone #) we store in the textfield
#define TEXT_LEN 12
char textfield[TEXT_LEN + 1] = "";
uint8_t textfield_i = 0;

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

// Tenemos una línea de estado para Me gusta, FONA está trabajando
#define STATUS_X 10
#define STATUS_Y 65

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
// Si se usa el escudo, todas las líneas de control y de datos son fijas, y
// Se puede usar opcionalmente una declaración más simple:
// Adafruit_TFTLCD tft;

Adafruit_GFX_Button buttons[15];
Adafruit_GFX_Button settingsButton;
Adafruit_GFX_Button backHomeButton;
Adafruit_GFX_Button backMenuButton;
Adafruit_GFX_Button opcion1Button;
Adafruit_GFX_Button opcion2Button;
Adafruit_GFX_Button opcion3Button;
Adafruit_GFX_Button opcion4Button;
Adafruit_GFX_Button opcion5Button;
Adafruit_GFX_Button veranoz1Button;
Adafruit_GFX_Button otonioz1Button;
Adafruit_GFX_Button inviernoz1Button;
Adafruit_GFX_Button primaveraz1Button;
Adafruit_GFX_Button veranoz2Button;
Adafruit_GFX_Button otonioz2Button;
Adafruit_GFX_Button inviernoz2Button;
Adafruit_GFX_Button primaveraz2Button;
Adafruit_GFX_Button controlz1Button;
Adafruit_GFX_Button controlz2Button;
Adafruit_GFX_Button alarmasButton;
Adafruit_GFX_Button wifiButton;
Adafruit_GFX_Button iotButton;

/* Crea 15 botones, en el clásico estilo de teléfono candybar. */
char buttonlabels[15][5] = {"Ent.", "Clr.", "Esc.", "1", "2", "3", "4", "5",
                            "6",    "7",    "8",    "9", "*", "0", "#"};
uint16_t buttoncolors[15] = {
    ILI9341_DARKGREEN, ILI9341_DARKGREY, ILI9341_RED,   ILI9341_BLUE,
    ILI9341_BLUE,      ILI9341_BLUE,     ILI9341_BLUE,  ILI9341_BLUE,
    ILI9341_BLUE,      ILI9341_BLUE,     ILI9341_BLUE,  ILI9341_BLUE,
    ILI9341_ORANGE,    ILI9341_BLUE,     ILI9341_ORANGE};

////////////////////////////////////
bool numericScreenOn = false;
bool numericScreenInit = false;
////////////////////////////////////
bool homeScreenOn = true;
bool homeScreenInit = false;
////////////////////////////////////
bool settingsScreenOn = false;
bool settingsScreenInit = false;
////////////////////////////////////
bool menuScreenOn = false;
bool menuScreenInit = false;
////////////////////////////////////
bool opcion1ScreenOn = false;
bool opcion1ScreenInit = false;
////////////////////////////////////
bool opcion2ScreenOn = false;
bool opcion2ScreenInit = false;
////////////////////////////////////
bool opcion3ScreenOn = false;
bool opcion3ScreenInit = false;
////////////////////////////////////
bool opcion4ScreenOn = false;
bool opcion4ScreenInit = false;
////////////////////////////////////
bool opcion5ScreenOn = false;
bool opcion5ScreenInit = false;
////////////////////////////////////
bool veranoz1ScreenOn = false;
bool veranoz1ScreenInit = false;
////////////////////////////////////
bool otonioz1ScreenOn = false;
bool otonioz1ScreenInit = false;
////////////////////////////////////
bool inviernoz1ScreenOn = false;
bool inviernoz1ScreenInit = false;
////////////////////////////////////
bool primaveraz1ScreenOn = false;
bool primaveraz1ScreenInit = false;
////////////////////////////////////
bool veranoz2ScreenOn = false;
bool veranoz2ScreenInit = false;
////////////////////////////////////
bool otonioz2ScreenOn = false;
bool otonioz2ScreenInit = false;
////////////////////////////////////
bool inviernoz2ScreenOn = false;
bool inviernoz2ScreenInit = false;
////////////////////////////////////
bool primaveraz2ScreenOn = false;
bool primaveraz2ScreenInit = false;
////////////////////////////////////
bool controlz1ScreenOn = false;
bool controlz1ScreenInit = false;
////////////////////////////////////
bool controlz2ScreenOn = false;
bool controlz2ScreenInit = false;
////////////////////////////////////
bool alarmasScreenOn = false;
bool alarmasScreenInit = false;
////////////////////////////////////
bool wifiScreenOn = false;
bool wifiScreenInit = false;
////////////////////////////////////
bool iotScreenOn = false;
bool iotScreenInit = false;
////////////////////////////////////
bool numericScreenOnTemperature = false;
bool numericScreenOnHumidity = false;
////////////////////////////////////

String serialTemp;
String oldData = "";
String temperatureSP = "0";
String humiditySP = "0";

bool heaterOn = false;
bool fanOn = false;

void setup(void) {
  dht.begin();
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));

#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  Serial.println(F("Using Adafruit 2.4\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Adafruit 2.4\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is ");
  Serial.print(tft.width());
  Serial.print("x");
  Serial.println(tft.height());

  tft.reset();

  uint16_t identifier = tft.readID();
  if (identifier == 0x0101) identifier = 0x9341;

  if (identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if (identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  } else if (identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if (identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if (identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if (identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(
        F("If using the Adafruit 2.4\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(
        F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(
        F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    return;
  }

  tft.begin(identifier);
}

// Imprime algo en la mini barra de estado con cualquiera de las cadenas de
// flash
void status(const __FlashStringHelper *msg) {
  tft.fillRect(STATUS_X, STATUS_Y, 240, 8, ILI9341_BLACK);
  tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.print(msg);
}

// o caracteres
void status(char *msg) {
  tft.fillRect(STATUS_X, STATUS_Y, 240, 8, ILI9341_BLACK);
  tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.print(msg);
}
#define MINPRESSURE 10
#define MAXPRESSURE 250


////// screens



void initAllScreens() {
  veranoz1ScreenInit = false;
  otonioz1ScreenInit = false;
  inviernoz1ScreenInit = false;
  primaveraz1ScreenInit = false;
  veranoz2ScreenInit = false;
  otonioz2ScreenInit = false;
  inviernoz2ScreenInit = false;
  primaveraz2ScreenInit = false;
  controlz1ScreenInit = false;
  controlz2ScreenInit = false;
  opcion1ScreenInit = false;
  opcion2ScreenInit = false;
  opcion3ScreenInit = false;
  opcion4ScreenInit = false;
  opcion5ScreenInit = false;
  alarmasScreenInit = false;
  wifiScreenInit = false;
  iotScreenInit = false;
  numericScreenInit = false;
  homeScreenInit = false;
  menuScreenInit = false;
  settingsScreenInit = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////  Loop Principal
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop(void) {
  /*TSPoint p;
  p = ts.getPoint();
  */
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // Si comparte pines, deberá fijar las instrucciones de los pines de la
  // pantalla táctil.
  // pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  // pinMode(YM, OUTPUT);

  // Tenemos una presión mínima que consideramos 'válida'
  // ¡La presión de 0 significa no presionar!

  // p = ts.getPoint();
  /*
  if (ts.bufferSize()) {



  } else {
    // ¡Esta es nuestra forma de rastrear el lanzamiento del touch!
    p.x = p.y = p.z = -1;
  }*/

  // Escale de ~ 0-> 4000 a tft.width usando los # de calibración
  /*
  if (p.z != -1) {
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    Serial.print("("); Serial.print(p.x); Serial.print(", ");
    Serial.print(p.y); Serial.print(", ");
    Serial.print(p.z); Serial.println(") ");
  }*/
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);

    Serial.print("(");
    Serial.print(p.x);
    Serial.print(", ");
    Serial.print(p.y);
    Serial.print(", ");
    Serial.print(p.z);
    Serial.println(") ");

    // delay(100);
  } /*
  else{
   pt = 1
   if (p.z == 0) {
   pt = 0SS
   }
  }
  */
  /*
   if(Serial.available()){
    serialTemp = Serial.readString();
   }
   if(oldData!=serialTemp){
    oldData = serialTemp;
    //homeScreenOn = false;
    }
    */

  if (homeScreenOn) {
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    if (t < temperatureSP.toFloat()) {
      heaterOn = true;
    } else {
      heaterOn = false;
    }

    if (h > humiditySP.toFloat()) {
      fanOn = true;
    } else {
      fanOn = false;
    }
    // initAllScreens();
    HomeScreen(String(t), String(h), temperatureSP, humiditySP, heaterOn,
               fanOn);
    // HomeScreen("14.5","20",temperatureSP,humiditySP);
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////  Acciones del menu
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (settingsScreenOn) {
    // initAllScreens();
    SettingsScreen(temperatureSP, humiditySP);
  }

  if (menuScreenOn) {
    // initAllScreens();
    MenuScreen(temperatureSP, humiditySP);
  }

  if (opcion1ScreenOn) {
    // initAllScreens();
    Opcion1Screen(temperatureSP, humiditySP);
  }

  if (opcion2ScreenOn) {
    // initAllScreens();
    Opcion2Screen(temperatureSP, humiditySP);
  }
  /*
    if(opcion3ScreenOn){
      //initAllScreens();
      Opcion3Screen(temperatureSP, humiditySP);
      }
  */
  if (opcion4ScreenOn) {
    // initAllScreens();
    Opcion4Screen(temperatureSP, humiditySP);
  }
  /*
    if(opcion5ScreenOn){
      //initAllScreens();
      Opcion5Screen(temperatureSP, humiditySP);
      }
  */
  if (veranoz1ScreenOn) {
    // initAllScreens();
    Veranoz1Screen(temperatureSP, humiditySP);
  }

  if (otonioz1ScreenOn) {
    // initAllScreens();
    Otonioz1Screen(temperatureSP, humiditySP);
  }

  if (inviernoz1ScreenOn) {
    // initAllScreens();
    Inviernoz1Screen(temperatureSP, humiditySP);
  }

  if (primaveraz1ScreenOn) {
    // initAllScreens();
    Primaveraz1Screen(temperatureSP, humiditySP);
  }

  if (veranoz2ScreenOn) {
    // initAllScreens();
    Veranoz2Screen(temperatureSP, humiditySP);
  }

  if (otonioz2ScreenOn) {
    // initAllScreens();
    Otonioz2Screen(temperatureSP, humiditySP);
  }

  if (inviernoz2ScreenOn) {
    // initAllScreens();
    Inviernoz2Screen(temperatureSP, humiditySP);
  }

  if (primaveraz2ScreenOn) {
    // initAllScreens();
    Primaveraz2Screen(temperatureSP, humiditySP);
  }

  if (controlz1ScreenOn) {
    // initAllScreens();
    Controlz1Screen(temperatureSP, humiditySP);
  }

  if (controlz2ScreenOn) {
    // initAllScreens();
    Controlz2Screen(temperatureSP, humiditySP);
  }

  if (alarmasScreenOn) {
    // initAllScreens();
    AlarmasScreen(temperatureSP, humiditySP);
  }

  if (wifiScreenOn) {
    // initAllScreens();
    WiFiScreen(temperatureSP, humiditySP);
  }

  if (iotScreenOn) {
    // initAllScreens();
    IotScreen(temperatureSP, humiditySP);
  }

  // Serial.println("Temp. SP "+temperatureSP+"settingsScreen
  // "+settingsScreenOn);
  if (numericScreenOnTemperature || numericScreenOnHumidity) {
    NumericKeyboardScreen(p);
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////  Botones del menu
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Configuracion Button
  if (homeScreenOn && p.x > 10 && p.x < 230 && p.y > 30 && p.y < 70) {
    Serial.println("Configuracion ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = false;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = true;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    numericScreenOnTemperature = false;
    initAllScreens();
  }

  // Opcion 1 Button
  if (menuScreenOn && p.x > 10 && p.x < 230 && p.y > 55 && p.y < 95) {
    Serial.println("Opcion 1 ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = true;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // Opcion 2 Button
  if (menuScreenOn && p.x > 10 && p.x < 230 && p.y > 100 && p.y < 140) {
    Serial.println("Opcion 2 ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = false;
    opcion2ScreenOn = true;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // Opcion 3 Button
  /*if(menuScreenOn && p.x>10 && p.x<230 && p.y>145 && p.y<185){
      Serial.println("Opcion 3 ON");
      veranoz1ScreenOn = false;
      otonioz1ScreenOn = false;
      inviernoz1ScreenOn = false;
      primaveraz1ScreenOn = false;
      veranoz2ScreenOn = false;
      otonioz2ScreenOn = false;
      inviernoz2ScreenOn = false;
      primaveraz2ScreenOn = false;
      controlz1ScreenOn = false;
      controlz2ScreenOn = false;
      opcion1ScreenOn = false;
      opcion2ScreenOn = false;
      opcion3ScreenOn = true;
      opcion4ScreenOn = false;
      opcion5ScreenOn = false;
      alarmasScreenOn = false;
      wifiScreenOn = false;
      iotScreenOn = false;
      menuScreenOn = false;
      numericScreenOnTemperature = false;
      settingsScreenOn = false;
      numericScreenOn = false;
      homeScreenOn = false;
      initAllScreens();
  }*/

  // Opcion 4 Button
  if (menuScreenOn && p.x > 10 && p.x < 230 && p.y > 190 && p.y < 230) {
    Serial.println("Opcion 4 ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = false;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = true;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // Opcion 5 Button

  /*if(menuScreenOn && p.x>10 && p.x<230 && p.y>235 && p.y<275){
    Serial.println("Opcion 5 ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = false;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = true;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }*/

  // verano Z1 Button

  if (opcion1ScreenOn && p.x > 10 && p.x < 230 && p.y > 55 && p.y < 95) {
    Serial.println("verano Z1 ON");
    veranoz1ScreenOn = true;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = false;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // Otoño Z1 Button
  if (opcion1ScreenOn && p.x > 10 && p.x < 230 && p.y > 100 && p.y < 140) {
    Serial.println("Otonio Z1 ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = true;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = false;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // Invierno Z1 Button
  if (opcion1ScreenOn && p.x > 10 && p.x < 230 && p.y > 145 && p.y < 185) {
    Serial.println("Invierno Z1 ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = true;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = false;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // Primavera Z1 Button
  if (opcion1ScreenOn && p.x > 10 && p.x < 230 && p.y > 190 && p.y < 230) {
    Serial.println("Primavera Z1 ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = true;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = false;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // Verano Z2 Button
  if (opcion2ScreenOn && p.x > 10 && p.x < 230 && p.y > 55 && p.y < 95) {
    Serial.println("Verano Z2 ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = true;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = false;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // Otoño Z2 Button
  if (opcion2ScreenOn && p.x > 10 && p.x < 230 && p.y > 100 && p.y < 140) {
    Serial.println("Otonio Z2 ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = true;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = false;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // Invierno Z2 Button
  if (opcion2ScreenOn && p.x > 10 && p.x < 230 && p.y > 145 && p.y < 185) {
    Serial.println("Invierno Z2 ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = true;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = false;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // Primavera Z2 Button
  if (opcion2ScreenOn && p.x > 10 && p.x < 230 && p.y > 190 && p.y < 230) {
    Serial.println("Primavera Z2 ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = true;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = false;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // Control Z1 Button
  if (opcion1ScreenOn && p.x > 10 && p.x < 230 && p.y > 235 && p.y < 275) {
    Serial.println("Control Z1 ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = true;
    controlz2ScreenOn = false;
    opcion1ScreenOn = false;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // Control Z2 Button
  if (opcion2ScreenOn && p.x > 10 && p.x < 230 && p.y > 235 && p.y < 275) {
    Serial.println("Control Z2 ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = true;
    opcion1ScreenOn = false;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // Alarmas Button
  if (opcion4ScreenOn && p.x > 10 && p.x < 230 && p.y > 55 && p.y < 95) {
    Serial.println("Alarmas ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = false;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = true;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // WiFi Button
  if (opcion4ScreenOn && p.x > 10 && p.x < 230 && p.y > 100 && p.y < 140) {
    Serial.println("WiFi ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = false;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = true;
    iotScreenOn = false;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // Iot Button
  if (opcion4ScreenOn && p.x > 10 && p.x < 230 && p.y > 145 && p.y < 185) {
    Serial.println("Iot ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = false;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = true;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // BackHome Button
  if (menuScreenOn && p.x > 10 && p.x < 230 && p.y > 300 && p.y < 340) {
    Serial.println("BackHome ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = false;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = true;
    initAllScreens();
  }

  // BackMenu Button
  if ((opcion1ScreenOn || opcion2ScreenOn || opcion3ScreenOn ||
       opcion4ScreenOn || opcion5ScreenOn || alarmasScreenOn || wifiScreenInit || iotScreenInit) &&
      p.x > 10 && p.x < 230 && p.y > 300 && p.y < 340) {
    Serial.println("BackMenu ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = false;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = true;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // BackOpcion1 Button
  if ((veranoz1ScreenOn || otonioz1ScreenOn || inviernoz1ScreenOn ||
       primaveraz1ScreenOn || controlz1ScreenOn) &&
      p.x > 10 && p.x < 230 && p.y > 300 && p.y < 340) {
    Serial.println("BackOpcion1 ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = true;
    opcion2ScreenOn = false;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // BackOpcion2 Button
  if ((veranoz2ScreenOn || otonioz2ScreenOn || inviernoz2ScreenOn ||
       primaveraz2ScreenOn || controlz2ScreenOn) &&
      p.x > 10 && p.x < 230 && p.y > 300 && p.y < 340) {
    Serial.println("BackOpcion2 ON");
    veranoz1ScreenOn = false;
    otonioz1ScreenOn = false;
    inviernoz1ScreenOn = false;
    primaveraz1ScreenOn = false;
    veranoz2ScreenOn = false;
    otonioz2ScreenOn = false;
    inviernoz2ScreenOn = false;
    primaveraz2ScreenOn = false;
    controlz1ScreenOn = false;
    controlz2ScreenOn = false;
    opcion1ScreenOn = false;
    opcion2ScreenOn = true;
    opcion3ScreenOn = false;
    opcion4ScreenOn = false;
    opcion5ScreenOn = false;
    alarmasScreenOn = false;
    wifiScreenOn = false;
    iotScreenOn = false;
    menuScreenOn = false;
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // Temperature TextBox
  if (settingsScreenOn && p.x > 10 && p.x < 230 && p.y > 40 && p.y < 80) {
    // Serial.println("Home ON");
    numericScreenOnTemperature = true;
    numericScreenOnHumidity = false;
    settingsScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // Humidity TextBox
  if (settingsScreenOn && p.x > 10 && p.x < 230 && p.y > 130 && p.y < 170) {
    // Serial.println("Home ON");
    numericScreenOnTemperature = false;
    numericScreenOnHumidity = true;
    settingsScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  // NumericKeyboardScreen(p);
  // delay(300);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////  Fin Loop Principal
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////