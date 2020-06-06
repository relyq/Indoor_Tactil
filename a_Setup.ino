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

  MCUSR = 0;  // clear out any flags of prior resets.

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // rtc.adjust(DateTime(1974, 1, 2, 0, 0, 0));
  }

  cargarEstado();  // carga el ultimo estado del dispositivo desde la EEPROM
  cargarFases();   // carga la configuracion de las fases desde la EEPROM

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

void cargarEstado() {
  z1fActiva = EEPROM.read(10);
  z1fActivalast = z1fActiva;
  z1fSeleccionada = z1fActiva;
  EEPROM.get(11, diaIniciodefase);
  EEPROM.get(15, diaFindefase);
  hInicioLuz = EEPROM.read(19);
  hFinLuz = EEPROM.read(20);
  mInicioFinLuz = EEPROM.read(21);
  ciclos = EEPROM.read(22);
}

void cargarFases() {  // podria hacer esto mucho mas facil con eeprom.get si
                      // usara structs para las fases
  EEPROM.get(30, z1f1dias);
  z1f1hLuz = EEPROM.read(32);
  z1f1templ = EEPROM.read(33);
  z1f1temph = EEPROM.read(34);
  z1f1riegol = EEPROM.read(35);
  z1f1riegoh = EEPROM.read(36);
  z1f1huml = EEPROM.read(37);
  z1f1humh = EEPROM.read(38);

  EEPROM.get(50, z1f2dias);
  z1f2hLuz = EEPROM.read(52);
  z1f2templ = EEPROM.read(53);
  z1f2temph = EEPROM.read(54);
  z1f2riegol = EEPROM.read(55);
  z1f2riegoh = EEPROM.read(56);
  z1f2huml = EEPROM.read(57);
  z1f2humh = EEPROM.read(58);

  EEPROM.get(70, z1f3dias);
  z1f3hLuz = EEPROM.read(72);
  z1f3templ = EEPROM.read(73);
  z1f3temph = EEPROM.read(74);
  z1f3riegol = EEPROM.read(75);
  z1f3riegoh = EEPROM.read(76);
  z1f3huml = EEPROM.read(77);
  z1f3humh = EEPROM.read(78);

  EEPROM.get(90, z1f4dias);
  z1f4hLuz = EEPROM.read(92);
  z1f4templ = EEPROM.read(93);
  z1f4temph = EEPROM.read(94);
  z1f4riegol = EEPROM.read(95);
  z1f4riegoh = EEPROM.read(96);
  z1f4huml = EEPROM.read(97);
  z1f4humh = EEPROM.read(98);
}