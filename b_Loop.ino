void loop() {
  readTH();
  tsMenu();

  if (currentScreen == "home" && (lastT != t || lastH != h)) {
    lastT = t;
    lastH = h;
    dtostrf(t, 5, 1, temperatura);  // convierte double a char[]
    dtostrf(h, 5, 0, humedad);      // dtostrf(double, strlength, precision, charbuf)
    tft.setCursor(10, 200);
    tft.setTextSize(5);
    tft.setTextColor(WHITE, BLACK);
    tft.print(strcat(temperatura, " C"));  // temperatura leida por el DHT

    tft.setCursor(10, 275);
    tft.setTextSize(5);
    tft.setTextColor(WHITE, BLACK);
    tft.print(strcat(humedad, " %"));  // humedad leida por el DHT
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
    Serial.print(F("  Heat index: "));
    Serial.print(hi);
    Serial.println(F("°C "));
    lastTime = millis();
  }
}