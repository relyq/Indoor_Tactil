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

    if (h <= fActivaSP.humh) {
      PORTCSTATE |= VAPPIN;
    } else if (h >= ((float)fActivaSP.humh + (float)fActivaSP.huml) / 2) {
      PORTCSTATE &= ~VAPPIN;
    }

    // sistema de riego de mi viejo

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
      tft.setTextColor(WHITE, BLACK);
      tft.print(buffer);
      LASTRIEGOSTATE = (PINC & RIEGOPIN);

      Serial.print(F("PIN37 = "));
      Serial.println(PINC & RIEGOPIN);

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

    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    // Serial.print(F("  Heat index: "));
    // Serial.print(hi);
    // Serial.println(F("°C "));
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
