#define VERSION "1.0.0.8"
#define DEBUG_ENABLED 1

#include <DHT.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <SPI.h>
#include <avr/wdt.h>

#include "common_defs.h"
#include "eepromThings.h"
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

// touchscreen
#define YP A3  // must be an analog pin // LCD CS
#define XM A2  // must be an analog pin // LCD RS - COMMAND/DATA
#define YM 9   // can be a digital pin  // LCD D1
#define XP 8   // can be a digital pin  // LCD D0
#define RX 10  // resistance between x+ and x-

// puntos maximos y minimos de la pantalla tactil, contando el espacio
// no-dibujable
#define TS_MINX 120
#define TS_MINY 75
#define TS_MAXX 900
#define TS_MAXY 950

#define MINPRESSURE 10
#define MAXPRESSURE 1000

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

TouchScreen ts = TouchScreen(XP, YP, XM, YM, RX);

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

Programa pActivo;

fActiva fActivaSP;

uint16_t dias;  // dias que lleva la fase activa

float t;          // temperatura
float h;          // humedad
uint8_t hTierra;  // humedad tierra

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

DHT dht(31, DHT22);  // pin hardcoded

RTC_DS3231 rtc;

// tiempo que dura la rafaga de riego
const uint8_t riegoTiempo PROGMEM = 5;
// tiempo que dura la espera para que la tierra se humedezca
const uint8_t riegoTiempoEspera PROGMEM = 30;
uint8_t LASTRIEGOSTATE;  // ultimo estado de riego - esto es para actualizar la
                         // luz del dashboard

const uint8_t refreshFrames PROGMEM = 100;

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
    15-18 = UNIX timestamp % 86400 segundo del dia de inicio de luz
    19 = hora de inicio de iluminacion
    21 = minuto de inicio/fin de iluminacion
    22 = cantidad de ciclo
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
  [4000-4019] = fase 0
*/

// SETUP

void setup() {
  Serial.begin(9600);

#if DEBUG_ENABLED
  Serial.print(F("DEBUG "));
#endif

  Serial.print(F("v"));
  Serial.println(F(VERSION));

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
  }

  dht.begin();
  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(0);

  if (EEPROM.read(0) != 88) eeprom_hardReset();

  z1fActiva = EEPROM.read(10);
  z1fActivalast = z1fActiva;
  z1fSeleccionada = z1fActiva;
  EEPROM.get(11, fActivaSP.diaIniciodefase);
  EEPROM.get(15, fActivaSP.sLuz);

  eeprom_cargarPrograma(&pActivo);
  eeprom_cargarfActivaSP(&fActivaSP, z1fActiva);

  tft.fillScreen(BLACK);

  now = rtc.now();
  prevTime = now.unixtime() + 61;
  drawStartupScreen();
  delay(1500);
  HomeScreen();

  Serial.println(F("Starting Loop"));
}

// LOOP

void loop() {
  static uint16_t framecount;

  now = rtc.now();

#if DEBUG_ENABLED
  DEBUG();
#else
  readTH();
  hTierra = map(analogRead(A8), 0, 1023, 100, 0);
#endif

  tsMenu();

  // aca manejo el cambio de fases
  if (z1fActivalast != z1fActiva) {
    eeprom_cargarfActivaSP(&fActivaSP, z1fActiva);

    fActivaSP.diaIniciodefase = now.unixtime();
    fActivaSP.diaFindefase = fActivaSP.diaIniciodefase + fActivaSP.dias * 86400;

    fActivaSP.sLuz = now.unixtime() % 86400;
    fActivaSP.sFinLuz = (fActivaSP.sLuz + fActivaSP.hLuz * 3600) % 86400;

    EEPROM.update(10, z1fActiva);
    EEPROM.put(11, fActivaSP.diaIniciodefase);
    EEPROM.put(15, fActivaSP.sLuz);

    dias = 0;

    z1fActivalast = z1fActiva;

    if (currentScreen == 0) {
      HomeScreen();
    }
  }

  // funcionalidad
  if (z1fActiva != 0) {
    static uint32_t tRiegoBomba;
    static uint32_t tRiegoEspera;
    uint8_t tempAvg = (fActivaSP.temph + fActivaSP.templ) / 2;
    uint8_t humAvg = (fActivaSP.humh + fActivaSP.huml) / 2;
    fActivaSP.sFinLuz = (fActivaSP.sLuz + fActivaSP.hLuz * 3600) % 86400;
    fActivaSP.diaFindefase = fActivaSP.diaIniciodefase + fActivaSP.dias * 86400;

    eeprom_cargarfActivaSP(&fActivaSP, z1fActiva);

    if (t >= fActivaSP.temph) {
      PORTC &= ~HEATPIN;
      PORTC |= FANPIN;
    } else if (t <= fActivaSP.templ) {
      PORTC &= ~FANPIN;
      PORTC |= HEATPIN;
    } else if (t <= tempAvg) {
      PORTC &= ~FANPIN;
    } else if (t >= tempAvg) {
      PORTC &= ~HEATPIN;
    }

    if (h >= fActivaSP.humh) {
      PORTC &= ~VAPPIN;
      // PORTC |= FANPIN;
    } else if (h <= fActivaSP.huml) {
      // PORTC &= ~FANPIN;
      PORTC |= VAPPIN;
    } else if (h <= humAvg) {
      // PORTC &= ~FANPIN;
    } else if (h >= humAvg) {
      PORTC &= ~VAPPIN;
    }

    if (hTierra <= fActivaSP.riegol && (tRiegoEspera + tRiegoBomba) == 0) {
      tRiegoBomba = now.unixtime() + riegoTiempo;  // tiempo encendido
      PORTC |= RIEGOPIN;
    } else if (hTierra >= fActivaSP.riegoh) {
      tRiegoEspera = 0;
      tRiegoBomba = 0;
      PORTC &= ~RIEGOPIN;
    }

    if (tRiegoBomba && !tRiegoEspera) {
      if (now.unixtime() >= tRiegoBomba) {
        tRiegoBomba = 0;
        tRiegoEspera = now.unixtime() + riegoTiempoEspera;  // tiempo apagado
        PORTC &= ~RIEGOPIN;
      }
    }

    if (tRiegoEspera && !tRiegoBomba) {
      if (now.unixtime() >= tRiegoEspera) {
        tRiegoEspera = 0;
        tRiegoBomba = now.unixtime() + riegoTiempo;  // tiempo encendido
        PORTC |= RIEGOPIN;
      }
    }

    // si la hora de inicio es menor que la de fin, o sea, si empieza y termina
    // el mismo dia
    if (fActivaSP.sLuz < fActivaSP.sFinLuz) {
      if ((now.unixtime() % 86400) >= fActivaSP.sLuz &&
          (now.unixtime() % 86400) <= fActivaSP.sFinLuz) {
        PORTC |= LUZPIN;
      } else {
        PORTC &= ~LUZPIN;
      }
    }
    // si la hora de inicio es mayor, o sea, si termina despues de las 24h
    else if (fActivaSP.sLuz > fActivaSP.sFinLuz) {
      // si todavia no paso la medianoche
      if ((now.unixtime() % 86400) >= fActivaSP.sLuz &&
          (now.unixtime() % 86400) <= 86400) {
        PORTC |= LUZPIN;
      }
      // si ya paso la medianoche
      else if ((now.unixtime() % 86400) >= 0 &&
               (now.unixtime() % 86400) <= fActivaSP.sFinLuz) {
        PORTC |= LUZPIN;
      } else {
        PORTC &= ~LUZPIN;
      }
    }

    if (now.unixtime() >= fActivaSP.diaFindefase) {
      if (z1fActiva == 4) {
        if (fActivaSP.ciclos == 0) {
          z1fActiva = 1;
        } else if (fActivaSP.ciclos == 1) {
          z1fActiva = 0;
        } else {
          fActivaSP.ciclos--;
          z1fActiva = 1;
        }
      } else {
        z1fActiva++;
      }
    }

    dias = (now.unixtime() - fActivaSP.diaIniciodefase) / 86400;
  } else if (z1fActiva == 0) {
    PORTC &= ~(FANPIN | HEATPIN | VAPPIN | RIEGOPIN | LUZPIN);
  }

  // aca actualizo el dashboard
  if (currentScreen == 0 && !(framecount % refreshFrames)) {
    static uint16_t lastDias = 0xffff;
    static float lastT = 0xff;
    static float lastH = 0xff;
    static uint8_t lastLuz;
    static uint8_t lasthTierra = 0xff;

    if (lastLuz != (PINC & LUZPIN)) {
      lastLuz = (PINC & LUZPIN);
      if (PINC & LUZPIN) {
        tft.fillCircle(180, 69, 10, GREEN);
      } else {
        tft.fillCircle(180, 69, 10, LIGHTGREY);
      }
    }

    if (lastDias != dias) {
      lastDias = dias;
      sprintf_P(buffer, PSTR("%d"), dias);
      tft.setCursor(125 - (strlen(buffer) * 18), 230);
      tft.setTextSize(3);
      tft.setTextColor(WHITE, BLACK);
      tft.print(buffer);
    }

    if (lasthTierra != hTierra || LASTRIEGOSTATE != (PINC & RIEGOPIN)) {
      lasthTierra = hTierra;
      sprintf_P(buffer, PSTR("%d"), hTierra);
      strcat_P(buffer, PSTR("%"));
      tft.setCursor(230 - (strlen(buffer) * 18), 230);
      tft.setTextSize(3);
      tft.setTextColor(WHITE, BLACK);
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
      strcat_P(buffer, PSTR("C"));
      tft.setCursor(125 - (strlen(buffer) * 18), 285);
      tft.setTextSize(3);
      tft.setTextColor(WHITE, BLACK);
      tft.print(buffer);  // temperatura leida por el DHT

      if (PINC & FANPIN && !(PINC & HEATPIN)) {
        tft.fillCircle(180, 94, 10, YELLOW);
      } else if (PINC & HEATPIN && !(PINC & FANPIN)) {
        tft.fillCircle(180, 94, 10, BLUE);
      } else if (!(PINC & HEATPIN) && !(PINC & FANPIN)) {
        tft.fillCircle(180, 94, 10, LIGHTGREY);
      }
    }

    if (lastH != h) {
      lastH = h;
      sprintf_P(buffer, PSTR("%d"), (uint8_t)h);
      strcat_P(buffer, PSTR("%"));
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

  // aca actualizo la hora en todas las pantallas excepto numpad
  if ((currentScreen != 255 &&
       (now.second() == 0 && now.unixtime() - prevTime >= 2)) ||
      prevScreen != currentScreen) {
    prevTime = now.unixtime();
    strcpy_P(buffer, PSTR("hh:mm"));
    now.toString(buffer);
    tft.setTextSize(2);
    tft.setTextColor(WHITE, BLACK);

    if (currentScreen == 0) {
      tft.setCursor(170, 165);
      tft.print(buffer);

      strcpy_P(buffer, PSTR("DD/MM/YY"));
      now.toString(buffer);
      tft.setCursor(134, 183);
      tft.print(buffer);
    } else {
      tft.setCursor(170, 10);
      tft.print(buffer);
    }
  }

  framecount++;
}

void readTH() {
  static uint32_t time;
  static uint32_t lastTime;

  time = millis();
  if (time - lastTime >= 2000) {
    t = dht.readTemperature();
    h = dht.readHumidity();
    lastTime = millis();
  }
}

#if DEBUG_ENABLED
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
        } else if (!strcmp_P(msg, PSTR("ERSH"))) {
          eeprom_read(1);
        } else if (!strcmp_P(msg, PSTR("ECLR"))) {
          eeprom_clear();
        } else if (!strcmp_P(msg, PSTR("ERST"))) {
          eeprom_hardReset();
        }
        break;
      case 'V':
        switch (atoi(msg + 1)) {
          case 0:
            Serial.print(F("now.unixtime(): \t\t"));
            Serial.println(now.unixtime());
            Serial.print(F("now.unixtime() % 86400: \t"));
            Serial.println(now.unixtime() % 86400);
            Serial.print(F("fActivaSP.diaIniciodefase: \t"));
            Serial.println(fActivaSP.diaIniciodefase);
            Serial.print(F("fActivaSP.diaFindefase: \t"));
            Serial.println(fActivaSP.diaFindefase);
            Serial.print(F("fActivaSP.sLuz: \t\t"));
            Serial.println(fActivaSP.sLuz);
            Serial.print(F("fActivaSP.sFinLuz: \t\t"));
            Serial.println(fActivaSP.sFinLuz);
            break;
          case 1:
            Serial.print(F("fActivaSP.dias: \t"));
            Serial.println(fActivaSP.dias);
            Serial.print(F("fActivaSP.hLuz: \t"));
            Serial.println(fActivaSP.hLuz);
            Serial.print(F("fActivaSP.templ: \t"));
            Serial.println(fActivaSP.templ);
            Serial.print(F("fActivaSP.temph: \t"));
            Serial.println(fActivaSP.temph);
            Serial.print(F("fActivaSP.riegoh: \t"));
            Serial.println(fActivaSP.riegol);
            Serial.print(F("fActivaSP.riegoh: \t"));
            Serial.println(fActivaSP.riegoh);
            Serial.print(F("fActivaSP.huml: \t"));
            Serial.println(fActivaSP.huml);
            Serial.print(F("fActivaSP.humh: \t"));
            Serial.println(fActivaSP.humh);
            break;
          case 2:
            Serial.print(F("Port C: \t0b"));
            Serial.println(PINC, BIN);
            Serial.print(F("Luz: \t\t"));
            Serial.println(PINC & LUZPIN, BIN);
            Serial.print(F("Fan: \t\t"));
            Serial.println(PINC & FANPIN, BIN);
            Serial.print(F("Heater: \t"));
            Serial.println(PINC & HEATPIN, BIN);
            Serial.print(F("Riego: \t\t"));
            Serial.println(PINC & RIEGOPIN, BIN);
            Serial.print(F("Vap: \t\t"));
            Serial.println(PINC & VAPPIN, BIN);
            break;
        }
        break;
    }

    strcpy(lastmsg, msg);
  }
}
#endif

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
    prevScreen = currentScreen;

    // scale from 0->1023 to tft.width
    /*
    Serial.print(F("Unmapped p: "));
    Serial.print(F("("));
    Serial.print(p.x);
    Serial.print(F(", "));
    Serial.print(p.y);
    Serial.print(F(", "));
    Serial.print(p.z);
    Serial.print(F(") "));
    */
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY - 60, tft.height(), 0);
    /*
    Serial.print(F("Mapped p: "));
    Serial.print(F("("));
    Serial.print(p.x);
    Serial.print(F(", "));
    Serial.print(p.y);
    Serial.print(F(", "));
    Serial.print(p.z);
    Serial.println(F(") "));
    */

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
          sprintf_P(buffer, PSTR("%d"), relojYYYY);
          tft.setCursor(28, 60);
          tft.print(buffer);
          if (relojMM == 2) {
            if ((relojYYYY % 4 == 0 && relojYYYY % 100 != 0) ||
                relojYYYY % 400 == 0) {
              if (relojDD > 29) {
                relojDD = 29;
                sprintf_P(buffer, PSTR("%02d"), relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
            } else {
              if (relojDD > 28) {
                relojDD = 28;
                sprintf_P(buffer, PSTR("%02d"), relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
            }
          }
        } else if ((p.x > 39 && p.x < 83) && (p.y > 81 && p.y < 114)) {
          if (relojYYYY > 1970) {
            relojYYYY--;
            sprintf_P(buffer, PSTR("%d"), relojYYYY);
            tft.setCursor(28, 60);
            tft.print(buffer);
          }
        } else if ((p.x > 114 && p.x < 158) && (p.y > 24 && p.y < 60)) {
          if (relojMM < 12) {
            relojMM++;
            sprintf_P(buffer, PSTR("%02d"), relojMM);
            tft.setCursor(28 + 72 + 20, 60);
            tft.print(buffer);
          } else {
            relojMM = 1;
            sprintf_P(buffer, PSTR("%02d"), relojMM);
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
                sprintf_P(buffer, PSTR("%02d"), relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 2:
              if ((relojYYYY % 4 == 0 && relojYYYY % 100 != 0) ||
                  relojYYYY % 400 == 0) {
                if (relojDD > 29) {
                  relojDD = 29;
                  sprintf_P(buffer, PSTR("%02d"), relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              } else {
                if (relojDD > 28) {
                  relojDD = 28;
                  sprintf_P(buffer, PSTR("%02d"), relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              }
              break;
          }
        } else if ((p.x > 114 && p.x < 158) && (p.y > 81 && p.y < 114)) {
          if (relojMM > 1) {
            relojMM--;
            sprintf_P(buffer, PSTR("%02d"), relojMM);
            tft.setCursor(28 + 72 + 20, 60);
            tft.print(buffer);
          } else {
            relojMM = 12;
            sprintf_P(buffer, PSTR("%02d"), relojMM);
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
                sprintf_P(buffer, PSTR("%02d"), relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 2:
              if ((relojYYYY % 4 == 0 && relojYYYY % 100 != 0) ||
                  relojYYYY % 400 == 0) {
                if (relojDD > 29) {
                  relojDD = 29;
                  sprintf_P(buffer, PSTR("%02d"), relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              } else {
                if (relojDD > 28) {
                  relojDD = 28;
                  sprintf_P(buffer, PSTR("%02d"), relojDD);
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
                sprintf_P(buffer, PSTR("%02d"), relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              } else {
                relojDD = 1;
                sprintf_P(buffer, PSTR("%02d"), relojDD);
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
                sprintf_P(buffer, PSTR("%02d"), relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              } else {
                relojDD = 1;
                sprintf_P(buffer, PSTR("%02d"), relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 2:
              if ((relojYYYY % 4 == 0 && relojYYYY % 100 != 0) ||
                  relojYYYY % 400 == 0) {
                if (relojDD < 29) {
                  relojDD++;
                  sprintf_P(buffer, PSTR("%02d"), relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                } else {
                  relojDD = 1;
                  sprintf_P(buffer, PSTR("%02d"), relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              } else {
                if (relojDD < 28) {
                  relojDD++;
                  sprintf_P(buffer, PSTR("%02d"), relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                } else {
                  relojDD = 1;
                  sprintf_P(buffer, PSTR("%02d"), relojDD);
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
                sprintf_P(buffer, PSTR("%02d"), relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              } else {
                relojDD = 31;
                sprintf_P(buffer, PSTR("%02d"), relojDD);
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
                sprintf_P(buffer, PSTR("%02d"), relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              } else {
                relojDD = 30;
                sprintf_P(buffer, PSTR("%02d"), relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 2:
              if ((relojYYYY % 4 == 0 && relojYYYY % 100 != 0) ||
                  relojYYYY % 400 == 0) {
                if (relojDD > 1) {
                  relojDD--;
                  sprintf_P(buffer, PSTR("%02d"), relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                } else {
                  relojDD = 29;
                  sprintf_P(buffer, PSTR("%02d"), relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              } else {
                if (relojDD > 1) {
                  relojDD--;
                  sprintf_P(buffer, PSTR("%02d"), relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                } else {
                  relojDD = 28;
                  sprintf_P(buffer, PSTR("%02d"), relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              }
              break;
          }
        } else if ((p.x > 67 && p.x < 111) && (p.y > 116 && p.y < 150)) {
          if (relojhh < 23) {
            relojhh++;
            sprintf_P(buffer, PSTR("%02d"), relojhh);
            tft.setCursor(74, 150);
            tft.print(buffer);
          } else {
            relojhh = 0;
            sprintf_P(buffer, PSTR("%02d"), relojhh);
            tft.setCursor(74, 150);
            tft.print(buffer);
          }
        } else if ((p.x > 67 && p.x < 111) && (p.y > 171 && p.y < 207)) {
          if (relojhh > 0) {
            relojhh--;
            sprintf_P(buffer, PSTR("%02d"), relojhh);
            tft.setCursor(74, 150);
            tft.print(buffer);
          } else {
            relojhh = 23;
            sprintf_P(buffer, PSTR("%02d"), relojhh);
            tft.setCursor(74, 150);
            tft.print(buffer);
          }
        } else if ((p.x > 124 && p.x < 168) && (p.y > 116 && p.y < 150)) {
          if (relojmm < 59) {
            relojmm++;
            sprintf_P(buffer, PSTR("%02d"), relojmm);
            tft.setCursor(74 + 36 + 20, 150);
            tft.print(buffer);
          } else {
            relojmm = 0;
            sprintf_P(buffer, PSTR("%02d"), relojmm);
            tft.setCursor(74 + 36 + 20, 150);
            tft.print(buffer);
          }
        } else if ((p.x > 124 && p.x < 168) && (p.y > 171 && p.y < 207)) {
          if (relojmm > 0) {
            relojmm--;
            sprintf_P(buffer, PSTR("%02d"), relojmm);
            tft.setCursor(74 + 36 + 20, 150);
            tft.print(buffer);
          } else {
            relojmm = 59;
            sprintf_P(buffer, PSTR("%02d"), relojmm);
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

              eeprom_cargarPrograma(&pActivo);

              Serial.println(F("Programa 1 cargado"));
              break;
            case 2:  // guardar
              EEPROM.put(110, pActivo.f1);
              EEPROM.put(130, pActivo.f2);
              EEPROM.put(150, pActivo.f3);
              EEPROM.put(170, pActivo.f4);

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

              eeprom_cargarPrograma(&pActivo);

              Serial.println(F("Programa 2 cargado"));
              break;
            case 2:  // guardar
              EEPROM.put(210, pActivo.f1);
              EEPROM.put(230, pActivo.f2);
              EEPROM.put(250, pActivo.f3);
              EEPROM.put(270, pActivo.f4);

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

              eeprom_cargarPrograma(&pActivo);

              Serial.println(F("Programa 3 cargado"));
              break;
            case 2:  // guardar
              EEPROM.put(310, pActivo.f1);
              EEPROM.put(330, pActivo.f2);
              EEPROM.put(350, pActivo.f3);
              EEPROM.put(370, pActivo.f4);

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

              eeprom_cargarPrograma(&pActivo);

              Serial.println(F("Programa 4 cargado"));
              break;
            case 2:  // guardar
              EEPROM.put(410, pActivo.f1);
              EEPROM.put(430, pActivo.f2);
              EEPROM.put(450, pActivo.f3);
              EEPROM.put(470, pActivo.f4);

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
          NumericKeyboardScreen(&fActivaSP.ciclos, 22, 2, "Ciclos");
        } else if (z1TerminarConfirmar == 1 &&
                   z1ControlButtons[3].contains(p.x, p.y)) {
          for (uint8_t i = 10; i < 29; i++) {
            EEPROM.update(i, 0x00);
          }
          EEPROM.update(22, fActivaSP.ciclos);
          Serial.println(F("EEPROM 10 to 29 cleared to 0x00\n"));

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
          Z1ControlScreen();
        }
        break;
      case 33:
        if (z1f1Buttons[4].contains(p.x, p.y)) {
          Z1Screen();
        } else if (z1f1Buttons[0].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f1.dias, 30, 3, "Dias");
        } else if (z1f1Buttons[1].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f1.hLuz, 32, 2, "Horas luz");
        } else if (z1f1Buttons[6].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f1.templ, 33, 2, "Temp baja");
        } else if (z1f1Buttons[5].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f1.temph, 34, 2, "Temp alta");
        } else if (z1f1Buttons[8].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f1.riegol, 35, 2, "Riego bajo");
        } else if (z1f1Buttons[7].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f1.riegoh, 36, 2, "Riego alto");
        } else if (z1f1Buttons[11].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f1.huml, 37, 2, "Hum baja");
        } else if (z1f1Buttons[10].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f1.humh, 38, 2, "Hum alta");
        }

        break;
      case 34:
        if (z1f2Buttons[4].contains(p.x, p.y)) {
          Z1Screen();
        } else if (z1f2Buttons[0].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f2.dias, 50, 3, "Dias");
        } else if (z1f2Buttons[1].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f2.hLuz, 52, 2, "Horas luz");
        } else if (z1f2Buttons[6].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f2.templ, 53, 2, "Temp baja");
        } else if (z1f2Buttons[5].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f2.temph, 54, 2, "Temp alta");
        } else if (z1f2Buttons[8].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f2.riegol, 55, 2, "Riego bajo");
        } else if (z1f2Buttons[7].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f2.riegoh, 56, 2, "Riego alto");
        } else if (z1f2Buttons[11].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f2.huml, 57, 2, "Hum baja");
        } else if (z1f2Buttons[10].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f2.humh, 58, 2, "Hum alta");
        }
        break;
      case 35:
        if (z1f3Buttons[4].contains(p.x, p.y)) {
          Z1Screen();
        } else if (z1f3Buttons[0].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f3.dias, 70, 3, "Dias");
        } else if (z1f3Buttons[1].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f3.hLuz, 72, 2, "Horas luz");
        } else if (z1f3Buttons[6].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f3.templ, 73, 2, "Temp baja");
        } else if (z1f3Buttons[5].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f3.temph, 74, 2, "Temp alta");
        } else if (z1f3Buttons[8].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f3.riegol, 75, 2, "Riego bajo");
        } else if (z1f3Buttons[7].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f3.riegoh, 76, 2, "Riego alto");
        } else if (z1f3Buttons[11].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f3.huml, 77, 2, "Hum baja");
        } else if (z1f3Buttons[10].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f3.humh, 78, 2, "Hum alta");
        }
        break;
      case 36:
        if (z1f4Buttons[4].contains(p.x, p.y)) {
          Z1Screen();
        } else if (z1f4Buttons[0].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f4.dias, 90, 3, "Dias");
        } else if (z1f4Buttons[1].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f4.hLuz, 92, 2, "Horas luz");
        } else if (z1f4Buttons[6].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f4.templ, 93, 2, "Temp baja");
        } else if (z1f4Buttons[5].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f4.temph, 94, 2, "Temp alta");
        } else if (z1f4Buttons[8].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f4.riegol, 95, 2, "Riego bajo");
        } else if (z1f4Buttons[7].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f4.riegoh, 96, 2, "Riego alto");
        } else if (z1f4Buttons[11].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f4.huml, 97, 2, "Hum baja");
        } else if (z1f4Buttons[10].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f4.humh, 98, 2, "Hum alta");
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
          switch (numKBPrevScreen) {
            case 33:
              Z1F1Screen();
              break;
            case 34:
              Z1F2Screen();
              break;
            case 35:
              Z1F3Screen();
              break;
            case 36:
              Z1F4Screen();
              break;
            case 31:
              Z1ControlScreen();
              break;
          }
        } else if (numericKeyboardButtons[13].contains(p.x, p.y)) {
          if (numKBvarptr8b != NULL) {
            *numKBvarptr8b = atoi(numKBstr);
            EEPROM.put(numKBeeprom, (uint8_t)atoi(numKBstr));
          } else if (numKBvarptr16b != NULL) {
            *numKBvarptr16b = atoi(numKBstr);
            EEPROM.put(numKBeeprom, atoi(numKBstr));
          }
          switch (numKBPrevScreen) {
            case 33:
              Z1F1Screen();
              break;
            case 34:
              Z1F2Screen();
              break;
            case 35:
              Z1F3Screen();
              break;
            case 36:
              Z1F4Screen();
              break;
            case 31:
              Z1ControlScreen();
              break;
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
}

void MenuScreen() {
  currentScreen = 1;
  drawMenuScreen();
}

void AjustesScreen() {
  currentScreen = 2;
  drawAjustesScreen();
}

void AlarmasScreen() {
  currentScreen = 3;
  drawAlarmasScreen();
}

void RelojScreen() {
  currentScreen = 4;
  drawRelojScreen();
}

void ProgramasScreen() {
  currentScreen = 5;
  drawProgramasScreen();
}

void Programa1Screen() {
  currentScreen = 7;
  drawPrograma1Screen();
}

void Programa2Screen() {
  currentScreen = 8;
  drawPrograma2Screen();
}

void Programa3Screen() {
  currentScreen = 9;
  drawPrograma3Screen();
}

void Programa4Screen() {
  currentScreen = 10;
  drawPrograma4Screen();
}

void ResetScreen() {
  currentScreen = 6;
  drawResetScreen();
}

void Z1Screen() {
  currentScreen = 30;
  drawZ1Screen();
}

void Z1ControlScreen() {
  currentScreen = 31;
  drawZ1ControlScreen();
}

void Z1InicioScreen() {
  currentScreen = 32;
  z1fSeleccionada = z1fActiva;
  drawZ1InicioScreen();
}

void Z1F1Screen() {
  currentScreen = 33;
  drawZ1F1Screen();
}

void Z1F2Screen() {
  currentScreen = 34;
  drawZ1F2Screen();
}

void Z1F3Screen() {
  currentScreen = 35;
  drawZ1F3Screen();
}

void Z1F4Screen() {
  currentScreen = 36;
  drawZ1F4Screen();
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
  sprintf_P(numKBstr, PSTR("%d"), *intptr);
  numKBvarptr16b = NULL;
  numKBvarptr8b = intptr;
  numKBeeprom = eepromdir;
  numKBbufferSize = bufferSize;
  drawNumericKeyboardScreen(title);
}

void NumericKeyboardScreen(uint16_t* intptr, uint16_t eepromdir,
                           uint8_t bufferSize, const char* title) {
  currentScreen = 255;
  sprintf_P(numKBstr, PSTR("%d"), *intptr);
  numKBvarptr16b = intptr;
  numKBvarptr8b = NULL;
  numKBeeprom = eepromdir;
  numKBbufferSize = bufferSize;
  drawNumericKeyboardScreen(title);
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
    tft.fillCircle(180, 94, 10, YELLOW);
  } else if (PINC & HEATPIN && !(PINC & FANPIN)) {
    tft.fillCircle(180, 94, 10, BLUE);
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

  eeprom_cargarfActivaSP(&fActivaSP, z1fActiva);

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
  // ajustesButtons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
  //                               "Alarmas", BUTTON_TEXTSIZE);
  // ajustesButtons[0].drawButton();

  // boton 2 - reloj
  ajustesButtons[1].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                                 "Reloj", BUTTON_TEXTSIZE);
  ajustesButtons[1].drawButton();

  // boton 3 - programas
  ajustesButtons[2].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                                 "Programas", BUTTON_TEXTSIZE);
  ajustesButtons[2].drawButton();

  // boton 4 - hard reset
  ajustesButtons[3].initButtonUL(&tft, 5, 125, 230, 40, WHITE, ORANGE, YELLOW,
                                 "H. Reset", BUTTON_TEXTSIZE);
  ajustesButtons[3].drawButton();

  // boton 5 - Volver
  ajustesButtons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                                 "Volver", BUTTON_TEXTSIZE);
  ajustesButtons[4].drawButton();
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

  sprintf_P(buffer, PSTR("%02d"), relojMM);
  tft.setCursor(28 + 72 + 20, 60);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%02d"), relojDD);
  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
  tft.print(buffer);

  tft.fillTriangle(61, 107, 49, 91, 73, 91, ORANGE);     // año--
  tft.fillTriangle(136, 107, 124, 91, 148, 91, ORANGE);  // mes--
  tft.fillTriangle(192, 107, 180, 91, 204, 91, ORANGE);  // dia--

  tft.fillTriangle(89, 124, 77, 140, 101, 140, ORANGE);    // hora++
  tft.fillTriangle(146, 124, 134, 140, 158, 140, ORANGE);  // minuto++

  sprintf_P(buffer, PSTR("%02d"), relojhh);
  tft.setCursor(74, 150);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%02d"), relojmm);
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
  sprintf_P(buffer, PSTR("%d"), pActivo.f1.dias);

  tft.setCursor(228 - (strlen(buffer) * 18), 45);
  tft.print(buffer);

  //// hluz
  if (pActivo.f1.hLuz > 24) pActivo.f1.hLuz = 24;
  sprintf_P(buffer, PSTR("%d"), pActivo.f1.hLuz);
  strcat_P(buffer, PSTR("H"));
  tft.setCursor(228 - (strlen(buffer) * 18), 90);
  tft.print(buffer);

  //// temp
  if (pActivo.f1.temph > 80) pActivo.f1.temph = 80;
  sprintf_P(buffer, PSTR("%d"), pActivo.f1.temph);
  uint8_t z1f1temphSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("C"));
  tft.setCursor(228 - (strlen(buffer) * 18), 135);
  tft.print(buffer);

  if (pActivo.f1.templ > 80) pActivo.f1.templ = 80;
  sprintf_P(buffer, PSTR("%d"), pActivo.f1.templ);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f1temphSTRlen * 18), 135);
  tft.print(buffer);

  tft.setCursor(192 - (z1f1temphSTRlen * 18), 135);
  tft.print(F("-"));

  //// riego
  sprintf_P(buffer, PSTR("%d"), pActivo.f1.riegoh);
  uint8_t z1f1riegohSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 180);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), pActivo.f1.riegol);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f1riegohSTRlen * 18), 180);
  tft.print(buffer);

  tft.setCursor(192 - (z1f1riegohSTRlen * 18), 180);
  tft.print(F("-"));

  //// humedad
  sprintf_P(buffer, PSTR("%d"), pActivo.f1.humh);
  uint8_t z1f1humhSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 225);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), pActivo.f1.huml);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f1humhSTRlen * 18), 225);
  tft.print(buffer);

  tft.setCursor(192 - (z1f1humhSTRlen * 18), 225);
  tft.print(F("-"));

  z1f1Buttons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                              "Volver", BUTTON_TEXTSIZE);
  z1f1Buttons[4].drawButton();
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
  sprintf_P(buffer, PSTR("%d"), pActivo.f2.dias);

  tft.setCursor(228 - (strlen(buffer) * 18), 45);
  tft.print(buffer);

  //// hluz
  if (pActivo.f2.hLuz > 24) pActivo.f2.hLuz = 24;
  sprintf_P(buffer, PSTR("%d"), pActivo.f2.hLuz);
  strcat_P(buffer, PSTR("H"));
  tft.setCursor(228 - (strlen(buffer) * 18), 90);
  tft.print(buffer);

  //// temp
  if (pActivo.f2.temph > 80) pActivo.f2.temph = 80;
  sprintf_P(buffer, PSTR("%d"), pActivo.f2.temph);
  uint8_t z1f2temphSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("C"));
  tft.setCursor(228 - (strlen(buffer) * 18), 135);
  tft.print(buffer);

  if (pActivo.f2.templ > 80) pActivo.f2.templ = 80;
  sprintf_P(buffer, PSTR("%d"), pActivo.f2.templ);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f2temphSTRlen * 18), 135);
  tft.print(buffer);

  tft.setCursor(192 - (z1f2temphSTRlen * 18), 135);
  tft.print(F("-"));

  //// riego
  sprintf_P(buffer, PSTR("%d"), pActivo.f2.riegoh);
  uint8_t z1f2riegohSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 180);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), pActivo.f2.riegol);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f2riegohSTRlen * 18), 180);
  tft.print(buffer);

  tft.setCursor(192 - (z1f2riegohSTRlen * 18), 180);
  tft.print(F("-"));

  //// humedad
  sprintf_P(buffer, PSTR("%d"), pActivo.f2.humh);
  uint8_t z1f2humhSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 225);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), pActivo.f2.huml);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f2humhSTRlen * 18), 225);
  tft.print(buffer);

  tft.setCursor(192 - (z1f2humhSTRlen * 18), 225);
  tft.print(F("-"));

  z1f2Buttons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                              "Volver", BUTTON_TEXTSIZE);
  z1f2Buttons[4].drawButton();
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
  sprintf_P(buffer, PSTR("%d"), pActivo.f3.dias);

  tft.setCursor(228 - (strlen(buffer) * 18), 45);
  tft.print(buffer);

  //// hluz
  if (pActivo.f3.hLuz > 24) pActivo.f3.hLuz = 24;
  sprintf_P(buffer, PSTR("%d"), pActivo.f3.hLuz);
  strcat_P(buffer, PSTR("H"));
  tft.setCursor(228 - (strlen(buffer) * 18), 90);
  tft.print(buffer);

  //// temp
  if (pActivo.f3.temph > 80) pActivo.f3.temph = 80;
  sprintf_P(buffer, PSTR("%d"), pActivo.f3.temph);
  uint8_t z1f3temphSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("C"));
  tft.setCursor(228 - (strlen(buffer) * 18), 135);
  tft.print(buffer);

  if (pActivo.f3.templ > 80) pActivo.f3.templ = 80;
  sprintf_P(buffer, PSTR("%d"), pActivo.f3.templ);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f3temphSTRlen * 18), 135);
  tft.print(buffer);

  tft.setCursor(192 - (z1f3temphSTRlen * 18), 135);
  tft.print(F("-"));

  //// riego
  sprintf_P(buffer, PSTR("%d"), pActivo.f3.riegoh);
  uint8_t z1f3riegohSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 180);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), pActivo.f3.riegol);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f3riegohSTRlen * 18), 180);
  tft.print(buffer);

  tft.setCursor(192 - (z1f3riegohSTRlen * 18), 180);
  tft.print(F("-"));

  //// humedad
  sprintf_P(buffer, PSTR("%d"), pActivo.f3.humh);
  uint8_t z1f3humhSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 225);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), pActivo.f3.huml);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f3humhSTRlen * 18), 225);
  tft.print(buffer);

  tft.setCursor(192 - (z1f3humhSTRlen * 18), 225);
  tft.print(F("-"));

  z1f3Buttons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                              "Volver", BUTTON_TEXTSIZE);
  z1f3Buttons[4].drawButton();
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
  sprintf_P(buffer, PSTR("%d"), pActivo.f4.dias);

  tft.setCursor(228 - (strlen(buffer) * 18), 45);
  tft.print(buffer);

  //// hluz
  if (pActivo.f4.hLuz > 24) pActivo.f4.hLuz = 24;
  sprintf_P(buffer, PSTR("%d"), pActivo.f4.hLuz);
  strcat_P(buffer, PSTR("H"));
  tft.setCursor(228 - (strlen(buffer) * 18), 90);
  tft.print(buffer);

  //// temp
  if (pActivo.f4.temph > 80) pActivo.f4.temph = 80;
  sprintf_P(buffer, PSTR("%d"), pActivo.f4.temph);
  uint8_t z1f4temphSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("C"));
  tft.setCursor(228 - (strlen(buffer) * 18), 135);
  tft.print(buffer);

  if (pActivo.f4.templ > 80) pActivo.f4.templ = 80;
  sprintf_P(buffer, PSTR("%d"), pActivo.f4.templ);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f4temphSTRlen * 18), 135);
  tft.print(buffer);

  tft.setCursor(192 - (z1f4temphSTRlen * 18), 135);
  tft.print(F("-"));

  //// riego
  sprintf_P(buffer, PSTR("%d"), pActivo.f4.riegoh);
  uint8_t z1f4riegohSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 180);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), pActivo.f4.riegol);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f4riegohSTRlen * 18), 180);
  tft.print(buffer);

  tft.setCursor(192 - (z1f4riegohSTRlen * 18), 180);
  tft.print(F("-"));

  //// humedad
  sprintf_P(buffer, PSTR("%d"), pActivo.f4.humh);
  uint8_t z1f4humhSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 225);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), pActivo.f4.huml);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f4humhSTRlen * 18), 225);
  tft.print(buffer);

  tft.setCursor(192 - (z1f4humhSTRlen * 18), 225);
  tft.print(F("-"));

  z1f4Buttons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                              "Volver", BUTTON_TEXTSIZE);
  z1f4Buttons[4].drawButton();
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

  fActivaSP.ciclos = EEPROM.read(22);

  sprintf_P(buffer, PSTR("%d"), fActivaSP.ciclos);
  tft.setCursor(228 - (strlen(buffer) * 18), 135);
  tft.print(buffer);

  z1ControlButtons[2].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY,
                                   WHITE, "Volver", BUTTON_TEXTSIZE);
  z1ControlButtons[2].drawButton();
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
}

void drawNumericKeyboardScreen(const char* title) {
  numKBPrevScreen = prevScreen;

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
  int16_t x1, y1;
  uint16_t w, h;

  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);

  tft.getTextBounds(F("Silics"), 120, 139, &x1, &y1, &w, &h);
  tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("Silics"));

  tft.setCursor(120 - (6 * 3 * 10) / 2, 170);
  // tft.getTextBounds(F("GrowOS 0.9"), 120, 170, &x1, &y1, &w, &h); // no se
  // por qué no funciona esto tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("GrowOS 1.0"));
}

void drawGoodbyeScreen() {
  int16_t x1, y1;
  uint16_t w, h;

  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);

  tft.getTextBounds(F("^_^"), 120, 160, &x1, &y1, &w, &h);
  tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("^_^"));
}
