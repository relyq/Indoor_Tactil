// EEPROM

#include "eepromThings.h"

#include <EEPROM.h>
#include <avr/wdt.h>

void drawGoodbyeScreen(Adafruit_TFTLCD* tft);

void eeprom_cargarfActivaSP(fActiva* fActivaSP, uint8_t f) {
  uint16_t fDir = eeprom_dirFase(f);

  EEPROM.get(fDir, fActivaSP->dias);
  fActivaSP->hLuz = EEPROM.read(fDir + 2);
  fActivaSP->templ = EEPROM.read(fDir + 3);
  fActivaSP->temph = EEPROM.read(fDir + 4);
  fActivaSP->riegol = EEPROM.read(fDir + 5);
  fActivaSP->riegoh = EEPROM.read(fDir + 6);
  fActivaSP->huml = EEPROM.read(fDir + 7);
  fActivaSP->humh = EEPROM.read(fDir + 8);
  fActivaSP->ciclos = EEPROM.read(22);
}

void eeprom_cargarPrograma(Programa* p) {
  EEPROM.get(30, p->f1);
  EEPROM.get(50, p->f2);
  EEPROM.get(70, p->f3);
  EEPROM.get(90, p->f4);
}

void eeprom_hardReset(Adafruit_TFTLCD* tft) {
  Fase F1DefaultSettings;
  Fase F2DefaultSettings;
  Fase F3DefaultSettings;
  Fase F4DefaultSettings;

  Programa p1;
  Programa p2;
  Programa p3;
  Programa p4;

  F1DefaultSettings.dias = 1;
  F1DefaultSettings.hLuz = 1;
  F1DefaultSettings.templ = 20;
  F1DefaultSettings.temph = 30;
  F1DefaultSettings.riegol = 20;
  F1DefaultSettings.riegoh = 60;
  F1DefaultSettings.huml = 60;
  F1DefaultSettings.humh = 80;

  F2DefaultSettings = F1DefaultSettings;
  F3DefaultSettings = F1DefaultSettings;
  F4DefaultSettings = F1DefaultSettings;

  F2DefaultSettings.dias = 2;
  F2DefaultSettings.hLuz = 2;
  F3DefaultSettings.dias = 3;
  F3DefaultSettings.hLuz = 3;
  F4DefaultSettings.dias = 4;
  F4DefaultSettings.hLuz = 4;

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

  p2 = p1;
  p3 = p1;
  p4 = p1;

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

  EEPROM.update(0, 88);
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

  for (uint16_t i = 4000; i < 4020; i++) {
    EEPROM.update(i, 0x00);
  }
  Serial.println(F("F0 restored"));

  drawGoodbyeScreen(tft);

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

void eeprom_read(uint8_t length) {
  Serial.print(F("Reading EEPROM, "));

  switch (length) {
    case 0:
    Serial.println(F("full: "));
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
      break;
      case 1:
      Serial.println(F("short: "));
      for (uint16_t i = 0; i < 30; i++) {
        switch (i) {
          case 0:
            Serial.print(F("\n\ndevice info"));
            break;
          case 10:
            Serial.print(F("\n\nphase info"));
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
  }
  Serial.print(F("\n"));
}

uint16_t eeprom_dirFase(uint8_t fase) {
  switch (fase) {
    case 0:
      Serial.print(F("no hay fase activa\n"));
      return 4000;
      break;
    case 1:
      return 30;
      break;
    case 2:
      return 50;
      break;
    case 3:
      return 70;
      break;
    case 4:
      return 90;
      break;
    default:
      Serial.print("fase inexistente\n");
      return 4000;
      break;
  }
}