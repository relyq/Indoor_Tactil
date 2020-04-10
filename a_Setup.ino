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
  pinMode(SENSORTIERRAPIN, INPUT_PULLUP);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1); }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // If the RTC have lost power it will sets the RTC to the date & time this
    // sketch was compiled in the following line
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

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
  humhSP = strtol(humhSPstr, 0, 10);
  diasSP = strtol(diasSPstr, 0, 10);
  riegolSP = strtol(riegolSPstr, 0, 10);
  riegohSP = strtol(riegohSPstr, 0, 10);

  dht.begin();

  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(0);
  tft.fillScreen(BLACK);

  HomeScreen();
  Serial.println("Starting Loop");

  now = rtc.now();
  prevTime = now.unixtime();
}