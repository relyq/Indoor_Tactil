void loop() {
  // readTH();
  // hTierra = map(analogRead(A8), 0, 1023, 100, 0);

  DEBUG();
  tsMenu();

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

  lastRIEGOPIN = digitalRead(RIEGOPIN);
  if (hTierra <= hTierraSPl) {
    digitalWrite(RIEGOPIN, 1);
  } else if (hTierra >= hTierraSPh) {
    digitalWrite(RIEGOPIN, 0);
  }

  if (currentScreen == "home" &&
      (lastT != t || lastH != h ||
       lastRIEGOPIN !=
           digitalRead(RIEGOPIN))) {  // TENGO QUE CAMBIAR ESTO PORQUE ME VA A
                                      // TRAER PROBLEMAS
    lastT = t;
    lastH = h;

    // dtostrf(double, strlength, precision, charbuff)
    // convierte double a char[]

    dtostrf(t, 5, 1, buffer);
    strcat(buffer, " C");

    tft.setCursor(10, 200);
    tft.setTextSize(5);
    tft.setTextColor(WHITE, BLACK);
    tft.print(buffer);  // temperatura leida por el DHT

    dtostrf(h, 5, 0, buffer);
    strcat(buffer, " %");

    tft.setCursor(10, 275);
    tft.setTextSize(5);
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
    }

    Serial.print(msg[0]);
    Serial.println(val);

    strcpy(lastmsg, msg);
  }
}
