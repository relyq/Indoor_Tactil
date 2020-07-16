#include <DHT.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <SPI.h>
#include <avr/wdt.h>

#include "src/Adafruit_GFX.h"     // Core graphics library
#include "src/Adafruit_TFTLCD.h"  // Hardware-specific library
#include "src/TouchScreen.h"

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
Adafruit_GFX_Button programasButtons[5];
Adafruit_GFX_Button programa1Buttons[5];
Adafruit_GFX_Button programa2Buttons[5];
Adafruit_GFX_Button programa3Buttons[5];
Adafruit_GFX_Button programa4Buttons[5];
Adafruit_GFX_Button resetButtons[2];
Adafruit_GFX_Button numericKeyboardButtons[16];

uint8_t currentScreen;  // acá guardo la pantalla activa
uint8_t prevScreen;     // acá guardo la pantalla anterior

uint8_t z1fActiva;
uint8_t z1fActivalast;
uint8_t z1fSeleccionada;  // fase seleccionada en la pantalla de inicio de fases

bool z1TerminarConfirmar = 0;
// 0
// 1 - cargar
// 2 - guardar
// 3 - reestablecer
uint8_t programasConfirmar = 0;

//// valores default

typedef struct Fase {
  uint16_t dias;
  uint8_t hLuz;
  uint8_t templ;
  uint8_t temph;
  uint8_t riegol;
  uint8_t riegoh;
  uint8_t huml;
  uint8_t humh;
} Fase;

typedef struct Programa {
  Fase f1;
  Fase f2;
  Fase f3;
  Fase f4;
} Programa;

Fase f1;
Fase f2;
Fase f3;
Fase f4;

Fase fActivaSP;

uint32_t diaIniciodefase;  // dia en unixtime del inicio de la fase activa
uint32_t diaFindefase;     // dia en unixtime del fin de la fase activa
uint8_t hLuz;              // horas luz de la fase
uint8_t hInicioLuz;        // hora de inicio de iluminacion
uint8_t hFinLuz;           // hora de fin de iluminacion
uint8_t mInicioFinLuz;     // minuto de inicio/fin de iluminacion
uint8_t ciclos;            // cantidad de ciclos - 0 = ciclo continuo

uint16_t dias = 0;           // dias que lleva la fase activa
uint16_t lastdias = 0xffff;  // esto lo uso para mostrar en la pantalla cuando
                             // cambia el dia que lleva la fase

uint8_t lastLuz = 0;

float t;  // temperatura
float h;  // humedad
float lastT = 255;
float lastH = 255;

uint8_t hTierra;  // humedad tierra
uint8_t lasthTierra = 255;

// ins
const uint8_t SENSORTIERRAPIN PROGMEM = A8;
const uint8_t DHTPIN PROGMEM = 0x40;  // pin 31 en el port c

// outs
const uint8_t HEATPIN PROGMEM = 0x20;   // pin 32 en el port c
const uint8_t FANPIN PROGMEM = 0x10;    // pin 33 en el port c
const uint8_t LUZPIN PROGMEM = 0x08;    // pin 34 en el port c
const uint8_t VAPPIN PROGMEM = 0x04;    // pin 35 en el port c
const uint8_t RIEGOPIN PROGMEM = 0x01;  // pin 37 en el port c

uint8_t PORTCSTATE;

char numKBstr[10];  // aca guardo la str que estoy modificando en el teclado
                    // numerico
uint8_t numKBPrevScreen;  // la pantalla a la que tengo que volver

// puntero a la variable que quiero modificar si es un byte
uint8_t* numKBvarptr8b;
uint16_t* numKBvarptr16b;  // si es de 2 byte. probablemente hay una mejor forma
                           // de hacerlo.
uint8_t numKBstrLength;    // el largo que puede tener la variable - para evitar
                           // overflow
// no estoy seguro para que sirve esto pero es necesario
uint8_t numKBbufferSize;
uint16_t numKBeeprom;

unsigned long time;      // acá guardo el tiempo que lleva el programa
                         // MILLIS() LLEGA A SU OVERFLOW A LOS 50 DIAS
unsigned long lastTime;  // acá guardo el tiempo de programa en el que
// llamé al dht por última vez

const char STR_DDMMYY[] PROGMEM = "DD/MM/YY";
const char STR_hhmm[] PROGMEM = "hh:mm";
const char STR_fdecimal[] PROGMEM = "%d";
const char STR_f02decimal[] PROGMEM = "%02d";
const char STR_percent[] PROGMEM = "%";
const char STR_celsius[] PROGMEM = "C";
const char STR_hum[] PROGMEM = "H";
const char daysOfTheWeek[7][10] PROGMEM = {
    "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};

DateTime now;
uint32_t prevTime;
uint16_t relojYYYY;
uint8_t relojMM;
uint8_t relojDD;
uint8_t relojhh;
uint8_t relojmm;

char buffer[50];  // buffer para mostrar cosas en pantalla

DHT dht(DHTPIN, DHT22);

RTC_DS3231 rtc;

const uint8_t riegoTiempo PROGMEM = 5;  // tiempo que dura la rafaga de riego
uint32_t tRiegoBomba;    // cuando deberia terminar la rafaga - unix timestamp
uint32_t tRiegoEspera;   // cuando deberia terminar la espera - unix timestamp
uint8_t LASTRIEGOSTATE;  // ultimo estado de riego - esto es para actualizar la
                         // luz del dashboard

/*
  screens:
  0 - dashboard/home screen
  1 - menu
  2 - ajustes
  3 - alarmas
  4 - reloj
  5 - programas
  6 - reset
  7 - programa 1
  8 - programa 2
  9 - programa 3
  10 - programa 4

  30 - zona 1 menu
  31 - zona 1 control
  32 - zona 1 inicio
  33 - zona 1 fase 1
  34 - zona 1 fase 2
  35 - zona 1 fase 3
  36 - zona 1 fase 4

  255 - numKB


  EEPROM:
  [0-9] = id, device info, etc
  [10-29] = fase activa, info fase activa, ciclos
    10 = fase activa
    11-14 = UNIX timestamp inicio de fase
    15-18 = UNIX timestamp fin de fase
    19 = hora de inicio de iluminacion
    20 = hora de fin de iluminacion
    21 = minuto de inicio/fin de iluminacion
    22 = ciclo/s
  [30-49] = configuracion fase 1
    30-31 = dias
    32 = horas luz
    33 = temp low
    34 = temp high
    35 = riego low
    36 = riego high
    37 = hum low
    38 = hum high
  [50-69] = configuracion fase 2
  [70-89] = configuracion fase 3
  [90-109] = configuracion fase 4
  [110-189] = programa 1
    [110-129] = programa 1 fase 1
    [130-149] = programa 1 fase 2
    [150-169] = programa 1 fase 3
    [170-189] = programa 1 fase 4
  [210-289] = programa 2
    [210-229] = programa 2 fase 1
    [230-249] = programa 2 fase 2
    [250-269] = programa 2 fase 3
    [270-289] = programa 2 fase 4
  [310-389] = programa 3
    [310-329] = programa 3 fase 1
    [330-349] = programa 3 fase 2
    [350-369] = programa 3 fase 3
    [370-389] = programa 3 fase 4
  [410-489] = programa 4
    [410-429] = programa 4 fase 1
    [430-449] = programa 4 fase 2
    [450-469] = programa 4 fase 3
    [470-489] = programa 4 fase 4
*/