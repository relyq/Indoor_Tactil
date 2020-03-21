
#include "Adafruit_GFX.h"     // Core graphics library
#include "Adafruit_TFTLCD.h"  // Hardware-specific library
#include "TouchScreen.h"

#include <SPI.h>

#include <DHT.h>

#define DHTPIN 40  // what digital pin we're connected to

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
// optional
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

// We have a status line for like, is FONA working
#define STATUS_X 10
#define STATUS_Y 65

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
// ???????????????
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

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

/* create 15 buttons, in classic candybar phone style */
char buttonlabels[15][5] = {"Ent.", "Clr.", "Esc.", "1", "2", "3", "4", "5",
                            "6",    "7",    "8",    "9", "*", "0", "#"};
uint16_t buttoncolors[15] = {
    ILI9341_DARKGREEN, ILI9341_DARKGREY, ILI9341_RED,   ILI9341_BLUE,
    ILI9341_BLUE,      ILI9341_BLUE,     ILI9341_BLUE,  ILI9341_BLUE,
    ILI9341_BLUE,      ILI9341_BLUE,     ILI9341_BLUE,  ILI9341_BLUE,
    ILI9341_ORANGE,    ILI9341_BLUE,     ILI9341_ORANGE};

const uint8_t screensTotal = 24;  // cantidad de pantallas
bool screens[screensTotal] = {
    // array donde se guarda la pantalla activa - entonces, screens[1] = true
    // significa menuScreenOn = true
    1,  // 0 - homeScreenOn
    0,  // 1 - menuScreenOn
    0,  // 2 - settingsScreenOn - configuracion temp hum
    0,  // 3 - alarmasScreenOn
    0,  // 4 - wifiScreenOn
    0,  // 5 - iotScreenOn
    0,  // 6 - opcion1ScreenOn - zona 1
    0,  // 7 - opcion2ScreenOn - zona 2
    0,  // 8 - opcion3ScreenOn
    0,  // 9 - opcion4ScreenOn - opciones
    0,  // 10 - opcion5ScreenOn
    0,  // 11 - veranoz1ScreenOn
    0,  // 12 - otonioz1ScreenOn
    0,  // 13 - inviernoz1ScreenOn
    0,  // 14 - primaveraz1ScreenOn
    0,  // 15 - veranoz2ScreenOn
    0,  // 16 - otonioz2ScreenOn
    0,  // 17 - inviernoz2ScreenOn
    0,  // 18 - primaveraz2ScreenOn
    0,  // 19 - controlz1ScreenOn
    0,  // 20 - controlz2ScreenOn
    0,  // 21 - numericScreenOn
    0,  // 22 - numericScreenOnTemperature
    0,  // 23 - numericScreenOnHumidity
};

bool screensInit[screensTotal - 2] = {
    // array donde se guarda el estado de inicialización de cada pantalla
    0,  // 0 - homeScreenInit
    0,  // 1 - menuScreenInit
    0,  // 2 - settingsScreenInit
    0,  // 3 - alarmasScreenInit
    0,  // 4 - wifiScreenInit
    0,  // 5 - iotScreenInit
    0,  // 6 - opcion1ScreenInit
    0,  // 7 - opcion2ScreenInit
    0,  // 8 - opcion3ScreenInit
    0,  // 9 - opcion4ScreenInit
    0,  // 10 - opcion5ScreenInit
    0,  // 11 - veranoz1ScreenInit
    0,  // 12 - otonioz1ScreenInit
    0,  // 13 - inviernoz1ScreenInit
    0,  // 14 - primaveraz1ScreenInit
    0,  // 15 - veranoz2ScreenInit
    0,  // 16 - otonioz2ScreenInit
    0,  // 17 - inviernoz2ScreenInit
    0,  // 18 - primaveraz2ScreenInit
    0,  // 19 - controlz1ScreenInit
    0,  // 20 - controlz2ScreenInit
    0,  // 21 - numericScreenInit
};

String temperatureSP = "0";
String humiditySP = "0";

bool heaterOn = false;
bool fanOn = false;

void setup() {
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

#define MINPRESSURE 10
#define MAXPRESSURE 1000

void loop() {

  Serial.println("void loop()");

  TSPoint p = ts.getPoint();

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  // pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  // pinMode(YM, OUTPUT);


  // si la pantalla está siendo presionada se mappea el resultado de getPoint() a un punto valido del cursor en la pantalla
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);

    // if the screen is being touched show cursor position
    Serial.print("(");
    Serial.print(p.x);
    Serial.print(", ");
    Serial.print(p.y);
    Serial.print(", ");
    Serial.print(p.z);
    Serial.println(") ");
  }

  if (screens[0]) {
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
    HomeScreen(String(t), String(h), temperatureSP, humiditySP, heaterOn,
               fanOn);
  }


  /////////////////////////
  /////  Acciones del menu

  if (screens[2]) {
    SettingsScreen(temperatureSP, humiditySP);
  }

  else if (screens[1]) {
    MenuScreen();
  }

  else if (screens[6]) {
    Opcion1Screen();
  }

  else if (screens[7]) {
    Opcion2Screen();
  }
  /*
    if(screens[8]){
      //initAllScreens();
      Opcion3Screen(temperatureSP, humiditySP);
      }
  */
  else if (screens[9]) {
    Opcion4Screen();
  }
  /*
    if(screens[10]){
      //initAllScreens();
      Opcion5Screen(temperatureSP, humiditySP);
      }
  */
  else if (screens[11]) {
    Veranoz1Screen();
  }

  else if (screens[12]) {
    Otonioz1Screen();
  }

  else if (screens[13]) {
    Inviernoz1Screen();
  }

  else if (screens[14]) {
    Primaveraz1Screen();
  }

  else if (screens[15]) {
    Veranoz2Screen();
  }

  else if (screens[16]) {
    Otonioz2Screen();
  }

  else if (screens[17]) {
    Inviernoz2Screen();
  }

  else if (screens[18]) {
    Primaveraz2Screen();
  }

  else if (screens[19]) {
    Controlz1Screen();
  }

  else if (screens[20]) {
    Controlz2Screen();
  }

  else if (screens[3]) {
    AlarmasScreen();
  }

  else if (screens[4]) {
    WiFiScreen();
  }

  else if (screens[5]) {
    IotScreen();
  }

  else if (screens[22] || screens[23]) {
    NumericKeyboardScreen(p);
  }

  // no es necesario refrescar la pantalla si no cambio nada ni está siendo presionada
  while(!hasTHChanged() && !(p.z > MINPRESSURE && p.z < MAXPRESSURE)){
    p = ts.getPoint();
  }


  // necesito volver a tomar la posicion del cursor para que la pantalla tenga buena respuesta
  p = ts.getPoint();
  p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
  p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
  Serial.print("(");
  Serial.print(p.x);
  Serial.print(", ");
  Serial.print(p.y);
  Serial.print(", ");
  Serial.print(p.z);
  Serial.println(") ");


  //////////////////////////
  /////  Botones del menu

  // home screen
  if (screens[0] && p.x > 10 && p.x < 230 && p.y > 30 && p.y < 70) {
    Serial.println("Configuracion ON");
    changeActiveScreenTo(0x01);
    initAllScreens();
  }

  // Opcion 1 Button - zona 1
  else if (screens[1] && p.x > 10 && p.x < 230 && p.y > 55 && p.y < 95) {
    Serial.println("Opcion 1 ON");
    changeActiveScreenTo(0x06);
    initAllScreens();
  }

  // Opcion 2 Button - zona 2
  else if (screens[1] && p.x > 10 && p.x < 230 && p.y > 100 && p.y < 140) {
    Serial.println("Opcion 2 ON");
    changeActiveScreenTo(0x07);
    initAllScreens();
  }

  // Opcion 3 Button
  /*if(screens[1] && p.x>10 && p.x<230 && p.y>145 && p.y<185){
      Serial.println("Opcion 3 ON");
      screens[11] = false;
      screens[12] = false;
      screens[13] = false;
      screens[14] = false;
      screens[15] = false;
      screens[16] = false;
      screens[17] = false;
      screens[18] = false;
      screens[19] = false;
      screens[20] = false;
      screens[6] = false;
      screens[7] = false;
      screens[8] = true;
      screens[9] = false;
      screens[10] = false;
      screens[3] = false;
      screens[4] = false;
      screens[5] = false;
      screens[1] = false;
      screens[22] = false;
      screens[2] = false;
      screens[21] = false;
      screens[0] = false;
      initAllScreens();
  }*/

  // Opcion 4 Button - opciones
  else if (screens[1] && p.x > 10 && p.x < 230 && p.y > 190 && p.y < 230) {
    Serial.println("Opcion 4 ON");
    changeActiveScreenTo(0x09);
    initAllScreens();
  }

  // Opcion 5 Button

  /*if(screens[1] && p.x>10 && p.x<230 && p.y>235 && p.y<275){
    Serial.println("Opcion 5 ON");
    screens[11] = false;
    screens[12] = false;
    screens[13] = false;
    screens[14] = false;
    screens[15] = false;
    screens[16] = false;
    screens[17] = false;
    screens[18] = false;
    screens[19] = false;
    screens[20] = false;
    screens[6] = false;
    screens[7] = false;
    screens[8] = false;
    screens[9] = false;
    screens[10] = true;
    screens[3] = false;
    screens[4] = false;
    screens[5] = false;
    screens[1] = false;
    screens[22] = false;
    screens[2] = false;
    screens[21] = false;
    screens[0] = false;
    initAllScreens();
  }*/

  // verano Z1 Button

  else if (screens[6] && p.x > 10 && p.x < 230 && p.y > 55 && p.y < 95) {
    Serial.println("verano Z1 ON");
    changeActiveScreenTo(0x0b);
    initAllScreens();
  }

  // Otoño Z1 Button
  else if (screens[6] && p.x > 10 && p.x < 230 && p.y > 100 && p.y < 140) {
    Serial.println("Otonio Z1 ON");
    changeActiveScreenTo(0x0c);
    initAllScreens();
  }

  // Invierno Z1 Button
  else if (screens[6] && p.x > 10 && p.x < 230 && p.y > 145 && p.y < 185) {
    Serial.println("Invierno Z1 ON");
    changeActiveScreenTo(0x0d);
    initAllScreens();
  }

  // Primavera Z1 Button
  else if (screens[6] && p.x > 10 && p.x < 230 && p.y > 190 && p.y < 230) {
    Serial.println("Primavera Z1 ON");
    changeActiveScreenTo(0x0e);
    initAllScreens();
  }

  // Verano Z2 Button
  else if (screens[7] && p.x > 10 && p.x < 230 && p.y > 55 && p.y < 95) {
    Serial.println("Verano Z2 ON");
    changeActiveScreenTo(0x0f);
    initAllScreens();
  }

  // Otoño Z2 Button
  else if (screens[7] && p.x > 10 && p.x < 230 && p.y > 100 && p.y < 140) {
    Serial.println("Otonio Z2 ON");
    changeActiveScreenTo(0x10);
    initAllScreens();
  }

  // Invierno Z2 Button
  else if (screens[7] && p.x > 10 && p.x < 230 && p.y > 145 && p.y < 185) {
    Serial.println("Invierno Z2 ON");
    changeActiveScreenTo(0x11);
    initAllScreens();
  }

  // Primavera Z2 Button
  else if (screens[7] && p.x > 10 && p.x < 230 && p.y > 190 && p.y < 230) {
    Serial.println("Primavera Z2 ON");
    changeActiveScreenTo(0x12);
    initAllScreens();
  }

  // Control Z1 Button
  else if (screens[6] && p.x > 10 && p.x < 230 && p.y > 235 && p.y < 275) {
    Serial.println("Control Z1 ON");
    changeActiveScreenTo(0x13);
    initAllScreens();
  }

  // Control Z2 Button
  else if (screens[7] && p.x > 10 && p.x < 230 && p.y > 235 && p.y < 275) {
    Serial.println("Control Z2 ON");
    changeActiveScreenTo(0x14);
    initAllScreens();
  }

  // Alarmas Button
  else if (screens[9] && p.x > 10 && p.x < 230 && p.y > 55 && p.y < 95) {
    Serial.println("Alarmas ON");
    changeActiveScreenTo(0x03);
    initAllScreens();
  }

  // WiFi Button
  else if (screens[9] && p.x > 10 && p.x < 230 && p.y > 100 && p.y < 140) {
    Serial.println("WiFi ON");
    changeActiveScreenTo(0x04);
    initAllScreens();
  }

  // Iot Button
  else if (screens[9] && p.x > 10 && p.x < 230 && p.y > 145 && p.y < 185) {
    Serial.println("Iot ON");
    changeActiveScreenTo(0x05);
    initAllScreens();
  }

  // volver a home
  else if (screens[1] && p.x > 10 && p.x < 230 && p.y > 300 && p.y < 340) {
    Serial.println("BackHome ON");
    changeActiveScreenTo(0x00);
    initAllScreens();
  }

  // volver al menu principal
  else if ((screens[6] || screens[7] || screens[8] || screens[9] ||
            screens[10]) &&
           p.x > 10 && p.x < 230 && p.y > 300 && p.y < 340) {
    Serial.println("BackMenu ON");
    changeActiveScreenTo(0x01);
    initAllScreens();
  }

  // volver a opciones
  else if ((screens[3] || screens[4] || screens[5]) && p.x > 10 && p.x < 230 &&
           p.y > 300 && p.y < 340) {
    Serial.println("back opciones");
    changeActiveScreenTo(0x09);
    initAllScreens();
  }

  // volver a zona 1
  else if ((screens[11] || screens[12] || screens[13] || screens[14] ||
            screens[19]) &&
           p.x > 10 && p.x < 230 && p.y > 300 && p.y < 340) {
    Serial.println("BackOpcion1 ON");
    changeActiveScreenTo(0x06);
    initAllScreens();
  }

  // volver a zona 2
  else if ((screens[15] || screens[16] || screens[17] || screens[18] ||
            screens[20]) &&
           p.x > 10 && p.x < 230 && p.y > 300 && p.y < 340) {
    Serial.println("BackOpcion2 ON");
    changeActiveScreenTo(0x07);
    initAllScreens();
  }

  // Temperature TextBox
  else if (screens[2] && p.x > 10 && p.x < 230 && p.y > 40 && p.y < 80) {
    // Serial.println("Home ON");
    screens[22] = true;
    screens[23] = false;
    screens[2] = false;
    screens[0] = false;
    initAllScreens();
  }

  // Humidity TextBox
  else if (screens[2] && p.x > 10 && p.x < 230 && p.y > 130 && p.y < 170) {
    // Serial.println("Home ON");
    screens[22] = false;
    screens[23] = true;
    screens[2] = false;
    screens[0] = false;
    initAllScreens();
  }

}

// Print something in the mini status bar with either flashstring
void status(const __FlashStringHelper *msg) {
  tft.fillRect(STATUS_X, STATUS_Y, 240, 8, ILI9341_BLACK);
  tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.print(msg);
}

// or charstring
void status(char *msg) {
  tft.fillRect(STATUS_X, STATUS_Y, 240, 8, ILI9341_BLACK);
  tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.print(msg);
}

void initAllScreens() {
  for (int i = 0; i < (screensTotal - 2); i++) {
    screensInit[i] = false;
  }
}

// cambia la pantalla activa
void changeActiveScreenTo(uint8_t newActiveScreen) {
  for (uint8_t i = 0; i < screensTotal; i++) {
    if (i == newActiveScreen)
      screens[i] = true;
    else if (i != newActiveScreen)
      screens[i] = false;
  }
}

// se fija si cambio la h o la t; si cambio devuelve true
bool hasTHChanged(){
  //float h = dht.readHumidity();
  //float t = dht.readTemperature();

  float h = 50;
  float t = 25;

  float lastH = h;
  float lastT = t;
  //h = dht.readHumidity();
  //t = dht.readTemperature();

  h = 50;
  t = 25;

  if(h != lastH || t != lastT) return true;
  else return false;
}

// se fija si la pantalla está siendo presionada, si está siendo presionada devuelte true y mappea las coordenadas
bool isScreenPressed(){
  TSPoint p = ts.getPoint();
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE){
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);

    Serial.print("(");
    Serial.print(p.x);
    Serial.print(", ");
    Serial.print(p.y);
    Serial.print(", ");
    Serial.print(p.z);
    Serial.println(") ");


    return true;
  }
  else false;
}