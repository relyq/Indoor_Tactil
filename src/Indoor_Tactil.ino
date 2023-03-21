#define VERSION "1.0.0.12"

#include <EEPROM.h>
#include <RTClib.h>
#include <SPI.h>
#include <avr/wdt.h>

#include "src/DHT.h"
// DHT.h clashes with my macros
#undef DEBUG_PRINT
#undef DEBUG_PRINTLN
#include "DEBUG_things.h"
#include "OUTPUT_things.h"
#include "common_defs.h"
#include "draw_screens.h"
#include "eepromThings.h"
#include "src/Adafruit_GFX.h"     // Core graphics library
#include "src/Adafruit_TFTLCD.h"  // Hardware-specific library
#include "src/TouchScreen.h"
#include "touchscreen_config.h"

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
Adafruit_GFX_Button calibracionButtons[2];
Adafruit_GFX_Button calibTierraButtons[3];
Adafruit_GFX_Button relojButtons[2];
Adafruit_GFX_Button programasButtons[5];
Adafruit_GFX_Button programa1Buttons[5];
Adafruit_GFX_Button programa2Buttons[5];
Adafruit_GFX_Button programa3Buttons[5];
Adafruit_GFX_Button programa4Buttons[5];
Adafruit_GFX_Button resetButtons[2];
Adafruit_GFX_Button numericKeyboardButtons[16];

enum Screens currentScreen;  // acá guardo la pantalla activa
enum Screens prevScreen;     // acá guardo la pantalla anterior

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
float dias_prox_riego;

float t;          // temperatura
float h;          // humedad
uint8_t hTierra;  // humedad tierra
int16_t soil;     // valor del sensor de humedad
uint16_t tMin;
uint16_t tMax;

// ins
const uint8_t SENSORTIERRAPIN PROGMEM = A8;
const uint8_t DHTPIN PROGMEM = 0x40;  // pin 31 // PC6

// outs
const uint8_t HEATPIN PROGMEM = 0x20;   // pin 32 // PC5
const uint8_t FANPIN PROGMEM = 0x10;    // pin 33 // PC4
const uint8_t LUZPIN PROGMEM = 0x04;    // pin 35 // PC3
const uint8_t VAPPIN PROGMEM = 0x08;    // pin 34 // PC2
const uint8_t RIEGOPIN PROGMEM = 0x01;  // pin 37 // PC0

/*
// supongo que asi se hace convencionalmente // y debería hacerlo así

#define HEATPIN PC5
#define FANPIN PC4
// etc...
*/

char numKBstr[10];  // aca guardo la str que estoy modificando en el teclado
                    // numerico
enum Screens numKBPrevScreen;  // la pantalla a la que tengo que volver

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

Reloj reloj;

char buffer[50];  // buffer para mostrar cosas en pantalla

DHT dht(31, DHT22);  // pin hardcoded

RTC_DS3231 rtc;

// tiempo que dura la rafaga de riego
const uint8_t riegoTiempo PROGMEM = 2;
// tiempo que dura la espera para que la tierra se humedezca
const uint8_t riegoTiempoEspera PROGMEM = 5;
uint8_t LASTRIEGOSTATE;  // ultimo estado de riego - esto es para actualizar la
                         // luz del dashboard

volatile static uint16_t framecount;
const uint8_t refreshFrames PROGMEM = 100;

uint8_t lcd_rotation = 0;

/*
  ESTOY GUARDANDO fActivaSP.sProx_riego EN 3900 PORQUE ME QUEDE SIN ESPACIO AL
  PRINCIPIO TENGO QUE CAMBIAR TODO EL LAYOUT DE LA EEPROM PARA ARREGLARLO
  EEPROM:
  [0] = magic number
  [1-9] = id, device info, etc
  [10-29] = fase activa, info fase activa, ciclos
    10 = fase activa
    11-14 = UNIX timestamp inicio de fase
    15-18 = UNIX timestamp % 86400 segundo del dia de inicio de luz
    19 = hora de inicio de iluminacion
    21 = minuto de inicio/fin de iluminacion
    22 = cantidad de ciclos
    26-27 = calibracion tierra seca / tMax
    28-29 = calibracion tierra mojada / tMin
    3900-3903 = UNIX timestamp prox riego
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
      39 = riego dias
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
  [3900-3903] = UNIX timestamp prox riego
  [4000-4019] = fase 0
*/

// SETUP

void setup() {
  DEBUG_BEGIN(9600);

  DEBUG_PRINT(F("DEBUG "));
  DEBUG_PRINT(F("v"));
  DEBUG_PRINTLN(F(VERSION));

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

  uint16_t lcd_id = tft.readID();

  switch (lcd_id) {
    case 0x9325: {
      lcd_rotation = 2;
      break;
    }
    case 0x9341: {
      lcd_rotation = 0;
      break;
    }
    case 0xC0C0: {  // 0x9341
      DEBUG_PRINT(F("UNRECOGNIZED DRIVER (0x"));
      DEBUG_PRINTHEX(lcd_id);
      DEBUG_PRINTLN(F(") USING 0x9341 DRIVER"));
      lcd_id = 0x9341;
      lcd_rotation = 0;
      break;
    }
    default: {
      DEBUG_PRINT(F("UNRECOGNIZED DRIVER (0x"));
      DEBUG_PRINTHEX(lcd_id);
      DEBUG_PRINTLN(F(") USING 0x9341 DRIVER"));
      lcd_id = 0x9341;
      lcd_rotation = 0;
      break;
    }
  }

  DEBUG_PRINT(F("LCD driver reported by screen: 0x"));
  DEBUG_PRINTLNHEX(lcd_id);
  DEBUG_PRINT(F("TFT size is "));
  DEBUG_PRINT(tft.width());
  DEBUG_PRINT(F("x"));
  DEBUG_PRINTLN(tft.height());

  dht.begin();
  tft.reset();
  tft.begin(lcd_id);
  tft.setRotation(lcd_rotation);

  if (EEPROM.read(0) != 90) eeprom_hardReset(&tft);

  z1fActiva = EEPROM.read(10);
  z1fActivalast = z1fActiva;
  z1fSeleccionada = z1fActiva;
  EEPROM.get(11, fActivaSP.diaIniciodefase);
  EEPROM.get(15, fActivaSP.sLuz);
  EEPROM.get(3900, fActivaSP.sProx_riego);

  EEPROM.get(26, tMin);
  EEPROM.get(28, tMax);

  eeprom_cargarPrograma(&pActivo);
  eeprom_cargarfActivaSP(&fActivaSP, z1fActiva);

  tft.fillScreen(BLACK);

#if DEBUG_ENABLED
  now = DateTime(F(__DATE__), F(__TIME__));
#else
  if (!rtc.begin()) {
    DEBUG_PRINTLN(F("Couldn't find RTC"));
    while (1)
      ;
  }

  if (rtc.lostPower()) {
    DEBUG_PRINTLN(F("RTC lost power, lets set the time!"));
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  now = rtc.now();
#endif  // DEBUG_ENABLED

  prevTime = now.unixtime() + 61;
  drawStartupScreen(&tft);
  delay(1500);
  HomeScreen();

  DEBUG_PRINTLN(F("Starting Loop"));
}

// LOOP

void loop() {
#if DEBUG_ENABLED
  DEBUG();
  // fake RTC
  static uint32_t last_millis = millis();
  // si paso un segundo o más, actualizar el tiempo
  if (millis() - last_millis >= 1000) {
    now = now + ((millis() - last_millis) / 1000);
    last_millis = millis();
  }
#else
  now = rtc.now();
  readTH();
  // readSoil();
#endif  // DEBUG_ENABLED

  // acá manejo la pantalla tactil
  tsMenu();

  // acá manejo el cambio de fases
  if (z1fActivalast != z1fActiva) {
    eeprom_cargarfActivaSP(&fActivaSP, z1fActiva);

    fActivaSP.diaIniciodefase = now.unixtime();
    fActivaSP.diaFindefase =
        fActivaSP.diaIniciodefase + (uint32_t)fActivaSP.dias * 86400;

    fActivaSP.sLuz = now.unixtime() % 86400;
    fActivaSP.sFinLuz =
        (fActivaSP.sLuz + (uint32_t)fActivaSP.hLuz * 3600) % 86400;

    fActivaSP.sProx_riego = now.unixtime();  // regar por primera vez ahora

    EEPROM.update(10, z1fActiva);
    EEPROM.put(11, fActivaSP.diaIniciodefase);
    EEPROM.put(15, fActivaSP.sLuz);
    EEPROM.put(3900, fActivaSP.sProx_riego);

    if (z1fActiva == 0) {
      dias_prox_riego = 0;
    } else {
      dias_prox_riego = (float)(fActivaSP.sProx_riego - now.unixtime()) / 86400;
    }

    dias = 0;

    z1fActivalast = z1fActiva;

    if (currentScreen == Screens::Home) {
      HomeScreen();
    }
  }

  // funcionalidad
  if (z1fActiva != 0) {
    // no deberia leer la eeprom todo el tiempo
    eeprom_cargarfActivaSP(&fActivaSP, z1fActiva);

    static uint32_t tRiegoBomba;
    static uint32_t tRiegoEspera;

    uint8_t tempAvg = (fActivaSP.temph + fActivaSP.templ) / 2;
    uint8_t humAvg = (fActivaSP.humh + fActivaSP.huml) / 2;
    fActivaSP.sFinLuz =
        (fActivaSP.sLuz + (uint32_t)fActivaSP.hLuz * 3600) % 86400;
    fActivaSP.diaFindefase =
        fActivaSP.diaIniciodefase + (uint32_t)fActivaSP.dias * 86400;

    if (t >= fActivaSP.temph) {
      FAN_ON();
      HEAT_OFF();
    } else if (t <= fActivaSP.templ) {
      FAN_OFF();
      HEAT_ON();
    } else if (t <= tempAvg) {
      FAN_OFF();
    } else if (t >= tempAvg) {
      HEAT_OFF();
    }

    if (h >= fActivaSP.humh) {
      VAP_OFF();
    } else if (h <= fActivaSP.huml) {
      VAP_ON();
    } else if (h <= humAvg) {
    } else if (h >= humAvg) {
      VAP_OFF();
    }

    // riego por tiempo en vez de medicion
    /*
    if (hTierra <= fActivaSP.riegol && (tRiegoEspera + tRiegoBomba) == 0) {
      tRiegoBomba = now.unixtime() + riegoTiempo;  // tiempo encendido
      RIEGO_ON();
    } else if (hTierra >= fActivaSP.riegoh) {
      tRiegoEspera = 0;
      tRiegoBomba = 0;
      RIEGO_OFF();
    }
    */

    static uint8_t contador_riego;
    uint8_t const contador_riego_max = 5;
    if (now.unixtime() >= fActivaSP.sProx_riego) {
      // iniciar riego
      contador_riego = 0;
      tRiegoBomba = now.unixtime() + riegoTiempo;  // tiempo encendido
      RIEGO_ON();

      // guardar proximo tiempo de riego
      fActivaSP.sProx_riego = now.unixtime() + (fActivaSP.riegod * 86400);
      EEPROM.put(3900, fActivaSP.sProx_riego);

    } else if (contador_riego >
               contador_riego_max) {  // cuantos ciclos de riego hacer
      // parar riego
      tRiegoEspera = 0;
      tRiegoBomba = 0;
      RIEGO_OFF();
    }

    // va despues porque si no se glitchea
    dias_prox_riego = (float)(fActivaSP.sProx_riego - now.unixtime()) / 86400;

    // esto maneja cuanto tiempo tiene que quedar prendido el riego
    if (tRiegoBomba && !tRiegoEspera) {
      if (now.unixtime() >= tRiegoBomba) {
        tRiegoBomba = 0;
        tRiegoEspera = now.unixtime() + riegoTiempoEspera;  // tiempo apagado
        RIEGO_OFF();
      }
    }

    if (tRiegoEspera && !tRiegoBomba) {
      if (now.unixtime() >= tRiegoEspera) {
        tRiegoEspera = 0;
        tRiegoBomba = now.unixtime() + riegoTiempo;  // tiempo encendido
        RIEGO_ON();
        contador_riego++;
      }
    }

    // si la hora de inicio es menor que la de fin, o sea, si empieza y termina
    // el mismo dia
    if (fActivaSP.sLuz < fActivaSP.sFinLuz) {
      if ((now.unixtime() % 86400) >= fActivaSP.sLuz &&
          (now.unixtime() % 86400) <= fActivaSP.sFinLuz) {
        LUZ_ON();
      } else {
        LUZ_OFF();
      }
    }
    // si la hora de inicio es mayor, o sea, si termina despues de las 24h
    else if (fActivaSP.sLuz > fActivaSP.sFinLuz) {
      // si todavia no paso la medianoche
      if ((now.unixtime() % 86400) >= fActivaSP.sLuz &&
          (now.unixtime() % 86400) <= 86400) {
        LUZ_ON();
      }
      // si ya paso la medianoche
      else if ((now.unixtime() % 86400) >= 0 &&
               (now.unixtime() % 86400) <= fActivaSP.sFinLuz) {
        LUZ_ON();
      } else {
        LUZ_OFF();
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
    SYSTEM_OFF();
  }

  // aca actualizo la pantalla de calibración
  static int16_t lastSoil = 0xffff;
  if (currentScreen == Screens::CalibTierra && !(framecount % refreshFrames) &&
      lastSoil != soil) {
    lastSoil = soil;
    tft.setTextSize(BUTTON_TEXTSIZE);
    sprintf_P(buffer, PSTR("%4d"), soil);
    tft.setCursor(230 - (strlen(buffer) * CHARACTER_WIDTH * 3),
                  ((100 - 10 + (CHARACTER_HEIGHT * TITLE_TEXTSIZE)) / 2));
    tft.setTextColor(WHITE, BLACK);
    tft.print(buffer);
  }
  // aca actualizo el dashboard
  if (currentScreen == Screens::Home && !(framecount % refreshFrames)) {
    static uint16_t lastDias = 0xffff;
    static float lastT = 0xff;
    static float lastH = 0xff;
    static uint8_t lastLuz;
    static uint8_t lasthTierra = 0xff;
    static float last_dias_riego = 0xff;  // dias que faltan para regar

    if (lastLuz != (PINC & LUZPIN)) {
      lastLuz = (PINC & LUZPIN);
      if (!(PINC & LUZPIN)) {
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

    /*
    if (lasthTierra != hTierra || LASTRIEGOSTATE != (PINC & RIEGOPIN)) {
      lasthTierra = hTierra;
      sprintf_P(buffer, PSTR("%2d"), hTierra);
      strcat_P(buffer, PSTR("%"));
      tft.setCursor(230 - (strlen(buffer) * 18), 230);
      tft.setTextSize(3);
      tft.setTextColor(WHITE, BLACK);
      tft.print(buffer);
      LASTRIEGOSTATE = (PINC & RIEGOPIN);

      if (!(PINC & RIEGOPIN)) {
        tft.fillCircle(180, 119, 10, GREEN);
      } else {
        tft.fillCircle(180, 119, 10, LIGHTGREY);
      }
    }
    */

    if ((last_dias_riego != dias_prox_riego) ||
        LASTRIEGOSTATE != (PINC & RIEGOPIN)) {
      last_dias_riego = dias_prox_riego;

      dtostrf(dias_prox_riego, 4, 2, buffer);
      tft.setTextSize(3);
      tft.setTextColor(WHITE, BLACK);
      tft.setCursor(230 - (strlen(buffer) * 18), 230);
      tft.print(buffer);

      LASTRIEGOSTATE = (PINC & RIEGOPIN);

      if (!(PINC & RIEGOPIN)) {
        tft.fillCircle(180, 119, 10, GREEN);
      } else {
        tft.fillCircle(180, 119, 10, LIGHTGREY);
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

      if (!(PINC & FANPIN)) {
        tft.fillCircle(180, 94, 10, GREEN);
      } else {
        tft.fillCircle(180, 94, 10, LIGHTGREY);
      }
    }
    if (lastH != h) {
      lastH = h;
      sprintf_P(buffer, PSTR("%2d"), (uint8_t)h);
      strcat_P(buffer, PSTR("%"));
      tft.setCursor(230 - (strlen(buffer) * 18), 285);
      tft.setTextSize(3);
      tft.setTextColor(WHITE, BLACK);
      tft.print(buffer);  // humedad leida por el DHT
      /*
        if (!(PINC & VAPPIN)) {
          tft.fillCircle(180, 119, 10, GREEN);
        } else {
          tft.fillCircle(180, 119, 10, LIGHTGREY);
        }
      */
    }
  }

  // aca actualizo la hora en todas las pantallas excepto numpad
  if (currentScreen != Screens::numKB &&
      ((now.second() == 0 && now.unixtime() - prevTime >= 2) ||
       prevScreen != currentScreen)) {
    prevTime = now.unixtime();
    strcpy_P(buffer, PSTR("hh:mm"));
    now.toString(buffer);
    tft.setTextSize(2);
    tft.setTextColor(WHITE, BLACK);

    if (currentScreen == Screens::Home) {
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

  prevScreen = currentScreen;

  framecount++;
}

void readTH() {
  static uint32_t lastTime;

  if (millis() - lastTime >= 2000) {
    t = dht.readTemperature();
    h = dht.readHumidity();
    lastTime = millis();
  }
}

void readSoil() {
  static uint32_t lastTime;

  if (millis() - lastTime >= 500) {
    soil = analogRead(SENSORTIERRAPIN);
    if (soil < tMin) {
      hTierra = 99;
    } else if (soil > tMax) {
      hTierra = 0;
    } else {
      hTierra = map(soil, tMin, tMax, 99, 0);
    }
    lastTime = millis();
  }
}

#if DEBUG_ENABLED
void DEBUG() {
  if (Serial.available() > 0) {
    char msg[5] = {0, 0, 0, 0, '\0'};
    Serial.readString().toCharArray(msg, 5);
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
          eeprom_hardReset(&tft);
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
            Serial.print(F("fActivaSP.riegod: \t\t"));
            Serial.println(fActivaSP.riegod);
            Serial.print(F("fActivaSP.sProx_riego: \t\t"));
            Serial.println(fActivaSP.sProx_riego);
            Serial.print(F("tMax: \t\t"));
            Serial.println(tMax);
            Serial.print(F("tMin: \t\t"));
            Serial.println(tMin);
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
          case 3:
            Serial.print(F("prevScreen: \t\t"));
            Serial.println((uint8_t)prevScreen);
            Serial.print(F("currentScreen: \t\t"));
            Serial.println((uint8_t)currentScreen);
            Serial.print(F("numKBPrevScreen: \t"));
            Serial.println((uint8_t)numKBPrevScreen);
            break;
        }
        break;
    }
  }
}
#endif  // DEBUG_ENABLED

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
    /*
    DEBUG_PRINT(F("Unmapped p: "));
    DEBUG_PRINT(F("("));
    DEBUG_PRINT(p.x);
    DEBUG_PRINT(F(", "));
    DEBUG_PRINT(p.y);
    DEBUG_PRINT(F(", "));
    DEBUG_PRINT(p.z);
    DEBUG_PRINT(F(") "));
    */
    if (lcd_rotation == 0) {
      p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
      p.y = map(p.y, TS_MINY, TS_MAXY - 60, tft.height(), 0);
    } else {
      p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
      p.y = map(p.y, TS_MINY, TS_MAXY - 60, 0, tft.height());
    }
    /*
    DEBUG_PRINT(F("Mapped p: "));
    DEBUG_PRINT(F("("));
    DEBUG_PRINT(p.x);
    DEBUG_PRINT(F(", "));
    DEBUG_PRINT(p.y);
    DEBUG_PRINT(F(", "));
    DEBUG_PRINT(p.z);
    DEBUG_PRINTLN(F(") "));
    */

    if (currentScreen != Screens::Home &&
        ((lcd_rotation == 0 && (p.y < 0)) ||
         (lcd_rotation == 2 && (p.y > 320)))) {
      HomeScreen();
    }

    switch (currentScreen) {
      case Screens::Home: {
        if (homeButtons[0].contains(p.x, p.y)) {
          MenuScreen();
        }
        break;
      }
      case Screens::Menu: {
        if (menuButtons[0].contains(p.x, p.y)) {
          Z1Screen();
        } else if (menuButtons[1].contains(p.x, p.y)) {
          AjustesScreen();
        } else if (menuButtons[2].contains(p.x, p.y)) {
          HomeScreen();
        }
        break;
      }
      case Screens::Ajustes: {
        if (ajustesButtons[4].contains(p.x, p.y)) {
          MenuScreen();
        } else if (ajustesButtons[1].contains(p.x, p.y)) {
          RelojScreen();
        } else if (ajustesButtons[2].contains(p.x, p.y)) {
          ProgramasScreen();
        } else if (ajustesButtons[3].contains(p.x, p.y)) {
          ResetScreen();
        } else if (ajustesButtons[0].contains(p.x, p.y)) {
          CalibracionScreen();
        }
        break;
      }
      case Screens::Calibracion: {
        if (calibracionButtons[0].contains(p.x, p.y)) {
          AjustesScreen();
        } else if (calibracionButtons[1].contains(p.x, p.y)) {
          CalibTierraScreen();
        }
        break;
      }
      case Screens::CalibTierra: {
        if (calibTierraButtons[0].contains(p.x, p.y)) {
          CalibracionScreen();
        } else if (calibTierraButtons[1].contains(p.x, p.y)) {
          tMax = soil;
          EEPROM.put(26, tMax);
          tft.setTextSize(BUTTON_TEXTSIZE);
          tft.setTextColor(WHITE, BLACK);
          sprintf_P(buffer, PSTR("%d"), tMax);
          tft.setCursor(230 - (strlen(buffer) * CHARACTER_WIDTH * 3), 100);
          tft.print(buffer);
        } else if (calibTierraButtons[2].contains(p.x, p.y)) {
          tMin = soil;
          tft.setTextSize(BUTTON_TEXTSIZE);
          tft.setTextColor(WHITE, BLACK);
          EEPROM.put(28, tMin);
          sprintf_P(buffer, PSTR("%d"), tMin);
          tft.setCursor(230 - (strlen(buffer) * CHARACTER_WIDTH * 3), 190);
          tft.print(buffer);
        }
        break;
      }
      case Screens::Reloj: {
        tft.setTextSize(3);
        tft.setTextColor(WHITE, BLACK);
        if (relojButtons[0].contains(p.x, p.y)) {
          AjustesScreen();
        } else if (relojButtons[1].contains(p.x, p.y)) {
#ifdef DEBUG_ENABLED
          now = DateTime(reloj.YYYY, reloj.MM, reloj.DD, reloj.hh, reloj.mm, 0);
#else
          rtc.adjust(
              DateTime(reloj.YYYY, reloj.MM, reloj.DD, reloj.hh, reloj.mm, 0));
          now = rtc.now();
#endif  // DEBUG_ENABLED
          AjustesScreen();
        } else if ((p.x > 39 && p.x < 83) && (p.y > 24 && p.y < 60)) {
          reloj.YYYY++;
          sprintf_P(buffer, PSTR("%d"), reloj.YYYY);
          tft.setCursor(28, 60);
          tft.print(buffer);
          if (reloj.MM == 2) {
            if ((reloj.YYYY % 4 == 0 && reloj.YYYY % 100 != 0) ||
                reloj.YYYY % 400 == 0) {
              if (reloj.DD > 29) {
                reloj.DD = 29;
                sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
            } else {
              if (reloj.DD > 28) {
                reloj.DD = 28;
                sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
            }
          }
        } else if ((p.x > 39 && p.x < 83) && (p.y > 81 && p.y < 114)) {
          if (reloj.YYYY > 1970) {
            reloj.YYYY--;
            sprintf_P(buffer, PSTR("%d"), reloj.YYYY);
            tft.setCursor(28, 60);
            tft.print(buffer);
          }
        } else if ((p.x > 114 && p.x < 158) && (p.y > 24 && p.y < 60)) {
          if (reloj.MM < 12) {
            reloj.MM++;
            sprintf_P(buffer, PSTR("%02d"), reloj.MM);
            tft.setCursor(28 + 72 + 20, 60);
            tft.print(buffer);
          } else {
            reloj.MM = 1;
            sprintf_P(buffer, PSTR("%02d"), reloj.MM);
            tft.setCursor(28 + 72 + 20, 60);
            tft.print(buffer);
          }
          switch (reloj.MM) {
            case 4:
            case 6:
            case 9:
            case 11:
              if (reloj.DD > 30) {
                reloj.DD = 30;
                sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 2:
              if ((reloj.YYYY % 4 == 0 && reloj.YYYY % 100 != 0) ||
                  reloj.YYYY % 400 == 0) {
                if (reloj.DD > 29) {
                  reloj.DD = 29;
                  sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              } else {
                if (reloj.DD > 28) {
                  reloj.DD = 28;
                  sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              }
              break;
          }
        } else if ((p.x > 114 && p.x < 158) && (p.y > 81 && p.y < 114)) {
          if (reloj.MM > 1) {
            reloj.MM--;
            sprintf_P(buffer, PSTR("%02d"), reloj.MM);
            tft.setCursor(28 + 72 + 20, 60);
            tft.print(buffer);
          } else {
            reloj.MM = 12;
            sprintf_P(buffer, PSTR("%02d"), reloj.MM);
            tft.setCursor(28 + 72 + 20, 60);
            tft.print(buffer);
          }
          switch (reloj.MM) {
            case 4:
            case 6:
            case 9:
            case 11:
              if (reloj.DD > 30) {
                reloj.DD = 30;
                sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 2:
              if ((reloj.YYYY % 4 == 0 && reloj.YYYY % 100 != 0) ||
                  reloj.YYYY % 400 == 0) {
                if (reloj.DD > 29) {
                  reloj.DD = 29;
                  sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              } else {
                if (reloj.DD > 28) {
                  reloj.DD = 28;
                  sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              }
              break;
          }
        } else if ((p.x > 170 && p.x < 214) && (p.y > 24 && p.y < 60)) {
          switch (reloj.MM) {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
              if (reloj.DD < 31) {
                reloj.DD++;
                sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              } else {
                reloj.DD = 1;
                sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 4:
            case 6:
            case 9:
            case 11:
              if (reloj.DD < 30) {
                reloj.DD++;
                sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              } else {
                reloj.DD = 1;
                sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 2:
              if ((reloj.YYYY % 4 == 0 && reloj.YYYY % 100 != 0) ||
                  reloj.YYYY % 400 == 0) {
                if (reloj.DD < 29) {
                  reloj.DD++;
                  sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                } else {
                  reloj.DD = 1;
                  sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              } else {
                if (reloj.DD < 28) {
                  reloj.DD++;
                  sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                } else {
                  reloj.DD = 1;
                  sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              }
              break;
          }
        } else if ((p.x > 170 && p.x < 214) && (p.y > 81 && p.y < 114)) {
          switch (reloj.MM) {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
              if (reloj.DD > 1) {
                reloj.DD--;
                sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              } else {
                reloj.DD = 31;
                sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 4:
            case 6:
            case 9:
            case 11:
              if (reloj.DD > 1) {
                reloj.DD--;
                sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              } else {
                reloj.DD = 30;
                sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 2:
              if ((reloj.YYYY % 4 == 0 && reloj.YYYY % 100 != 0) ||
                  reloj.YYYY % 400 == 0) {
                if (reloj.DD > 1) {
                  reloj.DD--;
                  sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                } else {
                  reloj.DD = 29;
                  sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              } else {
                if (reloj.DD > 1) {
                  reloj.DD--;
                  sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                } else {
                  reloj.DD = 28;
                  sprintf_P(buffer, PSTR("%02d"), reloj.DD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              }
              break;
          }
        } else if ((p.x > 67 && p.x < 111) && (p.y > 116 && p.y < 150)) {
          if (reloj.hh < 23) {
            reloj.hh++;
            sprintf_P(buffer, PSTR("%02d"), reloj.hh);
            tft.setCursor(74, 150);
            tft.print(buffer);
          } else {
            reloj.hh = 0;
            sprintf_P(buffer, PSTR("%02d"), reloj.hh);
            tft.setCursor(74, 150);
            tft.print(buffer);
          }
        } else if ((p.x > 67 && p.x < 111) && (p.y > 171 && p.y < 207)) {
          if (reloj.hh > 0) {
            reloj.hh--;
            sprintf_P(buffer, PSTR("%02d"), reloj.hh);
            tft.setCursor(74, 150);
            tft.print(buffer);
          } else {
            reloj.hh = 23;
            sprintf_P(buffer, PSTR("%02d"), reloj.hh);
            tft.setCursor(74, 150);
            tft.print(buffer);
          }
        } else if ((p.x > 124 && p.x < 168) && (p.y > 116 && p.y < 150)) {
          if (reloj.mm < 59) {
            reloj.mm++;
            sprintf_P(buffer, PSTR("%02d"), reloj.mm);
            tft.setCursor(74 + 36 + 20, 150);
            tft.print(buffer);
          } else {
            reloj.mm = 0;
            sprintf_P(buffer, PSTR("%02d"), reloj.mm);
            tft.setCursor(74 + 36 + 20, 150);
            tft.print(buffer);
          }
        } else if ((p.x > 124 && p.x < 168) && (p.y > 171 && p.y < 207)) {
          if (reloj.mm > 0) {
            reloj.mm--;
            sprintf_P(buffer, PSTR("%02d"), reloj.mm);
            tft.setCursor(74 + 36 + 20, 150);
            tft.print(buffer);
          } else {
            reloj.mm = 59;
            sprintf_P(buffer, PSTR("%02d"), reloj.mm);
            tft.setCursor(74 + 36 + 20, 150);
            tft.print(buffer);
          }
        }
        delay(150);
        break;
      }
      case Screens::Programas: {
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
      }
      case Screens::Reset: {
        if (resetButtons[0].contains(p.x, p.y)) {
          AjustesScreen();
        } else if (resetButtons[1].contains(p.x, p.y)) {
          eeprom_hardReset(&tft);
        }
        break;
      }
      case Screens::Programa_1: {
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

              DEBUG_PRINTLN(F("Programa 1 cargado"));
              break;
            case 2:  // guardar
              EEPROM.put(110, pActivo.f1);
              EEPROM.put(130, pActivo.f2);
              EEPROM.put(150, pActivo.f3);
              EEPROM.put(170, pActivo.f4);

              DEBUG_PRINTLN(F("Programa 1 guardado"));
              break;
          }
          programasConfirmar = 0;
          tft.fillRect(5, 170, 230, 40, BLACK);
        }
        break;
      }
      case Screens::Programa_2: {
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

              DEBUG_PRINTLN(F("Programa 2 cargado"));
              break;
            case 2:  // guardar
              EEPROM.put(210, pActivo.f1);
              EEPROM.put(230, pActivo.f2);
              EEPROM.put(250, pActivo.f3);
              EEPROM.put(270, pActivo.f4);

              DEBUG_PRINTLN(F("Programa 2 guardado"));
              break;
          }
          programasConfirmar = 0;
          tft.fillRect(5, 170, 230, 40, BLACK);
        }
        break;
      }
      case Screens::Programa_3: {
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

              DEBUG_PRINTLN(F("Programa 3 cargado"));
              break;
            case 2:  // guardar
              EEPROM.put(310, pActivo.f1);
              EEPROM.put(330, pActivo.f2);
              EEPROM.put(350, pActivo.f3);
              EEPROM.put(370, pActivo.f4);

              DEBUG_PRINTLN(F("Programa 3 guardado"));
              break;
          }
          programasConfirmar = 0;
          tft.fillRect(5, 170, 230, 40, BLACK);
        }
        break;
      }
      case Screens::Programa_4: {
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

              DEBUG_PRINTLN(F("Programa 4 cargado"));
              break;
            case 2:  // guardar
              EEPROM.put(410, pActivo.f1);
              EEPROM.put(430, pActivo.f2);
              EEPROM.put(450, pActivo.f3);
              EEPROM.put(470, pActivo.f4);

              DEBUG_PRINTLN(F("Programa 4 guardado"));
              break;
          }
          programasConfirmar = 0;
          tft.fillRect(5, 170, 230, 40, BLACK);
        }
        break;
      }
      case Screens::Zona_1_Menu: {
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
      }
      case Screens::Zona_1_Control: {
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
          DEBUG_PRINTLN(F("EEPROM 10 to 29 cleared to 0x00\n"));

          z1fActiva = 0;

          z1TerminarConfirmar = 0;
          tft.fillRect(5, 170, 230, 40, BLACK);
        }
        break;
      }
      case Screens::Zona_1_Inicio: {
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
      }
      case Screens::Zona_1_F1: {
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
        } else if (z1f1Buttons[3].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f1.riegod, eeprom_dirFase(1) + 9, 2,
                                "Riego dias");
        }
        /* else if (z1f1Buttons[8].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f1.riegol, 35, 2, "Riego bajo");
        } else if (z1f1Buttons[7].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f1.riegoh, 36, 2, "Riego alto");
        } */
        /* else if (z1f1Buttons[11].contains(p.x, p.y)) {
           NumericKeyboardScreen(&pActivo.f1.huml, 37, 2, "Hum baja");
         } else if (z1f1Buttons[10].contains(p.x, p.y)) {
           NumericKeyboardScreen(&pActivo.f1.humh, 38, 2, "Hum alta");
         }*/

        break;
      }
      case Screens::Zona_1_F2: {
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
        } else if (z1f2Buttons[3].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f2.riegod, eeprom_dirFase(2) + 9, 2,
                                "Riego dias");
        }
        /* else if (z1f2Buttons[8].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f2.riegol, 55, 2, "Riego bajo");
        } else if (z1f2Buttons[7].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f2.riegoh, 56, 2, "Riego alto");
        } */
        /* else if (z1f2Buttons[11].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f2.huml, 57, 2, "Hum baja");
        } else if (z1f2Buttons[10].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f2.humh, 58, 2, "Hum alta");
        }*/
        break;
      }
      case Screens::Zona_1_F3: {
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
        } else if (z1f3Buttons[3].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f3.riegod, eeprom_dirFase(3) + 9, 2,
                                "Riego dias");
        }
        /* else if (z1f3Buttons[8].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f3.riegol, 75, 2, "Riego bajo");
        } else if (z1f3Buttons[7].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f3.riegoh, 76, 2, "Riego alto");
        } */
        /* else if (z1f3Buttons[11].contains(p.x, p.y)) {
           NumericKeyboardScreen(&pActivo.f3.huml, 77, 2, "Hum baja");
         } else if (z1f3Buttons[10].contains(p.x, p.y)) {
           NumericKeyboardScreen(&pActivo.f3.humh, 78, 2, "Hum alta");
         }*/
        break;
      }
      case Screens::Zona_1_F4: {
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
        } else if (z1f4Buttons[3].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f4.riegod, eeprom_dirFase(4) + 9, 2,
                                "Riego dias");
        }
        /* else if (z1f4Buttons[8].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f4.riegol, 95, 2, "Riego bajo");
        } else if (z1f4Buttons[7].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f4.riegoh, 96, 2, "Riego alto");
        } */
        /* else if (z1f4Buttons[11].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f4.huml, 97, 2, "Hum baja");
        } else if (z1f4Buttons[10].contains(p.x, p.y)) {
          NumericKeyboardScreen(&pActivo.f4.humh, 98, 2, "Hum alta");
        }*/

        break;
      }
      case Screens::numKB: {
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
            case Screens::Zona_1_F1: {
              Z1F1Screen();
              break;
            }
            case Screens::Zona_1_F2: {
              Z1F2Screen();
              break;
            }
            case Screens::Zona_1_F3: {
              Z1F3Screen();
              break;
            }
            case Screens::Zona_1_F4: {
              Z1F4Screen();
              break;
            }
            case Screens::Zona_1_Control: {
              Z1ControlScreen();
              break;
            }
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
            case Screens::Zona_1_F1: {
              Z1F1Screen();
              break;
            }
            case Screens::Zona_1_F2: {
              Z1F2Screen();
              break;
            }
            case Screens::Zona_1_F3: {
              Z1F3Screen();
              break;
            }
            case Screens::Zona_1_F4: {
              Z1F4Screen();
              break;
            }
            case Screens::Zona_1_Control: {
              Z1ControlScreen();
              break;
            }
          }
        }
        break;
      }
    }
  }
}

// MENUS

void HomeScreen() {
  currentScreen = Screens::Home;
  Estado mediciones;
  mediciones.t = t;
  mediciones.dias = dias;
  mediciones.hTierra = hTierra;
  mediciones.h = (uint8_t)h;
  mediciones.dias_prox_riego = dias_prox_riego;
  eeprom_cargarfActivaSP(&fActivaSP, z1fActiva);
  drawHomeScreen(&tft, homeButtons, z1fActiva, fActivaSP, mediciones, now);
}

void MenuScreen() {
  currentScreen = Screens::Menu;
  drawMenuScreen(&tft, menuButtons);
}

void AjustesScreen() {
  currentScreen = Screens::Ajustes;
  drawAjustesScreen(&tft, ajustesButtons);
}

void CalibracionScreen() {
  currentScreen = Screens::Calibracion;
  drawCalibracionScreen(&tft, calibracionButtons);
}

void CalibTierraScreen() {
  currentScreen = Screens::CalibTierra;
  drawCalibTierraScreen(&tft, calibTierraButtons, soil, tMin, tMax);
}

void RelojScreen() {
  currentScreen = Screens::Reloj;
  drawRelojScreen(&tft, relojButtons, now, &reloj);
}

void ProgramasScreen() {
  currentScreen = Screens::Programas;
  drawProgramasScreen(&tft, programasButtons);
}

void Programa1Screen() {
  currentScreen = Screens::Programa_1;
  drawPrograma1Screen(&tft, programa1Buttons);
}

void Programa2Screen() {
  currentScreen = Screens::Programa_2;
  drawPrograma2Screen(&tft, programa2Buttons);
}

void Programa3Screen() {
  currentScreen = Screens::Programa_3;
  drawPrograma3Screen(&tft, programa3Buttons);
}

void Programa4Screen() {
  currentScreen = Screens::Programa_4;
  drawPrograma4Screen(&tft, programa4Buttons);
}

void ResetScreen() {
  currentScreen = Screens::Reset;
  drawResetScreen(&tft, resetButtons);
}

void Z1Screen() {
  currentScreen = Screens::Zona_1_Menu;
  drawZ1Screen(&tft, z1Buttons);
}

void Z1ControlScreen() {
  currentScreen = Screens::Zona_1_Control;
  drawZ1ControlScreen(&tft, z1ControlButtons, &fActivaSP);
}

void Z1InicioScreen() {
  currentScreen = Screens::Zona_1_Inicio;
  z1fSeleccionada = z1fActiva;
  drawZ1InicioScreen(&tft, z1InicioButtons, z1fActiva);
}

void Z1F1Screen() {
  currentScreen = Screens::Zona_1_F1;
  drawZ1F1Screen(&tft, z1f1Buttons, &pActivo);
}

void Z1F2Screen() {
  currentScreen = Screens::Zona_1_F2;
  drawZ1F2Screen(&tft, z1f2Buttons, &pActivo);
}

void Z1F3Screen() {
  currentScreen = Screens::Zona_1_F3;
  drawZ1F3Screen(&tft, z1f3Buttons, &pActivo);
}

void Z1F4Screen() {
  currentScreen = Screens::Zona_1_F4;
  drawZ1F4Screen(&tft, z1f4Buttons, &pActivo);
}

// pantalla de teclado numerico para modificar valores
// quiza puedo hacer una sola funcion con void*
// intptr       ptr al int a modificar
// eepromdir    dirección eeprom en la que guardar el valor
// bufferSize   digitos maximos de la string a modificar
// title        titulo de la pantalla de teclado
void NumericKeyboardScreen(uint8_t* intptr, uint16_t eepromdir,
                           uint8_t bufferSize, const char* title) {
  currentScreen = Screens::numKB;
  sprintf_P(numKBstr, PSTR("%d"), *intptr);
  numKBvarptr16b = NULL;
  numKBvarptr8b = intptr;
  numKBeeprom = eepromdir;
  numKBbufferSize = bufferSize;
  drawNumericKeyboardScreen(&tft, numericKeyboardButtons, title, prevScreen,
                            &numKBPrevScreen, numKBstr);
}

void NumericKeyboardScreen(uint16_t* intptr, uint16_t eepromdir,
                           uint8_t bufferSize, const char* title) {
  currentScreen = Screens::numKB;
  sprintf_P(numKBstr, PSTR("%d"), *intptr);
  numKBvarptr16b = intptr;
  numKBvarptr8b = NULL;
  numKBeeprom = eepromdir;
  numKBbufferSize = bufferSize;
  drawNumericKeyboardScreen(&tft, numericKeyboardButtons, title, prevScreen,
                            &numKBPrevScreen, numKBstr);
}
