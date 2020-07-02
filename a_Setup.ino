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
      fActivaSP.dias = 0;
      fActivaSP.templ = 0;
      fActivaSP.temph = 0;
      fActivaSP.huml = 0;
      fActivaSP.humh = 0;
      fActivaSP.riegol = 0;
      fActivaSP.riegoh = 0;
      break;
    case 1:
      fActivaSP = f1;
      break;
    case 2:
      fActivaSP = f2;
      break;
    case 3:
      fActivaSP = f3;
      break;
    case 4:
      fActivaSP = f4;
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
  EEPROM.get(30, f1);
  EEPROM.get(50, f2);
  EEPROM.get(70, f3);
  EEPROM.get(90, f4);
}