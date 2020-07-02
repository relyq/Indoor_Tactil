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

void eeprom_hardReset() {
  struct defFase {
    uint16_t defFaseDias = 1;
    uint8_t defFasehLuz = 1;
    uint8_t defFasetempl = 20;
    uint8_t defFasetemph = 30;
    uint8_t defFaseriegol = 20;
    uint8_t defFaseriegoh = 60;
    uint8_t defFasehuml = 60;
    uint8_t defFasehumh = 80;
  };

  defFase F1DefaultSettings;
  defFase F2DefaultSettings;
  F2DefaultSettings.defFaseDias = 2;
  F2DefaultSettings.defFasehLuz = 2;
  defFase F3DefaultSettings;
  F3DefaultSettings.defFaseDias = 3;
  F3DefaultSettings.defFasehLuz = 3;
  defFase F4DefaultSettings;
  F4DefaultSettings.defFaseDias = 4;
  F4DefaultSettings.defFasehLuz = 4;

  EEPROM.update(0, 0x00);
  Serial.println("device information restored");
  for (uint8_t i = 10; i < 30; i++) {
    EEPROM.update(i, 0x00);
  }
  EEPROM.update(22, 1);
  Serial.println("phase information restored");
  EEPROM.put(30, F1DefaultSettings);
  Serial.println("F1 settings restored");
  EEPROM.put(50, F2DefaultSettings);
  Serial.println("F2 settings restored");
  EEPROM.put(70, F3DefaultSettings);
  Serial.println("F3 settings restored");
  EEPROM.put(90, F4DefaultSettings);
  Serial.println("F4 settings restored");

  wdt_enable(WDTO_15MS);
  while (1)
    ;
}

void eeprom_clear() {
  for (uint16_t i = 0; i < EEPROM.length(); i++) {
    EEPROM.update(i, 0xFF);
    if (!(i % 512)) {
      Serial.print("cleared ");
      Serial.print(i);
      Serial.println(" EEPROM bytes");
    }
  }
  Serial.println("eeprom cleared to 0xFF");
}

void eeprom_read() {
  Serial.println("Reading EEPROM: ");
  for (uint8_t i = 0; i < 120; i++) {
    switch (i) {
      case 0:
        Serial.print("\n\ndevice info");
        break;
      case 10:
        Serial.print("\n\nphase info");
        break;
      case 30:
        Serial.print("\n\nF1 settings");
        break;
      case 50:
        Serial.print("\n\nF2 settings");
        break;
      case 70:
        Serial.print("\n\nF3 settings");
        break;
      case 90:
        Serial.print("\n\nF4 settings");
        break;
      case 110:
        Serial.print("\n\n");
        break;
    }
    if (!(i % 5)) {
      Serial.print("\n");
      Serial.print(i);
      Serial.print("\t\t");
    }
    Serial.print(EEPROM.read(i));
    Serial.print("\t");
  }
  Serial.print("\n");
}