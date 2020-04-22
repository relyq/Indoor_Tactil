void setup() {
  Serial.begin(9600);

  Serial.print("TFT size is ");
  Serial.print(tft.width());
  Serial.print("x");
  Serial.println(tft.height());

  pinMode(13, OUTPUT);

  DDRC |= (HEATPIN | FANPIN | LUZPIN | VAPPIN | RIEGOPIN) & ~(DHTPIN);
  PORTC |= DHTPIN;

  pinMode(SENSORTIERRAPIN, INPUT);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

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

  dht.begin();

  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(0);
  tft.fillScreen(BLACK);

  Serial.println("Starting Loop");

  now = rtc.now();
  prevTime = now.unixtime() + 61;
  drawStartupScreen();
  delay(1500);
  HomeScreen();
}