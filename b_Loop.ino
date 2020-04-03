void loop() {
  // readTH();
  // hTierra = map(analogRead(A8), 0, 1023, 100, 0);

  DEBUG();
  tsMenu();

  if (z1fActivalast != z1fActiva) {
    Serial.println("fase activa cambiada");
    switch (z1fActiva) {
      case 1:
        strcpy(tempSPstr, z1f1tempSPstr);
        strcpy(humSPstr, z1f1humSPstr);
        strcpy(diasSPstr, z1f1diasSPstr);
        strcpy(riegoSPstr, z1f1riegoSPstr);
        break;
      case 2:
        strcpy(tempSPstr, z1f2tempSPstr);
        strcpy(humSPstr, z1f2humSPstr);
        strcpy(diasSPstr, z1f2diasSPstr);
        strcpy(riegoSPstr, z1f2riegoSPstr);
        break;
      case 3:
        strcpy(tempSPstr, z1f3tempSPstr);
        strcpy(humSPstr, z1f3humSPstr);
        strcpy(diasSPstr, z1f3diasSPstr);
        strcpy(riegoSPstr, z1f3riegoSPstr);
        break;
      case 4:
        strcpy(tempSPstr, z1f4tempSPstr);
        strcpy(humSPstr, z1f4humSPstr);
        strcpy(diasSPstr, z1f4diasSPstr);
        strcpy(riegoSPstr, z1f4riegoSPstr);
        break;
    }

    tempSP = strtol(tempSPstr, 0, 10);
    humSP = strtol(humSPstr, 0, 10);
    diasSP = strtol(diasSPstr, 0, 10);
    riegoSP = strtol(riegoSPstr, 0, 10);

    z1fActivalast = z1fActiva;
  }

  if (t >= tempSP) {
    digitalWrite(FANPIN, 1);
  } else {
    digitalWrite(FANPIN, 0);
  }

  if (h >= humSP) {
    digitalWrite(VAPPIN, 1);
  } else {
    digitalWrite(VAPPIN, 0);
  }

  if (hTierra <= hTierraSPl) {
    digitalWrite(RIEGOPIN, 1);
  } else if (hTierra >= hTierraSPh) {
    digitalWrite(RIEGOPIN, 0);
  }

  if (currentScreen == "home" &&
      (lastT != t || lastH != h ||
       lasthTierra !=
           hTierra || lastdias != dias)) {
    lastT = t;
    lastH = h;
    lasthTierra = hTierra;
    lastdias = dias;

    sprintf(buffer, "%d", dias);

    tft.setCursor(65, 230);
    tft.setTextSize(3);
    tft.setTextColor(WHITE, BLACK);
    tft.print(buffer);

    sprintf(buffer, "%d", hTierra);
    strcat(buffer, "%");

    tft.setCursor(185, 230);
    tft.setTextSize(3);
    tft.setTextColor(WHITE, BLACK);
    tft.print(buffer);

    dtostrf(t, 4, 1, buffer);
    strcat(buffer, "C");

    tft.setCursor(30, 285);
    tft.setTextSize(3);
    tft.setTextColor(WHITE, BLACK);
    tft.print(buffer);  // temperatura leida por el DHT

    sprintf(buffer, "%d", (uint8_t)h);
    strcat(buffer, "%");

    tft.setCursor(185, 285);
    tft.setTextSize(3);
    tft.setTextColor(WHITE, BLACK);
    tft.print(buffer);  // humedad leida por el DHT

    if (digitalRead(FANPIN)) {
      tft.fillCircle(180, 94, 10, GREEN);
    } else {
      tft.fillCircle(180, 94, 10, RED);
    }
    if (digitalRead(VAPPIN)) {
      tft.fillCircle(180, 119, 10, GREEN);
    } else {
      tft.fillCircle(180, 119, 10, RED);
    }
    if (digitalRead(RIEGOPIN)) {
      tft.fillCircle(180, 144, 10, GREEN);
    } else {
      tft.fillCircle(180, 144, 10, RED);
    }
  }
}

void readTH() {
  time = millis();
  if (time - lastTime >= 2000) {
    lastT = t;
    lastH = h;
    t = dht.readTemperature();
    h = dht.readHumidity();

    // sensación térmica en C
    // hi = dht.computeHeatIndex(t, h, false);

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