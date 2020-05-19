void loop() {
  now = rtc.now();
  // readTH();
  // hTierra = map(analogRead(A8), 0, 1023, 100, 0);

  DEBUG();
  tsMenu();

  // aca manejo el cambio de fases
  if (z1fActivalast != z1fActiva) {
    Serial.println("fase activa cambiada");
    switch (z1fActiva) {
      case 0:
        diasSP = 0;
        hLuz = 0;
        templSP = 0;
        temphSP = 0;
        humlSP = 0;
        humhSP = 0;
        riegolSP = 0;
        riegohSP = 0;
        break;
      case 1:
        diasSP = z1f1dias;
        hLuz = z1f1hLuz;
        templSP = z1f1templ;
        temphSP = z1f1temph;
        humlSP = z1f1huml;
        humhSP = z1f1humh;
        riegolSP = z1f1riegol;
        riegohSP = z1f1riegoh;
        break;
      case 2:
        diasSP = z1f2dias;
        hLuz = z1f2hLuz;
        templSP = z1f2templ;
        temphSP = z1f2temph;
        humlSP = z1f2huml;
        humhSP = z1f2humh;
        riegolSP = z1f2riegol;
        riegohSP = z1f2riegoh;
        break;
      case 3:
        diasSP = z1f3dias;
        hLuz = z1f3hLuz;
        templSP = z1f3templ;
        temphSP = z1f3temph;
        humlSP = z1f3huml;
        humhSP = z1f3humh;
        riegolSP = z1f3riegol;
        riegohSP = z1f3riegoh;
        break;
      case 4:
        diasSP = z1f4dias;
        hLuz = z1f4hLuz;
        templSP = z1f4templ;
        temphSP = z1f4temph;
        humlSP = z1f4huml;
        humhSP = z1f4humh;
        riegolSP = z1f4riegol;
        riegohSP = z1f4riegoh;
        break;
    }

    diaIniciodefase = now.unixtime();
    diaFindefase = now.unixtime() + diasSP * 86400;

    hInicioLuz = now.hour();
    mInicioFinLuz = now.minute();
    hFinLuz = (now.unixtime() + (hLuz * 60 * 60)) / 3600 % 24;

    Serial.print("diaFindefase: ");
    Serial.println(diaFindefase);
    Serial.print("unixtime: ");
    Serial.println(now.unixtime());

    Serial.print("horas luz: ");
    Serial.println(hLuz);
    Serial.print("hora fin de luz: ");
    Serial.print(hFinLuz);
    Serial.print(":");
    Serial.println(mInicioFinLuz);

    z1fActivalast = z1fActiva;

    if (currentScreen == 0) {
      HomeScreen();
    }
  }

  // funcionalidad
  if (z1fActiva != 0) {
    PORTCSTATE = PINC;

    if (t >= temphSP) {
      PORTCSTATE &= ~HEATPIN;
      PORTCSTATE |= FANPIN;
    } else if (t <= templSP) {
      PORTCSTATE &= ~FANPIN;
      PORTCSTATE |= HEATPIN;
    } else if (t <= ((float)temphSP + (float)templSP) / 2) {
      PORTCSTATE &= ~FANPIN;
    } else if (t >= ((float)temphSP + (float)templSP) / 2) {
      PORTCSTATE &= ~HEATPIN;
    }

    if (h <= humhSP) {
      PORTCSTATE |= VAPPIN;
    } else if (h >= ((float)humhSP + (float)humlSP) / 2) {
      PORTCSTATE &= ~VAPPIN;
    }


                      // 0 - riego activo
                      // 1 - riego espera
                      // 2 - riego activo primera vez
                      // 3 - riego espera primera vez
    /*
    
    if (hTierra <= riegolSP) {
      eRiego = 1;
      riegoEspera = 2;
    } else if (hTierra >= riegohSP) {
      eRiego = 0;
    }

    
    if (eRiego) {
      if (riegoEspera == 0 || riegoEspera == 2) {
        if (riegoEspera == 2) {
          riegoFin = now.unixtime() + riegoTiempo; // solo tengo que cambiar el tiempo de fin la primera vez que entro aca
        }
        PORTCSTATE |= RIEGOPIN;
        riegoEspera = 0;
        if (now.unixtime() >= riegoFin) {
          riegoEspera = 3;
        }
      }
      if (riegoEspera == 1 || riegoEspera == 3) {
        if (riegoEspera == 3) {
          riegoFin = now.unixtime() + riegoTiempo * 2;
        }
        PORTCSTATE &= ~RIEGOPIN;
        riegoEspera = 1;
        if (now.unixtime() >= riegoFin) {
          riegoEspera = 2;
        }
      }
    } else if (!eRiego) {
      PORTCSTATE &= ~RIEGOPIN;
    }
    */

   if (hTierra <= riegolSP) {
      tRiegoBomba = now.unixtime() + riegoTiempo;
      PORTCSTATE |= RIEGOPIN;
      Serial.println("tRiegoBomba sobreescrito");
    } else if (hTierra >= riegohSP) {
      tRiegoEspera = 0;
      tRiegoBomba = 0;
      PORTCSTATE &= ~RIEGOPIN;
    }

    Serial.print("tRiegoEspera = "); Serial.println(tRiegoEspera);
    Serial.print("tRiegoBomba = "); Serial.println(tRiegoBomba);

    if (tRiegoBomba && !tRiegoEspera) {
      
      if(now.unixtime() >= tRiegoBomba){
        tRiegoBomba = 0;
        tRiegoEspera = now.unixtime() + riegoTiempo * 2;
        PORTC &= ~RIEGOPIN;
      }
    }
    
    if (tRiegoEspera && !tRiegoBomba) {
      
      if(now.unixtime() >= tRiegoEspera){
        tRiegoEspera = 0;
        tRiegoBomba = now.unixtime() + riegoTiempo;
        PORTC |= RIEGOPIN;
      }
    }
    


    if (PINC != PORTCSTATE) {
      PORTC = PORTCSTATE;
    }

    // si la hora esta entre la hora de inicio de luz y la hora de fin de luz, y
    // la luz no esta prendida
    if (now.hour() > hInicioLuz ||
        (now.hour() == hInicioLuz && now.minute() >= mInicioFinLuz)) {
      if (!(PINC & LUZPIN)) {
        Serial.println("luz encendida");
        PORTC |= LUZPIN;
      }
    } else if (now.hour() < hFinLuz ||
               (now.hour() == hFinLuz && now.minute() < mInicioFinLuz)) {
      if (!(PINC & LUZPIN)) {
        Serial.println("luz encendida");
        PORTC |= LUZPIN;
      }
    } else {
      if (PINC & LUZPIN) {
        Serial.println("luz apagada");
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
  if (currentScreen == 0) {
    if (now.second() == 0 && now.unixtime() - prevTime >= 2) {
      prevTime = now.unixtime();
      Serial.print(now.year(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.day(), DEC);
      Serial.print(" (");
      Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
      Serial.print(") ");
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
      Serial.println();

      strcpy(buffer, "hh:mm");
      now.toString(buffer);
      tft.setTextSize(2);
      tft.setTextColor(WHITE, BLACK);
      tft.setCursor(170, 165);
      tft.print(buffer);

      strcpy(buffer, "DD/MM/YY");
      now.toString(buffer);
      tft.setCursor(134, 183);
      tft.print(buffer);
    }

    if (lastLuz != (PINC & LUZPIN)) {
      lastLuz = (PINC & LUZPIN);
      Serial.print("lastLuz: ");
      Serial.println(lastLuz);
      if (PINC & LUZPIN) {
        tft.fillCircle(180, 69, 10, GREEN);
      } else {
        tft.fillCircle(180, 69, 10, LIGHTGREY);
      }
    }

    if (lastdias != dias) {
      lastdias = dias;
      sprintf(buffer, "%d", dias);
      tft.setCursor(125 - (strlen(buffer) * 18), 230);
      tft.setTextSize(3);
      tft.setTextColor(WHITE, BLACK);
      tft.print(buffer);
    }

    if (lasthTierra != hTierra) {
      lasthTierra = hTierra;
      sprintf(buffer, "%d", hTierra);
      strcat(buffer, "%");
      tft.setCursor(230 - (strlen(buffer) * 18), 230);
      tft.setTextSize(3);
      tft.setTextColor(WHITE, BLACK);
      tft.print(buffer);

      if (PINC & RIEGOPIN) {
        tft.fillCircle(180, 144, 10, GREEN);
      } else {
        tft.fillCircle(180, 144, 10, LIGHTGREY);
      }
    }

    if (lastT != t) {
      lastT = t;
      dtostrf(t, 4, 1, buffer);
      strcat(buffer, "C");
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
      sprintf(buffer, "%d", (uint8_t)h);
      strcat(buffer, "%");
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
    strcpy(buffer, "hh:mm");
    now.toString(buffer);
    tft.setTextSize(2);
    tft.setTextColor(WHITE, BLACK);
    tft.setCursor(170, 10);
    tft.print(buffer);
  }
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
    uint8_t val;

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
      case 'D':  // dias
        val = (msgval[0] - '0') * 10;
        val += msgval[1] - '0';
        dias = val;
    }

    Serial.print(msg[0]);
    Serial.println(val);

    strcpy(lastmsg, msg);
  }
}
