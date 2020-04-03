void setup() {
  Serial.begin(9600);

  Serial.print("TFT size is ");
  Serial.print(tft.width());
  Serial.print("x");
  Serial.println(tft.height());

  pinMode(13, OUTPUT);
  pinMode(DHTPIN, INPUT_PULLUP);
  pinMode(FANPIN, OUTPUT);
  pinMode(VAPPIN, OUTPUT);
  pinMode(RIEGOPIN, OUTPUT);
  digitalWrite(RIEGOPIN, 0);
  pinMode(SENSORTIERRAPIN, INPUT_PULLUP);

  switch (z1fActiva) {
    case 1:
      strcpy(tempSPstr, z1f1tempSPstr);
      strcpy(humSPstr, z1f1humSPstr);
      break;
    case 2:
      strcpy(tempSPstr, z1f2tempSPstr);
      strcpy(humSPstr, z1f2humSPstr);
      break;
    case 3:
      strcpy(tempSPstr, z1f3tempSPstr);
      strcpy(humSPstr, z1f3humSPstr);
      break;
    case 4:
      strcpy(tempSPstr, z1f4tempSPstr);
      strcpy(humSPstr, z1f4humSPstr);
      break;
  }

  tempSP = strtol(tempSPstr, 0, 10);
  humSP = strtol(humSPstr, 0, 10);

  dht.begin();

  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(0);
  tft.fillScreen(BLACK);

  HomeScreen();
  Serial.println("Starting Loop");
}