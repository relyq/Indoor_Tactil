
#include <DHT.h>
#include <RTClib.h>
#include <SPI.h>
#include <string.h>

#include "src/Adafruit_GFX.h"     // Core graphics library
#include "src/Adafruit_TFTLCD.h"  // Hardware-specific library
#include "src/TouchScreen.h"

#define DHTPIN 31
#define DHTTYPE DHT11

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3  // Chip Select goes to Analog 3
#define LCD_CD A2  // Command/Data goes to Analog 2
#define LCD_WR A1  // LCD Write goes to Analog 1
#define LCD_RD A0  // LCD Read goes to Analog 0
// optional
#define LCD_RESET A4  // Can alternately just connect to Arduino's reset pin

// Color Definitions
#define BLACK 0x0000
#define LIGHTGREY 0xC618
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define NAVY 0x000F
#define DARKGREEN 0x03E0
#define DARKCYAN 0x03EF
#define MAROON 0x7800
#define PURPLE 0x780F
#define OLIVE 0x7BE0
#define DARKGREY 0x7BEF
#define ORANGE 0xFD20
#define GREENYELLOW 0xAFE5
//#define PINK 0xF81F   // redefinition compiler warning
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

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
#define TEXT_TCOLOR MAGENTA
// the data (phone #) we store in the textfield
#define TEXT_LEN 12
char textfield[TEXT_LEN + 1] = "";
uint8_t textfield_i = 0;

#define YP A3  // must be an analog pin, use "An" notation! // LCD CS
#define XM \
  A2  // must be an analog pin, use "An" notation! // LCD RS - COMMAND/DATA
#define YM 9  // can be a digital pin                      // LCD D1
#define XP 8  // can be a digital pin                      // LCD D0

// puntos maximos y minimos de la pantalla tactil, contando el espacio
// no-dibujable
#define TS_MINX 120
#define TS_MINY 75
#define TS_MAXX 900
#define TS_MAXY 950

// We have a status line for like, is FONA working
#define STATUS_X 10
#define STATUS_Y 65

#define MINPRESSURE 10
#define MAXPRESSURE 1000

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
// ???????????????
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 10);

DHT dht(DHTPIN, DHTTYPE);

RTC_DS3231 rtc;

Adafruit_GFX_Button homeButtons[1];
Adafruit_GFX_Button menuButtons[3];
Adafruit_GFX_Button z1Buttons[6];
Adafruit_GFX_Button z1f1Buttons[12];
Adafruit_GFX_Button z1f2Buttons[12];
Adafruit_GFX_Button z1f3Buttons[12];
Adafruit_GFX_Button z1f4Buttons[12];
Adafruit_GFX_Button z1ControlButtons[4];
Adafruit_GFX_Button z1InicioButtons[6];
Adafruit_GFX_Button ajustesButtons[5];
Adafruit_GFX_Button alarmasButtons[1];
Adafruit_GFX_Button relojButtons[2];
Adafruit_GFX_Button programasButtons[1];
Adafruit_GFX_Button resetButtons[1];
Adafruit_GFX_Button numericKeyboardButtons[16];

// estos dos deberian ser char[]
char* currentScreen;  // acá guardo la pantalla activa
char* prevScreen;     // acá guardo la pantalla anterior

uint8_t z1fActiva = 0;
uint8_t z1fActivalast = z1fActiva;
uint8_t z1fSeleccionada =
    z1fActiva;  // fase seleccionada en la pantalla de inicio de fases

bool z1TerminarConfirmar = 0;

//// valores default
char z1f1diasSPstr[4] = "90";
char z1f1hluzSPstr[3] = "16";
char z1f1templSPstr[3] = "20";
char z1f1temphSPstr[3] = "25";
char z1f1riegolSPstr[3] = "20";
char z1f1riegohSPstr[3] = "60";
char z1f1humlSPstr[3] = "60";
char z1f1humhSPstr[3] = "80";

char z1f2diasSPstr[4] = "90";
char z1f2hluzSPstr[3] = "16";
char z1f2templSPstr[3] = "20";
char z1f2temphSPstr[3] = "25";
char z1f2riegolSPstr[3] = "20";
char z1f2riegohSPstr[3] = "60";
char z1f2humlSPstr[3] = "60";
char z1f2humhSPstr[3] = "80";

char z1f3diasSPstr[4] = "90";
char z1f3hluzSPstr[3] = "16";
char z1f3templSPstr[3] = "20";
char z1f3temphSPstr[3] = "25";
char z1f3riegolSPstr[3] = "20";
char z1f3riegohSPstr[3] = "60";
char z1f3humlSPstr[3] = "60";
char z1f3humhSPstr[3] = "80";

char z1f4diasSPstr[4] = "90";
char z1f4hluzSPstr[3] = "16";
char z1f4templSPstr[3] = "20";
char z1f4temphSPstr[3] = "25";
char z1f4riegolSPstr[3] = "20";
char z1f4riegohSPstr[3] = "60";
char z1f4humlSPstr[3] = "60";
char z1f4humhSPstr[3] = "80";

#define templSPdef "20"
#define temphSPdef "30"
#define humlSPdef "60"
#define humhSPdef "80"
#define diasSPdef "90"
#define riegolSPdef "20"
#define riegohSPdef "60"

char templSPstr[3] = templSPdef;
char temphSPstr[3] = temphSPdef;
char humlSPstr[3] = humlSPdef;
char humhSPstr[3] = humhSPdef;
char diasSPstr[4] = diasSPdef;
char riegolSPstr[3] = riegolSPdef;
char riegohSPstr[3] = riegohSPdef;

uint8_t templSP;   // temperatura limite l
uint8_t temphSP;   // temperatura limite h
uint8_t humlSP;    // humedad aire limite l
uint8_t humhSP;    // humedad aire limite h
uint8_t riegolSP;  // riego limite l
uint8_t riegohSP;  // riego limite h
uint16_t diasSP;   // dias de la fase

uint16_t dias;
uint16_t lastdias = 0xffff;

float t;  // temperatura
float h;  // humedad
float lastT = 255;
float lastH = 255;

uint8_t hTierra;  // humedad tierra
uint8_t lasthTierra = 255;

const uint8_t SENSORTIERRAPIN = A8;

const uint8_t FANPIN = 33;
const uint8_t VAPPIN = 35;
const uint8_t RIEGOPIN = 37;

char numKBstr[10];
char* numKBPrevScreen;
char* numKBvarptr;
uint8_t numKBstrLength;
uint8_t numKBbufferSize;

unsigned long time;      // acá guardo el tiempo que lleva el programa
                         // MILLIS() LLEGA A SU OVERFLOW A LOS 50 DIAS
unsigned long lastTime;  // acá guardo el tiempo de programa en el que
// llamé al dht por última vez

const char daysOfTheWeek[7][10] = {"Domingo", "Lunes",   "Martes", "Miercoles",
                                   "Jueves",  "Viernes", "Sabado"};

DateTime now;
uint32_t prevTime;
uint16_t relojYYYY;
uint8_t relojMM;
uint8_t relojDD;
uint8_t relojhh;
uint8_t relojmm;

char buffer[50];

/*

*/