/*
  GROWOS v0.9
*/

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

#define CHARACTER_WIDTH 6   // including space 1px
#define CHARACTER_HEIGHT 7  // not including space 1px
#define BUTTON_TEXTSIZE 3
#define TITLE_TEXTSIZE 2

// touchscreen
#define YP A3  // must be an analog pin // LCD CS
#define XM A2  // must be an analog pin // LCD RS - COMMAND/DATA
#define YM 9   // can be a digital pin  // LCD D1
#define XP 8   // can be a digital pin  // LCD D0

// puntos maximos y minimos de la pantalla tactil, contando el espacio
// no-dibujable
#define TS_MINX 120
#define TS_MINY 75
#define TS_MAXX 900
#define TS_MAXY 950

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
const uint8_t DHTPIN PROGMEM = 0x40;  // pin 31 // PC6

// outs
const uint8_t HEATPIN PROGMEM = 0x20;   // pin 32 // PC5
const uint8_t FANPIN PROGMEM = 0x10;    // pin 33 // PC4
const uint8_t LUZPIN PROGMEM = 0x08;    // pin 34 // PC3
const uint8_t VAPPIN PROGMEM = 0x04;    // pin 35 // PC2
const uint8_t RIEGOPIN PROGMEM = 0x01;  // pin 37 // PC0

/*
// supongo que asi se hace convencionalmente

#define HEATPIN PC5
#define FANPIN PC4
// etc...
*/

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
uint8_t numKBbufferSize;   // digitos maximos de la variable
uint16_t numKBeeprom;      // dir eeprom donde guardar

uint32_t time;      // acá guardo el tiempo que lleva el programa
uint32_t lastTime;  // acá guardo el tiempo de programa en el que
                    // llamé al dht por última vez

// esto es completamente innecesario
const char STR_DDMMYY[] PROGMEM = "DD/MM/YY";
const char STR_hhmm[] PROGMEM = "hh:mm";
const char STR_fdecimal[] PROGMEM = "%d";
const char STR_f02decimal[] PROGMEM = "%02d";
const char STR_percent[] PROGMEM = "%";
const char STR_celsius[] PROGMEM = "C";
const char STR_hum[] PROGMEM = "H";

const char daysOfTheWeek[7][10] = {"Domingo", "Lunes",   "Martes", "Miercoles",
                                   "Jueves",  "Viernes", "Sabado"};

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

uint16_t framecount = 0;  // cuenta cada pasada por loop()
const uint8_t refreshFrames PROGMEM =
    100;  // cantidad de frames que tarda el dashboard en refrescar

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
  [30-109] = programa activo
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

// SETUP

void setup() {
  Serial.begin(9600);

  Serial.print(F("TFT size is "));
  Serial.print(tft.width());
  Serial.print(F("x"));
  Serial.println(tft.height());

  pinMode(13, OUTPUT);

  DDRC |= (HEATPIN | FANPIN | LUZPIN | VAPPIN | RIEGOPIN) & ~(DHTPIN);
  PORTC |= DHTPIN;

  /*
  // supongo que asi se hace convencionalmente

  DDRC |= (1 << PC5); // DDRC |= (1 << HEATPIN); // etc...
  DDRC |= (1 << PC4);
  DDRC |= (1 << PC3);
  DDRC |= (1 << PC2);
  DDRC |= (1 << PC0);

  PORTC |= (1 << PC6);
  */

  pinMode(SENSORTIERRAPIN, INPUT);

  MCUSR = 0;  // clear out any flags of prior resets.

  if (!rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    while (1)
      ;
  }

  if (rtc.lostPower()) {
    Serial.println(F("RTC lost power, lets set the time!"));
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // rtc.adjust(DateTime(1974, 1, 2, 0, 0, 0));
  }

  cargarEstado();  // carga el ultimo estado del dispositivo desde la EEPROM
  cargarFases();   // carga la configuracion de las fases desde la EEPROM

  switch (z1fActiva) {
    case 0:
      fActivaSP.dias = 0;
      fActivaSP.templ = 0;
      fActivaSP.temph = 0;
      fActivaSP.huml = 0;
      fActivaSP.humh = 0;
      fActivaSP.riegol = 0;
      fActivaSP.riegoh = 0;
      break;
    case 1:
      fActivaSP = f1;
      break;
    case 2:
      fActivaSP = f2;
      break;
    case 3:
      fActivaSP = f3;
      break;
    case 4:
      fActivaSP = f4;
      break;
  }

  dht.begin();

  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(0);
  tft.fillScreen(BLACK);

  Serial.println(F("Starting Loop"));

  now = rtc.now();
  prevTime = now.unixtime() + 61;
  drawStartupScreen();
  delay(1500);
  HomeScreen();
}

// LOOP

void loop() {
  now = rtc.now();
  // readTH();
  // hTierra = map(analogRead(A8), 0, 1023, 100, 0);

  DEBUG();

  tsMenu();

  // aca manejo el cambio de fases
  if (z1fActivalast != z1fActiva) {
    Serial.println(F("fase activa cambiada"));
    switch (z1fActiva) {
      case 0:
        fActivaSP.dias = 0;
        fActivaSP.templ = 0;
        fActivaSP.temph = 0;
        fActivaSP.huml = 0;
        fActivaSP.humh = 0;
        fActivaSP.riegol = 0;
        fActivaSP.riegoh = 0;
        break;
      case 1:
        fActivaSP = f1;
        break;
      case 2:
        fActivaSP = f2;
        break;
      case 3:
        fActivaSP = f3;
        break;
      case 4:
        fActivaSP = f4;
        break;
    }

    diaIniciodefase = now.unixtime();
    diaFindefase = now.unixtime() + fActivaSP.dias * 86400;

    hInicioLuz = now.hour();
    mInicioFinLuz = now.minute();
    hFinLuz = (now.unixtime() + (hLuz * 60 * 60)) / 3600 % 24;

    EEPROM.update(10, z1fActiva);
    EEPROM.put(11, diaIniciodefase);
    EEPROM.put(15, diaFindefase);
    EEPROM.update(19, hInicioLuz);
    EEPROM.update(20, hFinLuz);
    EEPROM.update(21, mInicioFinLuz);
    EEPROM.update(22, ciclos);

    Serial.print(F("diaFindefase: "));
    Serial.println(diaFindefase);
    Serial.print(F("unixtime: "));
    Serial.println(now.unixtime());

    Serial.print(F("horas luz: "));
    Serial.println(hLuz);
    Serial.print(F("hora fin de luz: "));
    Serial.print(hFinLuz);
    Serial.print(F(":"));
    Serial.println(mInicioFinLuz);

    z1fActivalast = z1fActiva;

    if (currentScreen == 0) {
      HomeScreen();
    }
  }

  // funcionalidad
  if (z1fActiva != 0) {
    PORTCSTATE = PINC;

    if (t >= fActivaSP.temph) {
      PORTCSTATE &= ~HEATPIN;
      PORTCSTATE |= FANPIN;
    } else if (t <= fActivaSP.templ) {
      PORTCSTATE &= ~FANPIN;
      PORTCSTATE |= HEATPIN;
    } else if (t <= ((float)fActivaSP.temph + (float)fActivaSP.templ) / 2) {
      PORTCSTATE &= ~FANPIN;
    } else if (t >= ((float)fActivaSP.temph + (float)fActivaSP.templ) / 2) {
      PORTCSTATE &= ~HEATPIN;
    }

    if (h >= fActivaSP.humh) {
      PORTCSTATE &= ~VAPPIN;
      PORTCSTATE |= FANPIN;
    } else if (h <= fActivaSP.huml) {
      // PORTCSTATE &= ~FANPIN;
      PORTCSTATE |= VAPPIN;
    } else if (h <= ((float)fActivaSP.humh + (float)fActivaSP.huml) / 2) {
      // PORTCSTATE &= ~FANPIN;
    } else if (h >= ((float)fActivaSP.humh + (float)fActivaSP.huml) / 2) {
      PORTCSTATE &= ~VAPPIN;
    }

    if (hTierra <= fActivaSP.riegol && (tRiegoEspera + tRiegoBomba) == 0) {
      tRiegoBomba = now.unixtime() + riegoTiempo;
      PORTCSTATE |= RIEGOPIN;
      Serial.println(F("tRiegoBomba sobreescrito"));
    } else if (hTierra >= fActivaSP.riegoh) {
      tRiegoEspera = 0;
      tRiegoBomba = 0;
      PORTCSTATE &= ~RIEGOPIN;
    }

    if (tRiegoBomba && !tRiegoEspera) {
      if (now.unixtime() >= tRiegoBomba) {
        tRiegoBomba = 0;
        tRiegoEspera = now.unixtime() + riegoTiempo * 6;  // tiempo apagado
        PORTCSTATE &= ~RIEGOPIN;
      }
    }

    if (tRiegoEspera && !tRiegoBomba) {
      if (now.unixtime() >= tRiegoEspera) {
        tRiegoEspera = 0;
        tRiegoBomba = now.unixtime() + riegoTiempo;  // tiempo encendido
        PORTCSTATE |= RIEGOPIN;
      }
    }

    // Serial.print("PINC = 0x");
    // Serial.println(PINC, HEX);
    // Serial.print("PORTCSTATE = 0x");
    // Serial.println(PORTCSTATE, HEX);

    if (PINC != PORTCSTATE) {
      PORTC = PORTCSTATE;
      Serial.println(F("portc actualizado"));
      Serial.print(F("tRiegoEspera = "));
      Serial.println(tRiegoEspera);
      Serial.print(F("tRiegoBomba = "));
      Serial.println(tRiegoBomba);
      Serial.print(F("PIN37 = "));
      Serial.println(PINC & RIEGOPIN);
    }

    // si la hora esta entre la hora de inicio de luz y la hora de fin de luz, y
    // la luz no esta prendida
    if (now.hour() > hInicioLuz ||
        (now.hour() == hInicioLuz && now.minute() >= mInicioFinLuz)) {
      if (!(PINC & LUZPIN)) {
        Serial.println(F("luz encendida"));
        PORTC |= LUZPIN;
      }
    } else if (now.hour() < hFinLuz ||
               (now.hour() == hFinLuz && now.minute() < mInicioFinLuz)) {
      if (!(PINC & LUZPIN)) {
        Serial.println(F("luz encendida"));
        PORTC |= LUZPIN;
      }
    } else {
      if (PINC & LUZPIN) {
        Serial.println(F("luz apagada"));
        PORTC &= ~LUZPIN;
      }
    }

    if (now.unixtime() >= diaFindefase) {
      if (z1fActiva == 4) {
        if (ciclos == 0) {
          z1fActiva = 1;
        } else if (ciclos == 1) {
          z1fActiva = 0;
        } else {
          ciclos--;
          z1fActiva = 1;
        }
      } else {
        z1fActiva++;
      }
    }

    dias = (now.unixtime() - diaIniciodefase) / 86400;
  } else if (z1fActiva == 0) {
    PORTC &= ~(FANPIN | HEATPIN | VAPPIN | RIEGOPIN);
  }

  // aca actualizo el dashboard
  if (currentScreen == 0 && !(framecount % refreshFrames)) {
    // Serial.print(F("Framecount: "));
    // Serial.print(framecount);
    // Serial.print(F("\n"));

    if (now.second() == 0 && now.unixtime() - prevTime >= 2) {
      prevTime = now.unixtime();
      Serial.print(now.year(), DEC);
      Serial.print(F("/"));
      Serial.print(now.month(), DEC);
      Serial.print(F("/"));
      Serial.print(now.day(), DEC);
      Serial.print(F(" ("));
      Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
      Serial.print(F(") "));
      Serial.print(now.hour(), DEC);
      Serial.print(F(":"));
      Serial.print(now.minute(), DEC);
      Serial.print(F(":"));
      Serial.print(now.second(), DEC);
      Serial.println();

      strcpy_P(buffer, STR_hhmm);
      now.toString(buffer);
      tft.setTextSize(2);
      tft.setTextColor(WHITE, BLACK);
      tft.setCursor(170, 165);
      tft.print(buffer);

      strcpy_P(buffer, STR_DDMMYY);
      now.toString(buffer);
      tft.setCursor(134, 183);
      tft.print(buffer);
    }

    if (lastLuz != (PINC & LUZPIN)) {
      lastLuz = (PINC & LUZPIN);
      Serial.print(F("lastLuz: "));
      Serial.println(lastLuz);
      if (PINC & LUZPIN) {
        tft.fillCircle(180, 69, 10, GREEN);
      } else {
        tft.fillCircle(180, 69, 10, LIGHTGREY);
      }
    }

    if (lastdias != dias) {
      lastdias = dias;
      sprintf_P(buffer, STR_fdecimal, dias);
      tft.setCursor(125 - (strlen(buffer) * 18), 230);
      tft.setTextSize(3);
      tft.setTextColor(WHITE, BLACK);
      tft.print(buffer);
    }

    if (lasthTierra != hTierra || LASTRIEGOSTATE != (PINC & RIEGOPIN)) {
      lasthTierra = hTierra;
      sprintf_P(buffer, STR_fdecimal, hTierra);
      strcat_P(buffer, STR_percent);
      tft.setCursor(230 - (strlen(buffer) * 18), 230);
      tft.setTextSize(3);
      tft.setTextColor(WHITE);
      tft.print(buffer);
      LASTRIEGOSTATE = (PINC & RIEGOPIN);

      if (PINC & RIEGOPIN) {
        tft.fillCircle(180, 144, 10, GREEN);
      } else {
        tft.fillCircle(180, 144, 10, LIGHTGREY);
      }
    }

    if (lastT != t) {
      lastT = t;
      dtostrf(t, 4, 1, buffer);
      strcat_P(buffer, STR_celsius);
      tft.setCursor(125 - (strlen(buffer) * 18), 285);
      tft.setTextSize(3);
      tft.setTextColor(WHITE, BLACK);
      tft.print(buffer);  // temperatura leida por el DHT

      if (PINC & FANPIN && !(PINC & HEATPIN)) {
        tft.fillCircle(180, 94, 10, BLUE);
      } else if (PINC & HEATPIN && !(PINC & FANPIN)) {
        tft.fillCircle(180, 94, 10, YELLOW);
      } else if (!(PINC & HEATPIN) && !(PINC & FANPIN)) {
        tft.fillCircle(180, 94, 10, LIGHTGREY);
      }
    }

    if (lastH != h) {
      lastH = h;
      sprintf_P(buffer, STR_fdecimal, (uint8_t)h);
      strcat_P(buffer, STR_percent);
      tft.setCursor(230 - (strlen(buffer) * 18), 285);
      tft.setTextSize(3);
      tft.setTextColor(WHITE, BLACK);
      tft.print(buffer);  // humedad leida por el DHT
      if (PINC & VAPPIN) {
        tft.fillCircle(180, 119, 10, GREEN);
      } else {
        tft.fillCircle(180, 119, 10, LIGHTGREY);
      }
    }
  }
  // aca actualizo la hora en todas las pantallas excepto dashboard y numpad
  if ((currentScreen != 0 && currentScreen != 255) &&
      (now.second() == 0 && now.unixtime() - prevTime >= 2)) {
    strcpy_P(buffer, STR_hhmm);
    now.toString(buffer);
    tft.setTextSize(2);
    tft.setTextColor(WHITE, BLACK);
    tft.setCursor(170, 10);
    tft.print(buffer);
  }

  framecount++;
}

void readTH() {
  time = millis();
  if (time - lastTime >= 2000) {
    lastT = t;
    lastH = h;
    t = dht.readTemperature();
    h = dht.readHumidity();
    lastTime = millis();
  }
}

void DEBUG() {
  char msg[5] = {0, 0, 0, 0, '\0'};
  char lastmsg[5] = {0, 0, 0, 0, '\0'};

  if (Serial.available() > 0) {
    (Serial.readString()).toCharArray(msg, 5);
  }

  if (!(strcmp(msg, lastmsg) == 0)) {
    char msgval[4];
    uint8_t val = 0;

    strcpy(msgval, msg + 1);

    switch (msg[0]) {
      case 'T':  // temp
        val = (msgval[0] - '0') * 10;
        val += msgval[1] - '0';
        t = val;
        break;
      case 'H':  // hum
        val = (msgval[0] - '0') * 10;
        val += msgval[1] - '0';
        h = val;
        break;
      case 'M':  // moisture
        val = (msgval[0] - '0') * 10;
        val += msgval[1] - '0';
        hTierra = val;
        break;
      case 'E':  // eeprom
        if (!strcmp_P(msg, PSTR("ERDD"))) {
          eeprom_read();
        } else if (!strcmp_P(msg, PSTR("ECLR"))) {
          eeprom_clear();
        } else if (!strcmp_P(msg, PSTR("ERST"))) {
          eeprom_hardReset();
        }
        break;
    }

    Serial.print(msg[0]);
    Serial.println(val);

    strcpy(lastmsg, msg);
  }
}

// TSMENU

void tsMenu() {
  TSPoint p = ts.getPoint();

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  // pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  // pinMode(YM, OUTPUT);

  // si la pantalla está siendo presionada se mappea el resultado de getPoint()
  // a un punto valido del cursor en la pantalla
  // no es necesario refrescar la pantalla si no cambió nada ni está siendo
  // presionada
  if (p.z > MINPRESSURE) {
    // scale from 0->1023 to tft.width
    Serial.print(F("Unmapped p: "));
    Serial.print(F("("));
    Serial.print(p.x);
    Serial.print(F(", "));
    Serial.print(p.y);
    Serial.print(F(", "));
    Serial.print(p.z);
    Serial.print(F(") "));

    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY - 60, tft.height(), 0);
    // if the screen is being touched show cursor position

    Serial.print(F("Mapped p: "));
    Serial.print(F("("));
    Serial.print(p.x);
    Serial.print(F(", "));
    Serial.print(p.y);
    Serial.print(F(", "));
    Serial.print(p.z);
    Serial.println(F(") "));

    if (currentScreen != 0 && (p.y < -1)) {
      HomeScreen();
    }

    switch (currentScreen) {
      case 0:
        if (homeButtons[0].contains(p.x, p.y)) {
          MenuScreen();
        }
        break;
      case 1:
        if (menuButtons[0].contains(p.x, p.y)) {
          Z1Screen();
        } else if (menuButtons[1].contains(p.x, p.y)) {
          AjustesScreen();
        } else if (menuButtons[2].contains(p.x, p.y)) {
          HomeScreen();
        }
        break;
      case 2:
        if (ajustesButtons[4].contains(p.x, p.y)) {
          MenuScreen();
        } else if (ajustesButtons[0].contains(p.x, p.y)) {
          AlarmasScreen();
        } else if (ajustesButtons[1].contains(p.x, p.y)) {
          RelojScreen();
        } else if (ajustesButtons[2].contains(p.x, p.y)) {
          ProgramasScreen();
        } else if (ajustesButtons[3].contains(p.x, p.y)) {
          ResetScreen();
        }
        break;
      case 3:
        if (alarmasButtons[0].contains(p.x, p.y)) {
          AjustesScreen();
        }
        break;
      case 4:
        tft.setTextSize(3);
        tft.setTextColor(WHITE, BLACK);
        if (relojButtons[0].contains(p.x, p.y)) {
          AjustesScreen();
        } else if (relojButtons[1].contains(p.x, p.y)) {
          rtc.adjust(
              DateTime(relojYYYY, relojMM, relojDD, relojhh, relojmm, 0));
          now = rtc.now();
          AjustesScreen();
        } else if ((p.x > 39 && p.x < 83) && (p.y > 24 && p.y < 60)) {
          relojYYYY++;
          sprintf_P(buffer, STR_fdecimal, relojYYYY);
          tft.setCursor(28, 60);
          tft.print(buffer);
          if (relojMM == 2) {
            if ((relojYYYY % 4 == 0 && relojYYYY % 100 != 0) ||
                relojYYYY % 400 == 0) {
              if (relojDD > 29) {
                relojDD = 29;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
            } else {
              if (relojDD > 28) {
                relojDD = 28;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
            }
          }
        } else if ((p.x > 39 && p.x < 83) && (p.y > 81 && p.y < 114)) {
          if (relojYYYY > 1970) {
            relojYYYY--;
            sprintf_P(buffer, STR_fdecimal, relojYYYY);
            tft.setCursor(28, 60);
            tft.print(buffer);
          }
        } else if ((p.x > 114 && p.x < 158) && (p.y > 24 && p.y < 60)) {
          if (relojMM < 12) {
            relojMM++;
            sprintf_P(buffer, STR_f02decimal, relojMM);
            tft.setCursor(28 + 72 + 20, 60);
            tft.print(buffer);
          } else {
            relojMM = 1;
            sprintf_P(buffer, STR_f02decimal, relojMM);
            tft.setCursor(28 + 72 + 20, 60);
            tft.print(buffer);
          }
          switch (relojMM) {
            case 4:
            case 6:
            case 9:
            case 11:
              if (relojDD > 30) {
                relojDD = 30;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 2:
              if ((relojYYYY % 4 == 0 && relojYYYY % 100 != 0) ||
                  relojYYYY % 400 == 0) {
                if (relojDD > 29) {
                  relojDD = 29;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              } else {
                if (relojDD > 28) {
                  relojDD = 28;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              }
              break;
          }
        } else if ((p.x > 114 && p.x < 158) && (p.y > 81 && p.y < 114)) {
          if (relojMM > 1) {
            relojMM--;
            sprintf_P(buffer, STR_f02decimal, relojMM);
            tft.setCursor(28 + 72 + 20, 60);
            tft.print(buffer);
          } else {
            relojMM = 12;
            sprintf_P(buffer, STR_f02decimal, relojMM);
            tft.setCursor(28 + 72 + 20, 60);
            tft.print(buffer);
          }
          switch (relojMM) {
            case 4:
            case 6:
            case 9:
            case 11:
              if (relojDD > 30) {
                relojDD = 30;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 2:
              if ((relojYYYY % 4 == 0 && relojYYYY % 100 != 0) ||
                  relojYYYY % 400 == 0) {
                if (relojDD > 29) {
                  relojDD = 29;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              } else {
                if (relojDD > 28) {
                  relojDD = 28;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              }
              break;
          }
        } else if ((p.x > 170 && p.x < 214) && (p.y > 24 && p.y < 60)) {
          switch (relojMM) {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
              if (relojDD < 31) {
                relojDD++;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              } else {
                relojDD = 1;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 4:
            case 6:
            case 9:
            case 11:
              if (relojDD < 30) {
                relojDD++;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              } else {
                relojDD = 1;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 2:
              if ((relojYYYY % 4 == 0 && relojYYYY % 100 != 0) ||
                  relojYYYY % 400 == 0) {
                if (relojDD < 29) {
                  relojDD++;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                } else {
                  relojDD = 1;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              } else {
                if (relojDD < 28) {
                  relojDD++;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                } else {
                  relojDD = 1;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              }
              break;
          }
        } else if ((p.x > 170 && p.x < 214) && (p.y > 81 && p.y < 114)) {
          switch (relojMM) {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
              if (relojDD > 1) {
                relojDD--;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              } else {
                relojDD = 31;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 4:
            case 6:
            case 9:
            case 11:
              if (relojDD > 1) {
                relojDD--;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              } else {
                relojDD = 30;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 2:
              if ((relojYYYY % 4 == 0 && relojYYYY % 100 != 0) ||
                  relojYYYY % 400 == 0) {
                if (relojDD > 1) {
                  relojDD--;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                } else {
                  relojDD = 29;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              } else {
                if (relojDD > 1) {
                  relojDD--;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                } else {
                  relojDD = 28;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              }
              break;
          }
        } else if ((p.x > 67 && p.x < 111) && (p.y > 116 && p.y < 150)) {
          if (relojhh < 23) {
            relojhh++;
            sprintf_P(buffer, STR_f02decimal, relojhh);
            tft.setCursor(74, 150);
            tft.print(buffer);
          } else {
            relojhh = 0;
            sprintf_P(buffer, STR_f02decimal, relojhh);
            tft.setCursor(74, 150);
            tft.print(buffer);
          }
        } else if ((p.x > 67 && p.x < 111) && (p.y > 171 && p.y < 207)) {
          if (relojhh > 0) {
            relojhh--;
            sprintf_P(buffer, STR_f02decimal, relojhh);
            tft.setCursor(74, 150);
            tft.print(buffer);
          } else {
            relojhh = 23;
            sprintf_P(buffer, STR_f02decimal, relojhh);
            tft.setCursor(74, 150);
            tft.print(buffer);
          }
        } else if ((p.x > 124 && p.x < 168) && (p.y > 116 && p.y < 150)) {
          if (relojmm < 59) {
            relojmm++;
            sprintf_P(buffer, STR_f02decimal, relojmm);
            tft.setCursor(74 + 36 + 20, 150);
            tft.print(buffer);
          } else {
            relojmm = 0;
            sprintf_P(buffer, STR_f02decimal, relojmm);
            tft.setCursor(74 + 36 + 20, 150);
            tft.print(buffer);
          }
        } else if ((p.x > 124 && p.x < 168) && (p.y > 171 && p.y < 207)) {
          if (relojmm > 0) {
            relojmm--;
            sprintf_P(buffer, STR_f02decimal, relojmm);
            tft.setCursor(74 + 36 + 20, 150);
            tft.print(buffer);
          } else {
            relojmm = 59;
            sprintf_P(buffer, STR_f02decimal, relojmm);
            tft.setCursor(74 + 36 + 20, 150);
            tft.print(buffer);
          }
        }
        delay(150);
        break;
      case 5:
        if (programasButtons[0].contains(p.x, p.y)) {
          AjustesScreen();
        } else if (programasButtons[1].contains(p.x, p.y)) {
          Programa1Screen();
        } else if (programasButtons[2].contains(p.x, p.y)) {
          Programa2Screen();
        } else if (programasButtons[3].contains(p.x, p.y)) {
          Programa3Screen();
        } else if (programasButtons[4].contains(p.x, p.y)) {
          Programa4Screen();
        }
        break;
      case 6:
        if (resetButtons[0].contains(p.x, p.y)) {
          AjustesScreen();
        } else if (resetButtons[1].contains(p.x, p.y)) {
          eeprom_hardReset();
        }
        break;
      case 7:
        if (programa1Buttons[0].contains(p.x, p.y)) {
          programasConfirmar = 0;
          ProgramasScreen();
        } else if (programa1Buttons[1].contains(p.x, p.y) &&
                   programasConfirmar != 1) {
          programa1Buttons[4].drawButton();
          programasConfirmar = 1;
        } else if (programa1Buttons[2].contains(p.x, p.y) &&
                   programasConfirmar != 2) {
          programa1Buttons[4].drawButton();
          programasConfirmar = 2;
        } else if (programa1Buttons[4].contains(p.x, p.y) &&
                   programasConfirmar) {
          switch (programasConfirmar) {
            case 1:  // cargar
              Programa p1;
              EEPROM.get(110, p1.f1);
              EEPROM.get(130, p1.f2);
              EEPROM.get(150, p1.f3);
              EEPROM.get(170, p1.f4);

              EEPROM.put(30, p1.f1);
              EEPROM.put(50, p1.f2);
              EEPROM.put(70, p1.f3);
              EEPROM.put(90, p1.f4);

              cargarFases();

              Serial.println(F("Programa 1 cargado"));
              break;
            case 2:  // guardar
              EEPROM.put(110, f1);
              EEPROM.put(130, f2);
              EEPROM.put(150, f3);
              EEPROM.put(170, f4);

              Serial.println(F("Programa 1 guardado"));
              break;
          }
          programasConfirmar = 0;
          tft.fillRect(5, 170, 230, 40, BLACK);
        }
        break;
      case 8:
        if (programa2Buttons[0].contains(p.x, p.y)) {
          programasConfirmar = 0;
          ProgramasScreen();
        } else if (programa2Buttons[1].contains(p.x, p.y) &&
                   programasConfirmar != 1) {
          programa2Buttons[4].drawButton();
          programasConfirmar = 1;
        } else if (programa2Buttons[2].contains(p.x, p.y) &&
                   programasConfirmar != 2) {
          programa2Buttons[4].drawButton();
          programasConfirmar = 2;
        } else if (programa2Buttons[4].contains(p.x, p.y) &&
                   programasConfirmar) {
          switch (programasConfirmar) {
            case 1:  // cargar
              Programa p2;
              EEPROM.get(210, p2.f1);
              EEPROM.get(230, p2.f2);
              EEPROM.get(250, p2.f3);
              EEPROM.get(270, p2.f4);

              EEPROM.put(30, p2.f1);
              EEPROM.put(50, p2.f2);
              EEPROM.put(70, p2.f3);
              EEPROM.put(90, p2.f4);

              cargarFases();

              Serial.println(F("Programa 2 cargado"));
              break;
            case 2:  // guardar
              EEPROM.put(210, f1);
              EEPROM.put(230, f2);
              EEPROM.put(250, f3);
              EEPROM.put(270, f4);

              Serial.println(F("Programa 2 guardado"));
              break;
          }
          programasConfirmar = 0;
          tft.fillRect(5, 170, 230, 40, BLACK);
        }
        break;
      case 9:
        if (programa3Buttons[0].contains(p.x, p.y)) {
          programasConfirmar = 0;
          ProgramasScreen();
        } else if (programa3Buttons[1].contains(p.x, p.y) &&
                   programasConfirmar != 1) {
          programa3Buttons[4].drawButton();
          programasConfirmar = 1;
        } else if (programa3Buttons[2].contains(p.x, p.y) &&
                   programasConfirmar != 2) {
          programa3Buttons[4].drawButton();
          programasConfirmar = 2;
        } else if (programa3Buttons[4].contains(p.x, p.y) &&
                   programasConfirmar) {
          switch (programasConfirmar) {
            case 1:  // cargar
              Programa p3;
              EEPROM.get(310, p3.f1);
              EEPROM.get(330, p3.f2);
              EEPROM.get(350, p3.f3);
              EEPROM.get(370, p3.f4);

              EEPROM.put(30, p3.f1);
              EEPROM.put(50, p3.f2);
              EEPROM.put(70, p3.f3);
              EEPROM.put(90, p3.f4);

              cargarFases();

              Serial.println(F("Programa 3 cargado"));
              break;
            case 2:  // guardar
              EEPROM.put(310, f1);
              EEPROM.put(330, f2);
              EEPROM.put(350, f3);
              EEPROM.put(370, f4);

              Serial.println(F("Programa 3 guardado"));
              break;
          }
          programasConfirmar = 0;
          tft.fillRect(5, 170, 230, 40, BLACK);
        }
        break;
      case 10:
        if (programa4Buttons[0].contains(p.x, p.y)) {
          programasConfirmar = 0;
          ProgramasScreen();
        } else if (programa4Buttons[1].contains(p.x, p.y) &&
                   programasConfirmar != 1) {
          programa4Buttons[4].drawButton();
          programasConfirmar = 1;
        } else if (programa4Buttons[2].contains(p.x, p.y) &&
                   programasConfirmar != 2) {
          programa4Buttons[4].drawButton();
          programasConfirmar = 2;
        } else if (programa4Buttons[4].contains(p.x, p.y) &&
                   programasConfirmar) {
          switch (programasConfirmar) {
            case 1:  // cargar
              Programa p4;
              EEPROM.get(410, p4.f1);
              EEPROM.get(430, p4.f2);
              EEPROM.get(450, p4.f3);
              EEPROM.get(470, p4.f4);

              EEPROM.put(30, p4.f1);
              EEPROM.put(50, p4.f2);
              EEPROM.put(70, p4.f3);
              EEPROM.put(90, p4.f4);

              cargarFases();

              Serial.println(F("Programa 4 cargado"));
              break;
            case 2:  // guardar
              EEPROM.put(410, f1);
              EEPROM.put(430, f2);
              EEPROM.put(450, f3);
              EEPROM.put(470, f4);

              Serial.println(F("Programa 4 guardado"));
              break;
          }
          programasConfirmar = 0;
          tft.fillRect(5, 170, 230, 40, BLACK);
        }
        break;
      case 30:
        if (z1Buttons[5].contains(p.x, p.y)) {
          MenuScreen();
        } else if (z1Buttons[0].contains(p.x, p.y)) {
          Z1F1Screen();
        } else if (z1Buttons[1].contains(p.x, p.y)) {
          Z1F2Screen();
        } else if (z1Buttons[2].contains(p.x, p.y)) {
          Z1F3Screen();
        } else if (z1Buttons[3].contains(p.x, p.y)) {
          Z1F4Screen();
        } else if (z1Buttons[4].contains(p.x, p.y)) {
          Z1ControlScreen();
        }
        break;
      case 31:
        if (z1ControlButtons[2].contains(p.x, p.y)) {
          z1TerminarConfirmar = 0;
          Z1Screen();
        } else if (z1ControlButtons[0].contains(p.x, p.y)) {
          z1TerminarConfirmar = 0;
          Z1InicioScreen();
        } else if (z1ControlButtons[1].contains(p.x, p.y) &&
                   z1TerminarConfirmar == 0) {
          z1ControlButtons[3].drawButton();
          z1TerminarConfirmar = 1;
        } else if ((p.x > 170 && p.x < 235) && (p.y > 125 && p.y < 165)) {
          z1TerminarConfirmar = 0;
          NumericKeyboardScreen(&ciclos, 22, 2, "Ciclos");
        } else if (z1TerminarConfirmar == 1 &&
                   z1ControlButtons[3].contains(p.x, p.y)) {
          for (uint8_t i = 10; i < 29; i++) {
            EEPROM.update(i, 0x00);
          }
          EEPROM.update(22, ciclos);
          Serial.println(F("eeprom 10 to 29 cleared to 0x00\n"));
          eeprom_read();
          z1fActiva = 0;

          z1TerminarConfirmar = 0;
          tft.fillRect(5, 170, 230, 40, BLACK);
        }
        break;
      case 32:
        if (z1InicioButtons[5].contains(p.x, p.y)) {
          Z1ControlScreen();
        } else if (z1InicioButtons[0].contains(p.x, p.y) &&
                   (z1fActiva != 1 && z1fSeleccionada != 1)) {
          z1fSeleccionada = 1;
          z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, YELLOW,
                                          WHITE, "1", BUTTON_TEXTSIZE);

          z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE,
                                          DARKGREY, WHITE, "2",
                                          BUTTON_TEXTSIZE);

          z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE,
                                          DARKGREY, WHITE, "3",
                                          BUTTON_TEXTSIZE);

          z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                          DARKGREY, WHITE, "4",
                                          BUTTON_TEXTSIZE);
          switch (z1fActiva) {
            break;
            case 2:
              z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE,
                                              OLIVE, WHITE, "2",
                                              BUTTON_TEXTSIZE);
              break;
            case 3:
              z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE,
                                              OLIVE, WHITE, "3",
                                              BUTTON_TEXTSIZE);
              break;
            case 4:
              z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                              OLIVE, WHITE, "4",
                                              BUTTON_TEXTSIZE);
              break;
          }
          z1InicioButtons[0].drawRectButton();
          z1InicioButtons[1].drawRectButton();
          z1InicioButtons[2].drawRectButton();
          z1InicioButtons[3].drawRectButton();

        } else if (z1InicioButtons[1].contains(p.x, p.y) &&
                   (z1fActiva != 2 && z1fSeleccionada != 2)) {
          z1fSeleccionada = 2;
          z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE, YELLOW,
                                          WHITE, "2", BUTTON_TEXTSIZE);

          z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, DARKGREY,
                                          WHITE, "1", BUTTON_TEXTSIZE);

          z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE,
                                          DARKGREY, WHITE, "3",
                                          BUTTON_TEXTSIZE);

          z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                          DARKGREY, WHITE, "4",
                                          BUTTON_TEXTSIZE);

          switch (z1fActiva) {
            case 1:
              z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE,
                                              OLIVE, WHITE, "1",
                                              BUTTON_TEXTSIZE);
              break;
            case 3:
              z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE,
                                              OLIVE, WHITE, "3",
                                              BUTTON_TEXTSIZE);
              break;
            case 4:
              z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                              OLIVE, WHITE, "4",
                                              BUTTON_TEXTSIZE);
              break;
          }
          z1InicioButtons[0].drawRectButton();
          z1InicioButtons[1].drawRectButton();
          z1InicioButtons[2].drawRectButton();
          z1InicioButtons[3].drawRectButton();

        } else if (z1InicioButtons[2].contains(p.x, p.y) &&
                   (z1fActiva != 3 && z1fSeleccionada != 3)) {
          z1fSeleccionada = 3;
          z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE, YELLOW,
                                          WHITE, "3", BUTTON_TEXTSIZE);

          z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, DARKGREY,
                                          WHITE, "1", BUTTON_TEXTSIZE);

          z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE,
                                          DARKGREY, WHITE, "2",
                                          BUTTON_TEXTSIZE);

          z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                          DARKGREY, WHITE, "4",
                                          BUTTON_TEXTSIZE);

          switch (z1fActiva) {
            case 1:
              z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE,
                                              OLIVE, WHITE, "1",
                                              BUTTON_TEXTSIZE);
              break;
            case 2:
              z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE,
                                              OLIVE, WHITE, "2",
                                              BUTTON_TEXTSIZE);
              break;
            case 4:
              z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                              OLIVE, WHITE, "4",
                                              BUTTON_TEXTSIZE);
              break;
          }
          z1InicioButtons[0].drawRectButton();
          z1InicioButtons[1].drawRectButton();
          z1InicioButtons[2].drawRectButton();
          z1InicioButtons[3].drawRectButton();

        } else if (z1InicioButtons[3].contains(p.x, p.y) &&
                   (z1fActiva != 4 && z1fSeleccionada != 4)) {
          z1fSeleccionada = 4;
          z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE, YELLOW,
                                          WHITE, "4", BUTTON_TEXTSIZE);

          z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, DARKGREY,
                                          WHITE, "1", BUTTON_TEXTSIZE);

          z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE,
                                          DARKGREY, WHITE, "2",
                                          BUTTON_TEXTSIZE);

          z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE,
                                          DARKGREY, WHITE, "3",
                                          BUTTON_TEXTSIZE);

          switch (z1fActiva) {
            case 1:
              z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE,
                                              OLIVE, WHITE, "1",
                                              BUTTON_TEXTSIZE);
              break;
            case 2:
              z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE,
                                              OLIVE, WHITE, "2",
                                              BUTTON_TEXTSIZE);
              break;
            case 3:
              z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE,
                                              OLIVE, WHITE, "3",
                                              BUTTON_TEXTSIZE);
              break;
          }
          z1InicioButtons[0].drawRectButton();
          z1InicioButtons[1].drawRectButton();
          z1InicioButtons[2].drawRectButton();
          z1InicioButtons[3].drawRectButton();

        } else if (z1InicioButtons[4].contains(p.x, p.y) &&
                   (z1fActiva != z1fSeleccionada)) {
          z1fActiva = z1fSeleccionada;
          z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, DARKGREY,
                                          WHITE, "1", BUTTON_TEXTSIZE);

          z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE,
                                          DARKGREY, WHITE, "2",
                                          BUTTON_TEXTSIZE);

          z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE,
                                          DARKGREY, WHITE, "3",
                                          BUTTON_TEXTSIZE);

          z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                          DARKGREY, WHITE, "4",
                                          BUTTON_TEXTSIZE);
          switch (z1fActiva) {
            case 1:
              z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE,
                                              OLIVE, WHITE, "1",
                                              BUTTON_TEXTSIZE);
              break;
            case 2:
              z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE,
                                              OLIVE, WHITE, "2",
                                              BUTTON_TEXTSIZE);
              break;
            case 3:
              z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE,
                                              OLIVE, WHITE, "3",
                                              BUTTON_TEXTSIZE);
              break;
            case 4:
              z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                              OLIVE, WHITE, "4",
                                              BUTTON_TEXTSIZE);
              break;
          }
          z1InicioButtons[0].drawRectButton();
          z1InicioButtons[1].drawRectButton();
          z1InicioButtons[2].drawRectButton();
          z1InicioButtons[3].drawRectButton();
        }
        break;
      case 33:
        if (z1f1Buttons[4].contains(p.x, p.y)) {
          Z1Screen();
        } else if (z1f1Buttons[0].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f1.dias, 30, 3, "Dias");
        } else if (z1f1Buttons[1].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f1.hLuz, 32, 2, "Horas luz");
        } else if (z1f1Buttons[6].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f1.templ, 33, 2, "Temp baja");
        } else if (z1f1Buttons[5].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f1.temph, 34, 2, "Temp alta");
        } else if (z1f1Buttons[8].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f1.riegol, 35, 2, "Riego bajo");
        } else if (z1f1Buttons[7].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f1.riegoh, 36, 2, "Riego alto");
        } else if (z1f1Buttons[11].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f1.huml, 37, 2, "Hum baja");
        } else if (z1f1Buttons[10].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f1.humh, 38, 2, "Hum alta");
        }

        break;
      case 34:
        if (z1f2Buttons[4].contains(p.x, p.y)) {
          Z1Screen();
        } else if (z1f2Buttons[0].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f2.dias, 50, 3, "Dias");
        } else if (z1f2Buttons[1].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f2.hLuz, 52, 2, "Horas luz");
        } else if (z1f2Buttons[6].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f2.templ, 53, 2, "Temp baja");
        } else if (z1f2Buttons[5].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f2.temph, 54, 2, "Temp alta");
        } else if (z1f2Buttons[8].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f2.riegol, 55, 2, "Riego bajo");
        } else if (z1f2Buttons[7].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f2.riegoh, 56, 2, "Riego alto");
        } else if (z1f2Buttons[11].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f2.huml, 57, 2, "Hum baja");
        } else if (z1f2Buttons[10].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f2.humh, 58, 2, "Hum alta");
        }
        break;
      case 35:
        if (z1f3Buttons[4].contains(p.x, p.y)) {
          Z1Screen();
        } else if (z1f3Buttons[0].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f3.dias, 70, 3, "Dias");
        } else if (z1f3Buttons[1].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f3.hLuz, 72, 2, "Horas luz");
        } else if (z1f3Buttons[6].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f3.templ, 73, 2, "Temp baja");
        } else if (z1f3Buttons[5].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f3.temph, 74, 2, "Temp alta");
        } else if (z1f3Buttons[8].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f3.riegol, 75, 2, "Riego bajo");
        } else if (z1f3Buttons[7].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f3.riegoh, 76, 2, "Riego alto");
        } else if (z1f3Buttons[11].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f3.huml, 77, 2, "Hum baja");
        } else if (z1f3Buttons[10].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f3.humh, 78, 2, "Hum alta");
        }
        break;
      case 36:
        if (z1f4Buttons[4].contains(p.x, p.y)) {
          Z1Screen();
        } else if (z1f4Buttons[0].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f4.dias, 90, 3, "Dias");
        } else if (z1f4Buttons[1].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f4.hLuz, 92, 2, "Horas luz");
        } else if (z1f4Buttons[6].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f4.templ, 93, 2, "Temp baja");
        } else if (z1f4Buttons[5].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f4.temph, 94, 2, "Temp alta");
        } else if (z1f4Buttons[8].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f4.riegol, 95, 2, "Riego bajo");
        } else if (z1f4Buttons[7].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f4.riegoh, 96, 2, "Riego alto");
        } else if (z1f4Buttons[11].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f4.huml, 97, 2, "Hum baja");
        } else if (z1f4Buttons[10].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f4.humh, 98, 2, "Hum alta");
        }

        break;
      case 255:
        numKBstrLength = strlen(numKBstr);

        tft.setTextSize(BUTTON_TEXTSIZE);

        if (numericKeyboardButtons[3].contains(p.x, p.y)) {  // del
          if (numKBstrLength != 0) {  // para no borrar de más
            numKBstr[numKBstrLength - 1] = '\0';

            // todo esto tapa el ultimo char de la str
            tft.fillRect(15 + ((numKBstrLength - 1) * 6 * BUTTON_TEXTSIZE),
                         60 - ((7 * BUTTON_TEXTSIZE) / 2),
                         (6 * BUTTON_TEXTSIZE), (7 * BUTTON_TEXTSIZE), BLACK);
            delay(150);
          }
        } else if (numKBstrLength < numKBbufferSize) {
          if (numericKeyboardButtons[1].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '0';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("0"));
          } else if (numericKeyboardButtons[4].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '1';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("1"));
          } else if (numericKeyboardButtons[5].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '2';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("2"));
          } else if (numericKeyboardButtons[6].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '3';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("3"));
          } else if (numericKeyboardButtons[7].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '4';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("4"));
          } else if (numericKeyboardButtons[8].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '5';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("5"));
          } else if (numericKeyboardButtons[9].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '6';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("6"));
          } else if (numericKeyboardButtons[10].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '7';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("7"));
          } else if (numericKeyboardButtons[11].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '8';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("8"));
          } else if (numericKeyboardButtons[12].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '9';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("9"));
          }
          delay(150);
        }
        // aca van cada una de las pantallas en las que hay un teclado
        if (numericKeyboardButtons[0].contains(p.x, p.y)) {
          if (numKBPrevScreen == 33) {
            Z1F1Screen();
          } else if (numKBPrevScreen == 34) {
            Z1F2Screen();
          } else if (numKBPrevScreen == 35) {
            Z1F3Screen();
          } else if (numKBPrevScreen == 36) {
            Z1F4Screen();
          } else if (numKBPrevScreen == 31) {
            Z1ControlScreen();
          }
        } else if (numericKeyboardButtons[13].contains(p.x, p.y)) {
          if (numKBvarptr8b != NULL) {
            *numKBvarptr8b = atoi(numKBstr);
            EEPROM.put(numKBeeprom, atoi(numKBstr));
          } else if (numKBvarptr16b != NULL) {
            *numKBvarptr16b = atoi(numKBstr);
            EEPROM.put(numKBeeprom, atoi(numKBstr));
          }
          if (numKBPrevScreen == 33) {
            Z1F1Screen();
          } else if (numKBPrevScreen == 34) {
            Z1F2Screen();
          } else if (numKBPrevScreen == 35) {
            Z1F3Screen();
          } else if (numKBPrevScreen == 36) {
            Z1F4Screen();
          } else if (numKBPrevScreen == 31) {
            Z1ControlScreen();
          }
        }
        break;
    }
  }
}

// MENUS

void HomeScreen() {
  currentScreen = 0;
  drawHomeScreen();
  prevScreen = currentScreen;
}

void MenuScreen() {
  currentScreen = 1;
  drawMenuScreen();
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void AjustesScreen() {
  currentScreen = 2;
  drawAjustesScreen();
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void AlarmasScreen() {
  currentScreen = 3;
  drawAlarmasScreen();
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void RelojScreen() {
  currentScreen = 4;
  drawRelojScreen();
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void ProgramasScreen() {
  currentScreen = 5;
  drawProgramasScreen();
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Programa1Screen() {
  currentScreen = 7;
  drawPrograma1Screen();
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Programa2Screen() {
  currentScreen = 8;
  drawPrograma2Screen();
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Programa3Screen() {
  currentScreen = 9;
  drawPrograma3Screen();
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Programa4Screen() {
  currentScreen = 10;
  drawPrograma4Screen();
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void ResetScreen() {
  currentScreen = 6;
  drawResetScreen();
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Z1Screen() {
  currentScreen = 30;
  drawZ1Screen();
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Z1ControlScreen() {
  currentScreen = 31;
  drawZ1ControlScreen();
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Z1InicioScreen() {
  currentScreen = 32;
  z1fSeleccionada = z1fActiva;
  drawZ1InicioScreen();
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Z1F1Screen() {
  currentScreen = 33;
  drawZ1F1Screen();
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Z1F2Screen() {
  currentScreen = 34;
  drawZ1F2Screen();
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Z1F3Screen() {
  currentScreen = 35;
  drawZ1F3Screen();
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Z1F4Screen() {
  currentScreen = 36;
  drawZ1F4Screen();
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

// pantalla de teclado numerico para modificar valores
// quiza puedo hacer una sola funcion con void*
// intptr       ptr al int a modificar
// eepromdir    dirección eeprom en la que guardar el valor
// bufferSize   digitos maximos de la string a modificar
// title        titulo de la pantalla de teclado
void NumericKeyboardScreen(uint8_t* intptr, uint16_t eepromdir,
                           uint8_t bufferSize, const char* title) {
  currentScreen = 255;
  sprintf_P(numKBstr, STR_fdecimal, *intptr);
  numKBvarptr16b = NULL;
  numKBvarptr8b = intptr;
  numKBeeprom = eepromdir;
  numKBbufferSize = bufferSize;
  drawNumericKeyboardScreen(title);
  prevScreen = currentScreen;
}

void NumericKeyboardScreen(uint16_t* intptr, uint16_t eepromdir,
                           uint8_t bufferSize, const char* title) {
  currentScreen = 255;
  sprintf_P(numKBstr, STR_fdecimal, *intptr);
  numKBvarptr16b = intptr;
  numKBvarptr8b = NULL;
  numKBeeprom = eepromdir;
  numKBbufferSize = bufferSize;
  drawNumericKeyboardScreen(title);
  prevScreen = currentScreen;
}

// SCREENS

void drawHomeScreen() {
  tft.fillScreen(BLACK);

  // menu principal
  homeButtons[0].initButtonUL(&tft, 5, 10, 230, 40, YELLOW, DARKGREEN, YELLOW,
                              "Menu", BUTTON_TEXTSIZE);
  homeButtons[0].drawButton();

  tft.setCursor(10, 65);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.print(F("Iluminacion"));

  if (PINC & LUZPIN) {
    tft.fillCircle(180, 69, 10, GREEN);
  } else {
    tft.fillCircle(180, 69, 10, LIGHTGREY);
  }

  tft.setCursor(10, 90);
  tft.print(F("Ventilacion"));

  if (PINC & FANPIN && !(PINC & HEATPIN)) {
    tft.fillCircle(180, 94, 10, BLUE);
  } else if (PINC & HEATPIN && !(PINC & FANPIN)) {
    tft.fillCircle(180, 94, 10, YELLOW);
  } else if (!(PINC & HEATPIN) && !(PINC & FANPIN)) {
    tft.fillCircle(180, 94, 10, LIGHTGREY);
  }

  tft.setCursor(10, 115);
  tft.print(F("Vaporizacion"));

  if (PINC & VAPPIN) {
    tft.fillCircle(180, 119, 10, GREEN);
  } else {
    tft.fillCircle(180, 119, 10, LIGHTGREY);
  }

  tft.setCursor(10, 140);
  tft.print(F("Riego"));

  if (PINC & RIEGOPIN) {
    tft.fillCircle(180, 144, 10, GREEN);
  } else {
    tft.fillCircle(180, 144, 10, LIGHTGREY);
  }

  tft.setCursor(10, 165);
  tft.print(F("Fase "));
  if (z1fActiva != 0) {
    tft.print(z1fActiva);
  } else {
    tft.print(F("-"));
  }

  strcpy_P(buffer, PSTR("hh:mm"));
  now.toString(buffer);
  tft.setCursor(170, 165);
  tft.print(buffer);

  strcpy_P(buffer, PSTR("DD/MM/YY"));
  now.toString(buffer);
  tft.setCursor(134, 183);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), fActivaSP.dias);
  tft.setCursor(10, 200);
  tft.print(F("Dias   "));

  tft.setCursor(125 - (strlen(buffer) * 12), 200);
  tft.setTextColor(ORANGE);
  tft.print(buffer);

  tft.setCursor(130, 200);
  tft.setTextColor(WHITE, BLACK);
  tft.print(F("Riego "));

  sprintf_P(buffer, PSTR("%d"), fActivaSP.riegoh);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(130, 220);
  tft.setTextColor(ORANGE);
  tft.print(buffer);  // riego sp h

  sprintf_P(buffer, PSTR("%d"), fActivaSP.riegol);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(130, 240);
  tft.setTextColor(BLUE);
  tft.print(buffer);  // riego sp l

  tft.setCursor(10, 260);
  tft.setTextColor(WHITE, BLACK);
  tft.print(F("T."));

  sprintf_P(buffer, PSTR("%d"), fActivaSP.templ);
  strcat_P(buffer, PSTR("C"));
  tft.setCursor(77 - (strlen(buffer) * 12), 260);
  tft.setTextColor(BLUE);
  tft.print(buffer);  // temp sp l

  sprintf_P(buffer, PSTR("%d"), fActivaSP.temph);
  strcat_P(buffer, PSTR("C"));
  tft.setCursor(125 - (strlen(buffer) * 12), 260);
  tft.setTextColor(ORANGE);
  tft.print(buffer);  // temp sp h

  tft.setCursor(130, 260);
  tft.setTextColor(WHITE, BLACK);
  tft.print(F("Humedad "));

  sprintf_P(buffer, PSTR("%d"), fActivaSP.humh);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(130, 280);
  tft.setTextColor(ORANGE);
  tft.print(buffer);  // humedad sp h

  sprintf_P(buffer, PSTR("%d"), fActivaSP.huml);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(130, 300);
  tft.setTextColor(BLUE);
  tft.print(buffer);  // humedad sp l

  sprintf_P(buffer, PSTR("%d"), dias);
  tft.setCursor(125 - (strlen(buffer) * 18), 230);
  tft.setTextSize(3);
  tft.setTextColor(WHITE, BLACK);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), hTierra);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(230 - (strlen(buffer) * 18), 230);
  tft.print(buffer);

  dtostrf(t, 4, 1, buffer);
  strcat_P(buffer, PSTR("C"));
  tft.setCursor(125 - (strlen(buffer) * 18), 285);
  tft.print(buffer);  // temperatura leida por el DHT

  sprintf_P(buffer, PSTR("%d"), (uint8_t)h);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(230 - (strlen(buffer) * 18), 285);
  tft.print(buffer);  // humedad leida por el DHT

  Serial.println(F("home dibujada"));
}

void drawMenuScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 4) - 3) / 2), 10);
  tft.print(F("Menu"));

  // boton 1 - zona 1
  menuButtons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                              "Zona 1", BUTTON_TEXTSIZE);
  menuButtons[0].drawButton();

  // boton 2 - ajustes
  menuButtons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                              "Ajustes", BUTTON_TEXTSIZE);
  menuButtons[1].drawButton();

  menuButtons[2].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                              "Volver", BUTTON_TEXTSIZE);
  menuButtons[2].drawButton();

  Serial.println(F("menu dibujado"));
}

void drawAjustesScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(F("Ajustes"), 120, 18, &x1, &y1, &w, &h);
  tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("Ajustes"));

  // boton 1 - alarmas
  ajustesButtons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                                 "Alarmas", BUTTON_TEXTSIZE);
  ajustesButtons[0].drawButton();

  // boton 2 - reloj
  ajustesButtons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                                 "Reloj", BUTTON_TEXTSIZE);
  ajustesButtons[1].drawButton();

  // boton 3 - programas
  ajustesButtons[2].initButtonUL(&tft, 5, 125, 230, 40, WHITE, ORANGE, YELLOW,
                                 "Programas", BUTTON_TEXTSIZE);
  ajustesButtons[2].drawButton();

  // boton 4 - hard reset
  ajustesButtons[3].initButtonUL(&tft, 5, 170, 230, 40, WHITE, ORANGE, YELLOW,
                                 "H. Reset", BUTTON_TEXTSIZE);
  ajustesButtons[3].drawButton();

  // boton 5 - Volver
  ajustesButtons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                                 "Volver", BUTTON_TEXTSIZE);
  ajustesButtons[4].drawButton();

  Serial.println(F("ajustes dibujado"));
}

void drawAlarmasScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 7) - 3) / 2), 10);
  tft.print(F("Alarmas"));

  alarmasButtons[0].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                                 "Volver", BUTTON_TEXTSIZE);
  alarmasButtons[0].drawButton();

  Serial.println(F("alarmas dibujado"));
}

void drawRelojScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 5) - 3) / 2), 10);
  tft.print(F("Reloj"));

  now = rtc.now();

  relojYYYY = now.year();
  relojMM = now.month();
  relojDD = now.day();
  relojhh = now.hour();
  relojmm = now.minute();

  tft.setTextSize(3);

  tft.fillTriangle(61, 34, 49, 50, 73, 50, ORANGE);     // año++
  tft.fillTriangle(136, 34, 124, 50, 148, 50, ORANGE);  // mes++
  tft.fillTriangle(192, 34, 180, 50, 204, 50, ORANGE);  // dia++

  sprintf_P(buffer, PSTR("%d"), relojYYYY);
  tft.setCursor(28, 60);
  tft.print(buffer);

  sprintf_P(buffer, STR_f02decimal, relojMM);
  tft.setCursor(28 + 72 + 20, 60);
  tft.print(buffer);

  sprintf_P(buffer, STR_f02decimal, relojDD);
  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
  tft.print(buffer);

  tft.fillTriangle(61, 107, 49, 91, 73, 91, ORANGE);     // año--
  tft.fillTriangle(136, 107, 124, 91, 148, 91, ORANGE);  // mes--
  tft.fillTriangle(192, 107, 180, 91, 204, 91, ORANGE);  // dia--

  tft.fillTriangle(89, 124, 77, 140, 101, 140, ORANGE);    // hora++
  tft.fillTriangle(146, 124, 134, 140, 158, 140, ORANGE);  // minuto++

  sprintf_P(buffer, STR_f02decimal, relojhh);
  tft.setCursor(74, 150);
  tft.print(buffer);

  sprintf_P(buffer, STR_f02decimal, relojmm);
  tft.setCursor(74 + 36 + 20, 150);
  tft.print(buffer);

  tft.fillTriangle(89, 197, 77, 181, 101, 181, ORANGE);    // hora--
  tft.fillTriangle(146, 197, 134, 181, 158, 181, ORANGE);  // minuto--

  relojButtons[1].initButtonUL(&tft, 20, 210, 200, 40, WHITE, DARKGREEN, WHITE,
                               "Aceptar", BUTTON_TEXTSIZE);
  relojButtons[1].drawButton();

  relojButtons[0].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                               "Volver", BUTTON_TEXTSIZE);
  relojButtons[0].drawButton();

  Serial.println(F("reloj dibujado"));
}

void drawProgramasScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(F("Programas"), 110, 18, &x1, &y1, &w, &h);
  tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("Programas"));

  programasButtons[1].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Prog. 1", BUTTON_TEXTSIZE);
  programasButtons[1].drawButton();

  programasButtons[2].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Prog. 2", BUTTON_TEXTSIZE);
  programasButtons[2].drawButton();

  programasButtons[3].initButtonUL(&tft, 5, 125, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Prog. 3", BUTTON_TEXTSIZE);
  programasButtons[3].drawButton();

  programasButtons[4].initButtonUL(&tft, 5, 170, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Prog. 4", BUTTON_TEXTSIZE);
  programasButtons[4].drawButton();

  programasButtons[0].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY,
                                   WHITE, "Volver", BUTTON_TEXTSIZE);
  programasButtons[0].drawButton();

  Serial.println(F("programas dibujado"));
}

void drawPrograma1Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(F("Prog. 1"), 120, 18, &x1, &y1, &w, &h);
  tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("Prog. 1"));

  programa1Buttons[1].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Cargar", BUTTON_TEXTSIZE);
  programa1Buttons[1].drawButton();

  programa1Buttons[2].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Guardar", BUTTON_TEXTSIZE);
  programa1Buttons[2].drawButton();

  programa1Buttons[3].initButtonUL(&tft, 5, 125, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Reestablecer", BUTTON_TEXTSIZE);
  programa1Buttons[3].drawButton();

  programa1Buttons[4].initButtonUL(&tft, 20, 170, 200, 40, WHITE, GREEN, WHITE,
                                   "Confirmar", BUTTON_TEXTSIZE);

  programa1Buttons[0].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY,
                                   WHITE, "Volver", BUTTON_TEXTSIZE);
  programa1Buttons[0].drawButton();
}

void drawPrograma2Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(F("Prog. 2"), 120, 18, &x1, &y1, &w, &h);
  tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("Prog. 2"));

  programa2Buttons[1].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Cargar", BUTTON_TEXTSIZE);
  programa2Buttons[1].drawButton();

  programa2Buttons[2].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Guardar", BUTTON_TEXTSIZE);
  programa2Buttons[2].drawButton();

  programa2Buttons[3].initButtonUL(&tft, 5, 125, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Reestablecer", BUTTON_TEXTSIZE);
  programa2Buttons[3].drawButton();

  programa2Buttons[4].initButtonUL(&tft, 20, 170, 200, 40, WHITE, GREEN, WHITE,
                                   "Confirmar", BUTTON_TEXTSIZE);

  programa2Buttons[0].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY,
                                   WHITE, "Volver", BUTTON_TEXTSIZE);
  programa2Buttons[0].drawButton();
}

void drawPrograma3Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(F("Prog. 3"), 120, 18, &x1, &y1, &w, &h);
  tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("Prog. 3"));

  programa3Buttons[1].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Cargar", BUTTON_TEXTSIZE);
  programa3Buttons[1].drawButton();

  programa3Buttons[2].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Guardar", BUTTON_TEXTSIZE);
  programa3Buttons[2].drawButton();

  programa3Buttons[3].initButtonUL(&tft, 5, 125, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Reestablecer", BUTTON_TEXTSIZE);
  programa3Buttons[3].drawButton();

  programa3Buttons[4].initButtonUL(&tft, 20, 170, 200, 40, WHITE, GREEN, WHITE,
                                   "Confirmar", BUTTON_TEXTSIZE);

  programa3Buttons[0].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY,
                                   WHITE, "Volver", BUTTON_TEXTSIZE);
  programa3Buttons[0].drawButton();
}

void drawPrograma4Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(F("Prog. 4"), 120, 18, &x1, &y1, &w, &h);
  tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("Prog. 4"));

  programa4Buttons[1].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Cargar", BUTTON_TEXTSIZE);
  programa4Buttons[1].drawButton();

  programa4Buttons[2].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Guardar", BUTTON_TEXTSIZE);
  programa4Buttons[2].drawButton();

  programa4Buttons[3].initButtonUL(&tft, 5, 125, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Reestablecer", BUTTON_TEXTSIZE);
  programa4Buttons[3].drawButton();

  programa4Buttons[4].initButtonUL(&tft, 20, 170, 200, 40, WHITE, GREEN, WHITE,
                                   "Confirmar", BUTTON_TEXTSIZE);

  programa4Buttons[0].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY,
                                   WHITE, "Volver", BUTTON_TEXTSIZE);
  programa4Buttons[0].drawButton();
}

void drawResetScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 5) - 3) / 2), 10);
  tft.print(F("Reset"));

  tft.setCursor(5, 35);
  tft.print(
      F("\tEsta seguro que\n desea reiniciar el\n dispositivo a la\n "
        "configuracion de\n fabrica?"));

  resetButtons[0].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                               "Volver", BUTTON_TEXTSIZE);
  resetButtons[0].drawButton();

  resetButtons[1].initButtonUL(&tft, 20, 170, 200, 40, WHITE, RED, WHITE,
                               "Confirmar", BUTTON_TEXTSIZE);
  resetButtons[1].drawButton();

  Serial.println(F("reset dibujado"));
}

void drawZ1Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 6) - 3) / 2), 10);
  tft.print(F("Zona 1"));

  // boton 1 - fase 1
  z1Buttons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                            "Fase 1", BUTTON_TEXTSIZE);
  z1Buttons[0].drawButton();

  // boton 2 - fase 2
  z1Buttons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                            "Fase 2", BUTTON_TEXTSIZE);
  z1Buttons[1].drawButton();

  // boton 3 - fase 3
  z1Buttons[2].initButtonUL(&tft, 5, 125, 230, 40, WHITE, ORANGE, YELLOW,
                            "Fase 3", BUTTON_TEXTSIZE);
  z1Buttons[2].drawButton();

  // boton 4 - fase 4
  z1Buttons[3].initButtonUL(&tft, 5, 170, 230, 40, WHITE, ORANGE, YELLOW,
                            "Fase 4", BUTTON_TEXTSIZE);
  z1Buttons[3].drawButton();

  // boton 5 - control
  z1Buttons[4].initButtonUL(&tft, 5, 215, 230, 40, WHITE, ORANGE, YELLOW,
                            "Control", BUTTON_TEXTSIZE);
  z1Buttons[4].drawButton();

  // boton 6 - volver
  z1Buttons[5].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                            "Volver", BUTTON_TEXTSIZE);
  z1Buttons[5].drawButton();

  Serial.println(F("z1 dibujado"));
}

void drawZ1F1Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 6) - 3) / 2), 10);
  tft.print(F("Fase 1"));

  z1f1Buttons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f1Buttons[0].drawRectButton();
  tft.setCursor(15, 40);
  tft.print(F("Dias"));

  z1f1Buttons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f1Buttons[1].drawRectButton();
  tft.setCursor(15, 85);
  tft.print(F("Horas luz"));

  z1f1Buttons[6].initButtonUL(&tft, 100, 125, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f1Buttons[6].drawRectButton();
  z1f1Buttons[5].initButtonUL(&tft, 175, 125, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f1Buttons[5].drawRectButton();
  z1f1Buttons[2].initButtonUL(&tft, 5, 125, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f1Buttons[2].drawRectButton();
  tft.setCursor(15, 130);
  tft.print(F("Temp."));

  z1f1Buttons[8].initButtonUL(&tft, 100, 170, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f1Buttons[8].drawRectButton();
  z1f1Buttons[7].initButtonUL(&tft, 175, 170, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f1Buttons[7].drawRectButton();
  z1f1Buttons[3].initButtonUL(&tft, 5, 170, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f1Buttons[3].drawRectButton();
  tft.setCursor(15, 175);
  tft.print(F("Riego"));

  z1f1Buttons[11].initButtonUL(&tft, 100, 215, 60, 40, BLACK, BLACK, BLACK, "",
                               2);
  z1f1Buttons[11].drawRectButton();
  z1f1Buttons[10].initButtonUL(&tft, 175, 215, 60, 40, BLACK, BLACK, BLACK, "",
                               2);
  z1f1Buttons[10].drawRectButton();
  z1f1Buttons[9].initButtonUL(&tft, 5, 215, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f1Buttons[9].drawRectButton();
  tft.setCursor(15, 220);
  tft.print(F("Humedad"));

  tft.setTextSize(3);

  //// dias
  sprintf_P(buffer, PSTR("%d"), f1.dias);

  tft.setCursor(228 - (strlen(buffer) * 18), 45);
  tft.print(buffer);

  //// hluz
  if (f1.hLuz > 24) f1.hLuz = 24;
  sprintf_P(buffer, PSTR("%d"), f1.hLuz);
  strcat_P(buffer, STR_hum);
  tft.setCursor(228 - (strlen(buffer) * 18), 90);
  tft.print(buffer);

  //// temp
  if (f1.temph > 80) f1.temph = 80;
  sprintf_P(buffer, PSTR("%d"), f1.temph);
  uint8_t z1f1temphSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("C"));
  tft.setCursor(228 - (strlen(buffer) * 18), 135);
  tft.print(buffer);

  if (f1.templ > 80) f1.templ = 80;
  sprintf_P(buffer, PSTR("%d"), f1.templ);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f1temphSTRlen * 18), 135);
  tft.print(buffer);

  tft.setCursor(192 - (z1f1temphSTRlen * 18), 135);
  tft.print(F("-"));

  //// riego
  sprintf_P(buffer, PSTR("%d"), f1.riegoh);
  uint8_t z1f1riegohSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 180);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), f1.riegol);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f1riegohSTRlen * 18), 180);
  tft.print(buffer);

  tft.setCursor(192 - (z1f1riegohSTRlen * 18), 180);
  tft.print(F("-"));

  //// humedad
  sprintf_P(buffer, PSTR("%d"), f1.humh);
  uint8_t z1f1humhSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 225);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), f1.huml);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f1humhSTRlen * 18), 225);
  tft.print(buffer);

  tft.setCursor(192 - (z1f1humhSTRlen * 18), 225);
  tft.print(F("-"));

  z1f1Buttons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                              "Volver", BUTTON_TEXTSIZE);
  z1f1Buttons[4].drawButton();

  Serial.println(F("z1f1 dibujado"));
}

void drawZ1F2Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 6) - 3) / 2), 10);
  tft.print(F("Fase 2"));

  z1f2Buttons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f2Buttons[0].drawRectButton();
  tft.setCursor(15, 40);
  tft.print(F("Dias"));

  z1f2Buttons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f2Buttons[1].drawRectButton();
  tft.setCursor(15, 85);
  tft.print(F("Horas luz"));

  z1f2Buttons[6].initButtonUL(&tft, 100, 125, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f2Buttons[6].drawRectButton();
  z1f2Buttons[5].initButtonUL(&tft, 175, 125, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f2Buttons[5].drawRectButton();
  z1f2Buttons[2].initButtonUL(&tft, 5, 125, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f2Buttons[2].drawRectButton();
  tft.setCursor(15, 130);
  tft.print(F("Temp."));

  z1f2Buttons[8].initButtonUL(&tft, 100, 170, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f2Buttons[8].drawRectButton();
  z1f2Buttons[7].initButtonUL(&tft, 175, 170, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f2Buttons[7].drawRectButton();
  z1f2Buttons[3].initButtonUL(&tft, 5, 170, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f2Buttons[3].drawRectButton();
  tft.setCursor(15, 175);
  tft.print(F("Riego"));

  z1f2Buttons[11].initButtonUL(&tft, 100, 215, 60, 40, BLACK, BLACK, BLACK, "",
                               2);
  z1f2Buttons[11].drawRectButton();
  z1f2Buttons[10].initButtonUL(&tft, 175, 215, 60, 40, BLACK, BLACK, BLACK, "",
                               2);
  z1f2Buttons[10].drawRectButton();
  z1f2Buttons[9].initButtonUL(&tft, 5, 215, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f2Buttons[9].drawRectButton();
  tft.setCursor(15, 220);
  tft.print(F("Humedad"));

  tft.setTextSize(3);

  //// dias
  sprintf_P(buffer, PSTR("%d"), f2.dias);

  tft.setCursor(228 - (strlen(buffer) * 18), 45);
  tft.print(buffer);

  //// hluz
  if (f2.hLuz > 24) f2.hLuz = 24;
  sprintf_P(buffer, PSTR("%d"), f2.hLuz);
  strcat_P(buffer, STR_hum);
  tft.setCursor(228 - (strlen(buffer) * 18), 90);
  tft.print(buffer);

  //// temp
  if (f2.temph > 80) f2.temph = 80;
  sprintf_P(buffer, PSTR("%d"), f2.temph);
  uint8_t z1f2temphSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("C"));
  tft.setCursor(228 - (strlen(buffer) * 18), 135);
  tft.print(buffer);

  if (f2.templ > 80) f2.templ = 80;
  sprintf_P(buffer, PSTR("%d"), f2.templ);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f2temphSTRlen * 18), 135);
  tft.print(buffer);

  tft.setCursor(192 - (z1f2temphSTRlen * 18), 135);
  tft.print(F("-"));

  //// riego
  sprintf_P(buffer, PSTR("%d"), f2.riegoh);
  uint8_t z1f2riegohSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 180);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), f2.riegol);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f2riegohSTRlen * 18), 180);
  tft.print(buffer);

  tft.setCursor(192 - (z1f2riegohSTRlen * 18), 180);
  tft.print(F("-"));

  //// humedad
  sprintf_P(buffer, PSTR("%d"), f2.humh);
  uint8_t z1f2humhSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 225);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), f2.huml);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f2humhSTRlen * 18), 225);
  tft.print(buffer);

  tft.setCursor(192 - (z1f2humhSTRlen * 18), 225);
  tft.print(F("-"));

  z1f2Buttons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                              "Volver", BUTTON_TEXTSIZE);
  z1f2Buttons[4].drawButton();

  Serial.println(F("z1f2 dibujado"));
}

void drawZ1F3Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 6) - 3) / 2), 10);
  tft.print(F("Fase 3"));

  z1f3Buttons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f3Buttons[0].drawRectButton();
  tft.setCursor(15, 40);
  tft.print(F("Dias"));

  z1f3Buttons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f3Buttons[1].drawRectButton();
  tft.setCursor(15, 85);
  tft.print("Horas luz");

  z1f3Buttons[6].initButtonUL(&tft, 100, 125, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f3Buttons[6].drawRectButton();
  z1f3Buttons[5].initButtonUL(&tft, 175, 125, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f3Buttons[5].drawRectButton();
  z1f3Buttons[2].initButtonUL(&tft, 5, 125, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f3Buttons[2].drawRectButton();
  tft.setCursor(15, 130);
  tft.print(F("Temp."));

  z1f3Buttons[8].initButtonUL(&tft, 100, 170, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f3Buttons[8].drawRectButton();
  z1f3Buttons[7].initButtonUL(&tft, 175, 170, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f3Buttons[7].drawRectButton();
  z1f3Buttons[3].initButtonUL(&tft, 5, 170, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f3Buttons[3].drawRectButton();
  tft.setCursor(15, 175);
  tft.print(F("Riego"));

  z1f3Buttons[11].initButtonUL(&tft, 100, 215, 60, 40, BLACK, BLACK, BLACK, "",
                               2);
  z1f3Buttons[11].drawRectButton();
  z1f3Buttons[10].initButtonUL(&tft, 175, 215, 60, 40, BLACK, BLACK, BLACK, "",
                               2);
  z1f3Buttons[10].drawRectButton();
  z1f3Buttons[9].initButtonUL(&tft, 5, 215, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f3Buttons[9].drawRectButton();
  tft.setCursor(15, 220);
  tft.print(F("Humedad"));

  tft.setTextSize(3);

  //// dias
  sprintf_P(buffer, PSTR("%d"), f3.dias);

  tft.setCursor(228 - (strlen(buffer) * 18), 45);
  tft.print(buffer);

  //// hluz
  if (f3.hLuz > 24) f3.hLuz = 24;
  sprintf_P(buffer, PSTR("%d"), f3.hLuz);
  strcat_P(buffer, STR_hum);
  tft.setCursor(228 - (strlen(buffer) * 18), 90);
  tft.print(buffer);

  //// temp
  if (f3.temph > 80) f3.temph = 80;
  sprintf_P(buffer, PSTR("%d"), f3.temph);
  uint8_t z1f3temphSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("C"));
  tft.setCursor(228 - (strlen(buffer) * 18), 135);
  tft.print(buffer);

  if (f3.templ > 80) f3.templ = 80;
  sprintf_P(buffer, PSTR("%d"), f3.templ);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f3temphSTRlen * 18), 135);
  tft.print(buffer);

  tft.setCursor(192 - (z1f3temphSTRlen * 18), 135);
  tft.print(F("-"));

  //// riego
  sprintf_P(buffer, PSTR("%d"), f3.riegoh);
  uint8_t z1f3riegohSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 180);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), f3.riegol);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f3riegohSTRlen * 18), 180);
  tft.print(buffer);

  tft.setCursor(192 - (z1f3riegohSTRlen * 18), 180);
  tft.print(F("-"));

  //// humedad
  sprintf_P(buffer, PSTR("%d"), f3.humh);
  uint8_t z1f3humhSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 225);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), f3.huml);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f3humhSTRlen * 18), 225);
  tft.print(buffer);

  tft.setCursor(192 - (z1f3humhSTRlen * 18), 225);
  tft.print(F("-"));

  z1f3Buttons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                              "Volver", BUTTON_TEXTSIZE);
  z1f3Buttons[4].drawButton();

  Serial.println(F("z1f3 dibujado"));
}

void drawZ1F4Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 6) - 3) / 2), 10);
  tft.print(F("Fase 4"));

  z1f4Buttons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f4Buttons[0].drawRectButton();
  tft.setCursor(15, 40);
  tft.print(F("Dias"));

  z1f4Buttons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f4Buttons[1].drawRectButton();
  tft.setCursor(15, 85);
  tft.print(F("Horas luz"));

  z1f4Buttons[6].initButtonUL(&tft, 100, 125, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f4Buttons[6].drawRectButton();
  z1f4Buttons[5].initButtonUL(&tft, 175, 125, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f4Buttons[5].drawRectButton();
  z1f4Buttons[2].initButtonUL(&tft, 5, 125, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f4Buttons[2].drawRectButton();
  tft.setCursor(15, 130);
  tft.print(F("Temp."));

  z1f4Buttons[8].initButtonUL(&tft, 100, 170, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f4Buttons[8].drawRectButton();
  z1f4Buttons[7].initButtonUL(&tft, 175, 170, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f4Buttons[7].drawRectButton();
  z1f4Buttons[3].initButtonUL(&tft, 5, 170, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f4Buttons[3].drawRectButton();
  tft.setCursor(15, 175);
  tft.print(F("Riego"));

  z1f4Buttons[11].initButtonUL(&tft, 100, 215, 60, 40, BLACK, BLACK, BLACK, "",
                               2);
  z1f4Buttons[11].drawRectButton();
  z1f4Buttons[10].initButtonUL(&tft, 175, 215, 60, 40, BLACK, BLACK, BLACK, "",
                               2);
  z1f4Buttons[10].drawRectButton();
  z1f4Buttons[9].initButtonUL(&tft, 5, 215, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f4Buttons[9].drawRectButton();
  tft.setCursor(15, 220);
  tft.print(F("Humedad"));

  tft.setTextSize(3);

  //// dias
  sprintf_P(buffer, PSTR("%d"), f4.dias);

  tft.setCursor(228 - (strlen(buffer) * 18), 45);
  tft.print(buffer);

  //// hluz
  if (f4.hLuz > 24) f4.hLuz = 24;
  sprintf_P(buffer, PSTR("%d"), f4.hLuz);
  strcat_P(buffer, STR_hum);
  tft.setCursor(228 - (strlen(buffer) * 18), 90);
  tft.print(buffer);

  //// temp
  if (f4.temph > 80) f4.temph = 80;
  sprintf_P(buffer, PSTR("%d"), f4.temph);
  uint8_t z1f4temphSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("C"));
  tft.setCursor(228 - (strlen(buffer) * 18), 135);
  tft.print(buffer);

  if (f4.templ > 80) f4.templ = 80;
  sprintf_P(buffer, PSTR("%d"), f4.templ);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f4temphSTRlen * 18), 135);
  tft.print(buffer);

  tft.setCursor(192 - (z1f4temphSTRlen * 18), 135);
  tft.print(F("-"));

  //// riego
  sprintf_P(buffer, PSTR("%d"), f4.riegoh);
  uint8_t z1f4riegohSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 180);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), f4.riegol);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f4riegohSTRlen * 18), 180);
  tft.print(buffer);

  tft.setCursor(192 - (z1f4riegohSTRlen * 18), 180);
  tft.print(F("-"));

  //// humedad
  sprintf_P(buffer, PSTR("%d"), f4.humh);
  uint8_t z1f4humhSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 225);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), f4.huml);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f4humhSTRlen * 18), 225);
  tft.print(buffer);

  tft.setCursor(192 - (z1f4humhSTRlen * 18), 225);
  tft.print(F("-"));

  z1f4Buttons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                              "Volver", BUTTON_TEXTSIZE);
  z1f4Buttons[4].drawButton();

  Serial.println(F("z1f4 dibujado"));
}

void drawZ1ControlScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 7) - 3) / 2), 10);
  tft.print(F("Control"));

  z1ControlButtons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Inicio", BUTTON_TEXTSIZE);
  z1ControlButtons[0].drawButton();

  z1ControlButtons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Terminar", BUTTON_TEXTSIZE);
  z1ControlButtons[1].drawButton();

  z1ControlButtons[3].initButtonUL(&tft, 20, 170, 200, 40, WHITE, RED, WHITE,
                                   "Confirmar", BUTTON_TEXTSIZE);

  tft.drawRect(5, 125, 230, 40, WHITE);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.setCursor(15, 135);
  tft.print(F("Ciclos"));

  sprintf_P(buffer, PSTR("%d"), ciclos);
  tft.setCursor(228 - (strlen(buffer) * 18), 135);
  tft.print(buffer);

  z1ControlButtons[2].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY,
                                   WHITE, "Volver", BUTTON_TEXTSIZE);
  z1ControlButtons[2].drawButton();

  Serial.println(F("z1control dibujado"));
}

void drawZ1InicioScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 6) - 3) / 2), 10);
  tft.print(F("Inicio"));

  tft.setTextSize(BUTTON_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * BUTTON_TEXTSIZE * 5) - 3) / 2),
                40 - ((7 * BUTTON_TEXTSIZE) / 2));
  tft.print(F("Fases"));

  z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, DARKGREY, WHITE,
                                  "1", BUTTON_TEXTSIZE);

  z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE, DARKGREY, WHITE,
                                  "2", BUTTON_TEXTSIZE);

  z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE, DARKGREY, WHITE,
                                  "3", BUTTON_TEXTSIZE);

  z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE, DARKGREY,
                                  WHITE, "4", BUTTON_TEXTSIZE);

  switch (z1fActiva) {
    case 1:
      z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, OLIVE, WHITE,
                                      "1", BUTTON_TEXTSIZE);
      break;
    case 2:
      z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE, OLIVE,
                                      WHITE, "2", BUTTON_TEXTSIZE);
      break;
    case 3:
      z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE, OLIVE,
                                      WHITE, "3", BUTTON_TEXTSIZE);
      break;
    case 4:
      z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE, OLIVE,
                                      WHITE, "4", BUTTON_TEXTSIZE);
      break;
  }

  z1InicioButtons[0].drawRectButton();
  z1InicioButtons[1].drawRectButton();
  z1InicioButtons[2].drawRectButton();
  z1InicioButtons[3].drawRectButton();

  z1InicioButtons[4].initButtonUL(&tft, 5, 215, 230, 40, WHITE, DARKGREEN,
                                  WHITE, "Confirmar", BUTTON_TEXTSIZE);
  z1InicioButtons[4].drawButton();

  z1InicioButtons[5].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY,
                                  WHITE, "Volver", BUTTON_TEXTSIZE);
  z1InicioButtons[5].drawButton();

  Serial.println(F("z1inicio dibujado"));
}

void drawNumericKeyboardScreen(const char* title) {
  numKBPrevScreen = prevScreen;
  Serial.println(numKBPrevScreen);

  tft.fillScreen(BLACK);

  tft.setTextSize(BUTTON_TEXTSIZE);

  numericKeyboardButtons[0].initButtonUL(&tft, 5, 5, 60, 30, WHITE, ORANGE,
                                         WHITE, "<", BUTTON_TEXTSIZE);
  numericKeyboardButtons[0].drawButton();

  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(152 - (CHARACTER_WIDTH * TITLE_TEXTSIZE * strlen(title)) / 2,
                20 - (7 * 2) / 2);
  tft.print(title);

  tft.drawRect(5, 40, 230, 40, WHITE);

  tft.setTextSize(BUTTON_TEXTSIZE);
  tft.setCursor(15, 60 - ((7 * BUTTON_TEXTSIZE) / 2));
  tft.print(numKBstr);

  numericKeyboardButtons[1].initButtonUL(&tft, 30, 82, 40, 40, WHITE, BLACK,
                                         WHITE, "0", BUTTON_TEXTSIZE);
  numericKeyboardButtons[1].drawButton();
  // numericKeyboardButtons[2].initButtonUL(&tft, 100, 82, 40, 40, WHITE, BLACK,
  //                                       WHITE, "-", BUTTON_TEXTSIZE);
  // numericKeyboardButtons[2].drawButton();
  numericKeyboardButtons[3].initButtonUL(&tft, 155, 82, 80, 40, WHITE, BLACK,
                                         WHITE, "Del", BUTTON_TEXTSIZE);
  numericKeyboardButtons[3].drawButton();

  numericKeyboardButtons[4].initButtonUL(&tft, 30, 125, 40, 40, WHITE, BLACK,
                                         WHITE, "1", BUTTON_TEXTSIZE);
  numericKeyboardButtons[4].drawButton();
  numericKeyboardButtons[5].initButtonUL(&tft, 100, 125, 40, 40, WHITE, BLACK,
                                         WHITE, "2", BUTTON_TEXTSIZE);
  numericKeyboardButtons[5].drawButton();
  numericKeyboardButtons[6].initButtonUL(&tft, 175, 125, 40, 40, WHITE, BLACK,
                                         WHITE, "3", BUTTON_TEXTSIZE);
  numericKeyboardButtons[6].drawButton();

  numericKeyboardButtons[7].initButtonUL(&tft, 30, 177, 40, 40, WHITE, BLACK,
                                         WHITE, "4", BUTTON_TEXTSIZE);
  numericKeyboardButtons[7].drawButton();
  numericKeyboardButtons[8].initButtonUL(&tft, 100, 177, 40, 40, WHITE, BLACK,
                                         WHITE, "5", BUTTON_TEXTSIZE);
  numericKeyboardButtons[8].drawButton();
  numericKeyboardButtons[9].initButtonUL(&tft, 175, 177, 40, 40, WHITE, BLACK,
                                         WHITE, "6", BUTTON_TEXTSIZE);
  numericKeyboardButtons[9].drawButton();

  numericKeyboardButtons[10].initButtonUL(&tft, 30, 230, 40, 40, WHITE, BLACK,
                                          WHITE, "7", BUTTON_TEXTSIZE);
  numericKeyboardButtons[10].drawButton();
  numericKeyboardButtons[11].initButtonUL(&tft, 100, 230, 40, 40, WHITE, BLACK,
                                          WHITE, "8", BUTTON_TEXTSIZE);
  numericKeyboardButtons[11].drawButton();
  numericKeyboardButtons[12].initButtonUL(&tft, 175, 230, 40, 40, WHITE, BLACK,
                                          WHITE, "9", BUTTON_TEXTSIZE);
  numericKeyboardButtons[12].drawButton();

  numericKeyboardButtons[13].initButtonUL(&tft, 5, 280, 230, 40, WHITE,
                                          DARKCYAN, WHITE, "Aceptar",
                                          BUTTON_TEXTSIZE);
  numericKeyboardButtons[13].drawButton();
}

void drawStartupScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);

  // tft.setCursor(120 - (6 * 3 * 6) / 2, 160 - (7 * 3));
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(F("Silics"), 120, 139, &x1, &y1, &w, &h);
  tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("Silics"));

  tft.setCursor(120 - (6 * 3 * 10) / 2, 170);
  // tft.getTextBounds(F("GrowOS 0.9"), 120, 170, &x1, &y1, &w, &h);
  // tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("GrowOS 0.9"));
}

void drawGoodbyeScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);

  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(F("^_^"), 120, 160, &x1, &y1, &w, &h);
  tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("^_^"));
}

// EEPROM

void cargarEstado() {
  z1fActiva = EEPROM.read(10);
  z1fActivalast = z1fActiva;
  z1fSeleccionada = z1fActiva;
  EEPROM.get(11, diaIniciodefase);
  EEPROM.get(15, diaFindefase);
  hInicioLuz = EEPROM.read(19);
  hFinLuz = EEPROM.read(20);
  mInicioFinLuz = EEPROM.read(21);
  ciclos = EEPROM.read(22);
}

void cargarFases() {
  EEPROM.get(30, f1);
  EEPROM.get(50, f2);
  EEPROM.get(70, f3);
  EEPROM.get(90, f4);
}

void eeprom_hardReset() {
  Fase F1DefaultSettings;

  F1DefaultSettings.dias = 1;
  F1DefaultSettings.hLuz = 1;
  F1DefaultSettings.templ = 20;
  F1DefaultSettings.temph = 30;
  F1DefaultSettings.riegol = 20;
  F1DefaultSettings.riegoh = 60;
  F1DefaultSettings.huml = 60;
  F1DefaultSettings.humh = 80;

  Fase F2DefaultSettings = F1DefaultSettings;
  Fase F3DefaultSettings = F1DefaultSettings;
  Fase F4DefaultSettings = F1DefaultSettings;

  F2DefaultSettings.dias = 2;
  F2DefaultSettings.hLuz = 2;
  F3DefaultSettings.dias = 3;
  F3DefaultSettings.hLuz = 3;
  F4DefaultSettings.dias = 4;
  F4DefaultSettings.hLuz = 4;

  Programa p1;
  p1.f1.dias = 90;
  p1.f1.hLuz = 16;
  p1.f1.templ = 20;
  p1.f1.temph = 30;
  p1.f1.riegol = 30;
  p1.f1.riegoh = 60;
  p1.f1.huml = 60;
  p1.f1.humh = 70;

  p1.f2.dias = 90;
  p1.f2.hLuz = 14;
  p1.f2.templ = 10;
  p1.f2.temph = 20;
  p1.f2.riegol = 20;
  p1.f2.riegoh = 60;
  p1.f2.huml = 65;
  p1.f2.humh = 75;

  p1.f3.dias = 90;
  p1.f3.hLuz = 12;
  p1.f3.templ = 5;
  p1.f3.temph = 15;
  p1.f3.riegol = 10;
  p1.f3.riegoh = 40;
  p1.f3.huml = 70;
  p1.f3.humh = 80;

  p1.f4.dias = 90;
  p1.f4.hLuz = 14;
  p1.f4.templ = 10;
  p1.f4.temph = 20;
  p1.f4.riegol = 10;
  p1.f4.riegoh = 60;
  p1.f4.huml = 65;
  p1.f4.humh = 75;

  Programa p2 = p1;
  Programa p3 = p1;
  Programa p4 = p1;

  p2.f1.dias = 999;
  p2.f2.dias = 999;
  p2.f3.dias = 999;
  p2.f4.dias = 999;

  p3.f1.dias = 888;
  p3.f2.dias = 888;
  p3.f3.dias = 888;
  p3.f4.dias = 888;

  p4.f1.dias = 666;
  p4.f2.dias = 666;
  p4.f3.dias = 666;
  p4.f4.dias = 666;

  EEPROM.update(0, 0x00);
  Serial.println(F("device information restored"));
  for (uint8_t i = 10; i < 30; i++) {
    EEPROM.update(i, 0x00);
  }
  EEPROM.update(22, 1);
  Serial.println(F("phase information restored"));
  EEPROM.put(30, F1DefaultSettings);
  Serial.println(F("F1 settings restored"));
  EEPROM.put(50, F2DefaultSettings);
  Serial.println(F("F2 settings restored"));
  EEPROM.put(70, F3DefaultSettings);
  Serial.println(F("F3 settings restored"));
  EEPROM.put(90, F4DefaultSettings);
  Serial.println(F("F4 settings restored"));

  EEPROM.put(110, p1.f1);
  Serial.println(F("P1F1 settings restored"));
  EEPROM.put(130, p1.f2);
  Serial.println(F("P1F2 settings restored"));
  EEPROM.put(150, p1.f3);
  Serial.println(F("P1F3 settings restored"));
  EEPROM.put(170, p1.f4);
  Serial.println(F("P1F4 settings restored"));

  EEPROM.put(210, p2.f1);
  Serial.println(F("P2F1 settings restored"));
  EEPROM.put(230, p2.f2);
  Serial.println(F("P2F2 settings restored"));
  EEPROM.put(250, p2.f3);
  Serial.println(F("P2F3 settings restored"));
  EEPROM.put(270, p2.f4);
  Serial.println(F("P2F4 settings restored"));

  EEPROM.put(310, p3.f1);
  Serial.println(F("P3F1 settings restored"));
  EEPROM.put(330, p3.f2);
  Serial.println(F("P3F2 settings restored"));
  EEPROM.put(350, p3.f3);
  Serial.println(F("P3F3 settings restored"));
  EEPROM.put(370, p3.f4);
  Serial.println(F("P3F4 settings restored"));

  EEPROM.put(410, p4.f1);
  Serial.println(F("P4F1 settings restored"));
  EEPROM.put(430, p4.f2);
  Serial.println(F("P4F2 settings restored"));
  EEPROM.put(450, p4.f3);
  Serial.println(F("P4F3 settings restored"));
  EEPROM.put(470, p4.f4);
  Serial.println(F("P4F4 settings restored"));

  drawGoodbyeScreen();

  delay(250);

  wdt_enable(WDTO_15MS);
  while (1)
    ;
}

void eeprom_clear() {
  for (uint16_t i = 0; i < EEPROM.length(); i++) {
    EEPROM.update(i, 0xFF);
    if (!(i % 512)) {
      Serial.print(F("cleared "));
      Serial.print(i);
      Serial.println(F(" EEPROM bytes"));
    }
  }
  Serial.println(F("eeprom cleared to 0xFF"));
}

void eeprom_read() {
  Serial.println(F("Reading EEPROM: "));
  for (uint16_t i = 0; i < 500; i++) {
    switch (i) {
      case 0:
        Serial.print(F("\n\ndevice info"));
        break;
      case 10:
        Serial.print(F("\n\nphase info"));
        break;
      case 30:
        Serial.print(F("\n\nF1 settings"));
        break;
      case 50:
        Serial.print(F("\n\nF2 settings"));
        break;
      case 70:
        Serial.print(F("\n\nF3 settings"));
        break;
      case 90:
        Serial.print(F("\n\nF4 settings"));
        break;
      case 110:
        Serial.print(F("\n\nP1F1 settings"));
        break;
      case 130:
        Serial.print(F("\n\nP1F2 settings"));
        break;
      case 150:
        Serial.print(F("\n\nP1F3 settings"));
        break;
      case 170:
        Serial.print(F("\n\nP1F4 settings"));
        break;
      case 190:
        Serial.print(F("\n\n..."));
        break;
      case 210:
        Serial.print(F("\n\nP2F1 settings"));
        break;
      case 230:
        Serial.print(F("\n\nP2F2 settings"));
        break;
      case 250:
        Serial.print(F("\n\nP2F3 settings"));
        break;
      case 270:
        Serial.print(F("\n\nP2F4 settings"));
        break;
      case 290:
        Serial.print(F("\n\n..."));
        break;
      case 310:
        Serial.print(F("\n\nP3F1 settings"));
        break;
      case 330:
        Serial.print(F("\n\nP3F2 settings"));
        break;
      case 350:
        Serial.print(F("\n\nP3F3 settings"));
        break;
      case 370:
        Serial.print(F("\n\nP3F4 settings"));
        break;
      case 390:
        Serial.print(F("\n\n..."));
        break;
      case 410:
        Serial.print(F("\n\nP4F1 settings"));
        break;
      case 430:
        Serial.print(F("\n\nP4F2 settings"));
        break;
      case 450:
        Serial.print(F("\n\nP4F3 settings"));
        break;
      case 470:
        Serial.print(F("\n\nP4F4 settings"));
        break;
      case 490:
        Serial.print(F("\n\n..."));
        break;
    }
    if (!(i % 5)) {
      Serial.print(F("\n"));
      Serial.print(i);
      Serial.print(F("\t\t"));
    }
    Serial.print(EEPROM.read(i));
    Serial.print(F("\t"));
  }
  Serial.print(F("\n"));
}