void loop() {
  now = rtc.now();
  // readTH();
  // hTierra = map(analogRead(A8), 0, 1023, 100, 0);

  DEBUG();
  tsMenu();

  if (z1fActivalast != z1fActiva) {
    Serial.println("fase activa cambiada");
    switch (z1fActiva) {
      case 0:
        diasSP = 0;
        templSP = 0;
        temphSP = 0;
        humlSP = 0;
        humhSP = 0;
        riegolSP = 0;
        riegohSP = 0;
        break;
      case 1:
        diasSP = z1f1dias;
        templSP = z1f1templ;
        temphSP = z1f1temph;
        humlSP = z1f1huml;
        humhSP = z1f1humh;
        riegolSP = z1f1riegol;
        riegohSP = z1f1riegoh;
        break;
      case 2:
        diasSP = z1f2dias;
        templSP = z1f2templ;
        temphSP = z1f2temph;
        humlSP = z1f2huml;
        humhSP = z1f2humh;
        riegolSP = z1f2riegol;
        riegohSP = z1f2riegoh;
        break;
      case 3:
        diasSP = z1f3dias;
        templSP = z1f3templ;
        temphSP = z1f3temph;
        humlSP = z1f3huml;
        humhSP = z1f3humh;
        riegolSP = z1f3riegol;
        riegohSP = z1f3riegoh;
        break;
      case 4:
        diasSP = z1f4dias;
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

    Serial.print("diaFindefase: ");
    Serial.println(diaFindefase);
    Serial.print("unixtime: ");
    Serial.println(now.unixtime());

    z1fActivalast = z1fActiva;
  }

  if (z1fActiva != 0) {
    if (t >= temphSP) {  // t>=temphSP && t>=(temphSP-templSP)/2
      PORTC |= FANPIN;
    } else {
      PORTC &= ~FANPIN;
    }

    if (h >= humhSP) {
      PORTC |= VAPPIN;
    } else {
      PORTC &= ~VAPPIN;
    }

    if (hTierra <= riegolSP) {
      PORTC |= RIEGOPIN;
    } else if (hTierra >= riegohSP) {
      PORTC &= ~RIEGOPIN;
    }

    if (now.unixtime() >= diaFindefase) {
      if (z1fActiva == 4) {
        z1fActiva = 0;
      } else {
        z1fActiva++;
      }
    }

    dias = (now.unixtime() - diaIniciodefase) / 86400;
  }

  if (currentScreen == "home") {
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

      strcpy(buffer, "DD/MM");
      now.toString(buffer);
      tft.setCursor(170, 183);
      tft.print(buffer);
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
        tft.fillCircle(180, 144, 10, RED);
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

      if (PINC & FANPIN) {
        tft.fillCircle(180, 94, 10, GREEN);
      } else {
        tft.fillCircle(180, 94, 10, RED);
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
        tft.fillCircle(180, 119, 10, RED);
      }
    }
  }
  if ((currentScreen != "home" && currentScreen != "numKB") &&
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
    uint8_t i = 0;
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