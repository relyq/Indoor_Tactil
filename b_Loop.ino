void loop() {
  // readTH();
  // hTierra = map(analogRead(A8), 0, 1023, 100, 0);

  DEBUG();
  tsMenu();

  if (z1fActivalast != z1fActiva) {
    Serial.println("fase activa cambiada");
    switch (z1fActiva) {
      case 1:
        strcpy(templSPstr, z1f1templSPstr);
        strcpy(temphSPstr, z1f1temphSPstr);
        strcpy(humlSPstr, z1f1humlSPstr);
        strcpy(humhSPstr, z1f1humhSPstr);
        strcpy(diasSPstr, z1f1diasSPstr);
        strcpy(riegolSPstr, z1f1riegolSPstr);
        strcpy(riegohSPstr, z1f1riegohSPstr);
        break;
      case 2:
        strcpy(templSPstr, z1f2templSPstr);
        strcpy(temphSPstr, z1f2temphSPstr);
        strcpy(humlSPstr, z1f2humlSPstr);
        strcpy(humhSPstr, z1f2humhSPstr);
        strcpy(diasSPstr, z1f2diasSPstr);
        strcpy(riegolSPstr, z1f2riegolSPstr);
        strcpy(riegohSPstr, z1f2riegohSPstr);
        break;
      case 3:
        strcpy(templSPstr, z1f3templSPstr);
        strcpy(temphSPstr, z1f3temphSPstr);
        strcpy(humlSPstr, z1f3humlSPstr);
        strcpy(humhSPstr, z1f3humhSPstr);
        strcpy(diasSPstr, z1f3diasSPstr);
        strcpy(riegolSPstr, z1f3riegolSPstr);
        strcpy(riegohSPstr, z1f3riegohSPstr);
        break;
      case 4:
        strcpy(templSPstr, z1f4templSPstr);
        strcpy(temphSPstr, z1f4temphSPstr);
        strcpy(humlSPstr, z1f4humlSPstr);
        strcpy(humhSPstr, z1f4humhSPstr);
        strcpy(diasSPstr, z1f4diasSPstr);
        strcpy(riegolSPstr, z1f4riegolSPstr);
        strcpy(riegohSPstr, z1f4riegohSPstr);
        break;
    }

    templSP = strtol(templSPstr, 0, 10);
    temphSP = strtol(temphSPstr, 0, 10);
    humlSP = strtol(humlSPstr, 0, 10);
    humhSP = strtol(humhSPstr, 0, 10);
    diasSP = strtol(diasSPstr, 0, 10);
    riegolSP = strtol(riegolSPstr, 0, 10);
    riegohSP = strtol(riegohSPstr, 0, 10);

    z1fActivalast = z1fActiva;
  }

  if (t >= temphSP) {  // t>=temphSP && t>=(temphSP-templSP)/2
    digitalWrite(FANPIN, 1);
  } else {
    digitalWrite(FANPIN, 0);
  }

  if (h >= humhSP) {
    digitalWrite(VAPPIN, 1);
  } else {
    digitalWrite(VAPPIN, 0);
  }

  if (hTierra <= riegolSP) {
    digitalWrite(RIEGOPIN, 1);
  } else if (hTierra >= riegohSP) {
    digitalWrite(RIEGOPIN, 0);
  }

  if (currentScreen == "home" && (lastT != t || lastH != h ||
                                  lasthTierra != hTierra || lastdias != dias)) {
    lastT = t;
    lastH = h;
    lasthTierra = hTierra;
    lastdias = dias;

    sprintf(buffer, "%d", dias);
    tft.setCursor(125 - (strlen(buffer) * 18), 230);
    tft.setTextSize(3);
    tft.setTextColor(WHITE, BLACK);
    tft.print(buffer);

    sprintf(buffer, "%d", hTierra);
    strcat(buffer, "%");
    tft.setCursor(230 - (strlen(buffer) * 18), 230);
    tft.setTextSize(3);
    tft.setTextColor(WHITE, BLACK);
    tft.print(buffer);

    dtostrf(t, 4, 1, buffer);
    strcat(buffer, "C");
    tft.setCursor(125 - (strlen(buffer) * 18), 285);
    tft.setTextSize(3);
    tft.setTextColor(WHITE, BLACK);
    tft.print(buffer);  // temperatura leida por el DHT

    sprintf(buffer, "%d", (uint8_t)h);
    strcat(buffer, "%");
    tft.setCursor(230 - (strlen(buffer) * 18), 285);
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