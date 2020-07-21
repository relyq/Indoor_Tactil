void drawHomeScreen() {
  tft.fillScreen(BLACK);

  // menu principal
  homeButtons[0].initButtonUL(&tft, 5, 10, 230, 40, YELLOW, DARKGREEN, YELLOW,
                              "Menu", BUTTON_TEXTSIZE);
  homeButtons[0].drawButton();

  tft.setCursor(10, 65);
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.print(F("Iluminacion"));

  if (PINC & LUZPIN) {
    tft.fillCircle(180, 69, 10, GREEN);
  } else {
    tft.fillCircle(180, 69, 10, LIGHTGREY);
  }

  tft.setCursor(10, 90);
  tft.print(F("Ventilacion"));

  if (PINC & FANPIN && !(PINC & HEATPIN)) {
    tft.fillCircle(180, 94, 10, BLUE);
  } else if (PINC & HEATPIN && !(PINC & FANPIN)) {
    tft.fillCircle(180, 94, 10, YELLOW);
  } else if (!(PINC & HEATPIN) && !(PINC & FANPIN)) {
    tft.fillCircle(180, 94, 10, LIGHTGREY);
  }

  tft.setCursor(10, 115);
  tft.print(F("Vaporizacion"));

  if (PINC & VAPPIN) {
    tft.fillCircle(180, 119, 10, GREEN);
  } else {
    tft.fillCircle(180, 119, 10, LIGHTGREY);
  }

  tft.setCursor(10, 140);
  tft.print(F("Riego"));

  if (PINC & RIEGOPIN) {
    tft.fillCircle(180, 144, 10, GREEN);
  } else {
    tft.fillCircle(180, 144, 10, LIGHTGREY);
  }

  tft.setCursor(10, 165);
  tft.print(F("Fase "));
  if (z1fActiva != 0) {
    tft.print(z1fActiva);
  } else {
    tft.print(F("-"));
  }

  strcpy_P(buffer, PSTR("hh:mm"));
  now.toString(buffer);
  tft.setCursor(170, 165);
  tft.print(buffer);

  strcpy_P(buffer, PSTR("DD/MM/YY"));
  now.toString(buffer);
  tft.setCursor(134, 183);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), fActivaSP.dias);
  tft.setCursor(10, 200);
  tft.print(F("Dias   "));

  tft.setCursor(125 - (strlen(buffer) * 12), 200);
  tft.setTextColor(ORANGE);
  tft.print(buffer);

  tft.setCursor(130, 200);
  tft.setTextColor(WHITE);
  tft.print(F("Riego "));

  sprintf_P(buffer, PSTR("%d"), fActivaSP.riegoh);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(130, 220);
  tft.setTextColor(ORANGE);
  tft.print(buffer);  // riego sp h

  sprintf_P(buffer, PSTR("%d"), fActivaSP.riegol);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(130, 240);
  tft.setTextColor(BLUE);
  tft.print(buffer);  // riego sp l

  tft.setCursor(10, 260);
  tft.setTextColor(WHITE);
  tft.print(F("T."));

  sprintf_P(buffer, PSTR("%d"), fActivaSP.templ);
  strcat_P(buffer, PSTR("C"));
  tft.setCursor(77 - (strlen(buffer) * 12), 260);
  tft.setTextColor(BLUE);
  tft.print(buffer);  // temp sp l

  sprintf_P(buffer, PSTR("%d"), fActivaSP.temph);
  strcat_P(buffer, PSTR("C"));
  tft.setCursor(125 - (strlen(buffer) * 12), 260);
  tft.setTextColor(ORANGE);
  tft.print(buffer);  // temp sp h

  tft.setCursor(130, 260);
  tft.setTextColor(WHITE);
  tft.print(F("Humedad "));

  sprintf_P(buffer, PSTR("%d"), fActivaSP.humh);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(130, 280);
  tft.setTextColor(ORANGE);
  tft.print(buffer);  // humedad sp h

  sprintf_P(buffer, PSTR("%d"), fActivaSP.huml);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(130, 300);
  tft.setTextColor(BLUE);
  tft.print(buffer);  // humedad sp l

  sprintf_P(buffer, PSTR("%d"), dias);
  tft.setCursor(125 - (strlen(buffer) * 18), 230);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), hTierra);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(230 - (strlen(buffer) * 18), 230);
  tft.print(buffer);

  dtostrf(t, 4, 1, buffer);
  strcat_P(buffer, PSTR("C"));
  tft.setCursor(125 - (strlen(buffer) * 18), 285);
  tft.print(buffer);  // temperatura leida por el DHT

  sprintf_P(buffer, PSTR("%d"), (uint8_t)h);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(230 - (strlen(buffer) * 18), 285);
  tft.print(buffer);  // humedad leida por el DHT

  Serial.println(F("home dibujada"));
}

void drawMenuScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 4) - 3) / 2), 10);
  tft.print(F("Menu"));

  // boton 1 - zona 1
  menuButtons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                              "Zona 1", BUTTON_TEXTSIZE);
  menuButtons[0].drawButton();

  // boton 2 - ajustes
  menuButtons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                              "Ajustes", BUTTON_TEXTSIZE);
  menuButtons[1].drawButton();

  menuButtons[2].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                              "Volver", BUTTON_TEXTSIZE);
  menuButtons[2].drawButton();

  Serial.println(F("menu dibujado"));
}

void drawAjustesScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(F("Ajustes"), 120, 18, &x1, &y1, &w, &h);
  tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("Ajustes"));

  // boton 1 - alarmas
  ajustesButtons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                                 "Alarmas", BUTTON_TEXTSIZE);
  ajustesButtons[0].drawButton();

  // boton 2 - reloj
  ajustesButtons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                                 "Reloj", BUTTON_TEXTSIZE);
  ajustesButtons[1].drawButton();

  // boton 3 - programas
  ajustesButtons[2].initButtonUL(&tft, 5, 125, 230, 40, WHITE, ORANGE, YELLOW,
                                 "Programas", BUTTON_TEXTSIZE);
  ajustesButtons[2].drawButton();

  // boton 4 - hard reset
  ajustesButtons[3].initButtonUL(&tft, 5, 170, 230, 40, WHITE, ORANGE, YELLOW,
                                 "H. Reset", BUTTON_TEXTSIZE);
  ajustesButtons[3].drawButton();

  // boton 5 - Volver
  ajustesButtons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                                 "Volver", BUTTON_TEXTSIZE);
  ajustesButtons[4].drawButton();

  Serial.println(F("ajustes dibujado"));
}

void drawAlarmasScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 7) - 3) / 2), 10);
  tft.print(F("Alarmas"));

  alarmasButtons[0].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                                 "Volver", BUTTON_TEXTSIZE);
  alarmasButtons[0].drawButton();

  Serial.println(F("alarmas dibujado"));
}

void drawRelojScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 5) - 3) / 2), 10);
  tft.print(F("Reloj"));

  now = rtc.now();

  relojYYYY = now.year();
  relojMM = now.month();
  relojDD = now.day();
  relojhh = now.hour();
  relojmm = now.minute();

  tft.setTextSize(3);

  tft.fillTriangle(61, 34, 49, 50, 73, 50, ORANGE);     // año++
  tft.fillTriangle(136, 34, 124, 50, 148, 50, ORANGE);  // mes++
  tft.fillTriangle(192, 34, 180, 50, 204, 50, ORANGE);  // dia++

  sprintf_P(buffer, PSTR("%d"), relojYYYY);
  tft.setCursor(28, 60);
  tft.print(buffer);

  sprintf_P(buffer, STR_f02decimal, relojMM);
  tft.setCursor(28 + 72 + 20, 60);
  tft.print(buffer);

  sprintf_P(buffer, STR_f02decimal, relojDD);
  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
  tft.print(buffer);

  tft.fillTriangle(61, 107, 49, 91, 73, 91, ORANGE);     // año--
  tft.fillTriangle(136, 107, 124, 91, 148, 91, ORANGE);  // mes--
  tft.fillTriangle(192, 107, 180, 91, 204, 91, ORANGE);  // dia--

  tft.fillTriangle(89, 124, 77, 140, 101, 140, ORANGE);    // hora++
  tft.fillTriangle(146, 124, 134, 140, 158, 140, ORANGE);  // minuto++

  sprintf_P(buffer, STR_f02decimal, relojhh);
  tft.setCursor(74, 150);
  tft.print(buffer);

  sprintf_P(buffer, STR_f02decimal, relojmm);
  tft.setCursor(74 + 36 + 20, 150);
  tft.print(buffer);

  tft.fillTriangle(89, 197, 77, 181, 101, 181, ORANGE);    // hora--
  tft.fillTriangle(146, 197, 134, 181, 158, 181, ORANGE);  // minuto--

  relojButtons[1].initButtonUL(&tft, 20, 210, 200, 40, WHITE, DARKGREEN, WHITE,
                               "Aceptar", BUTTON_TEXTSIZE);
  relojButtons[1].drawButton();

  relojButtons[0].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                               "Volver", BUTTON_TEXTSIZE);
  relojButtons[0].drawButton();

  Serial.println(F("reloj dibujado"));
}

void drawProgramasScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(F("Programas"), 110, 18, &x1, &y1, &w, &h);
  tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("Programas"));

  programasButtons[1].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Prog. 1", BUTTON_TEXTSIZE);
  programasButtons[1].drawButton();

  programasButtons[2].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Prog. 2", BUTTON_TEXTSIZE);
  programasButtons[2].drawButton();

  programasButtons[3].initButtonUL(&tft, 5, 125, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Prog. 3", BUTTON_TEXTSIZE);
  programasButtons[3].drawButton();

  programasButtons[4].initButtonUL(&tft, 5, 170, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Prog. 4", BUTTON_TEXTSIZE);
  programasButtons[4].drawButton();

  programasButtons[0].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY,
                                   WHITE, "Volver", BUTTON_TEXTSIZE);
  programasButtons[0].drawButton();

  Serial.println(F("programas dibujado"));
}

void drawPrograma1Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(F("Prog. 1"), 120, 18, &x1, &y1, &w, &h);
  tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("Prog. 1"));

  programa1Buttons[1].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Cargar", BUTTON_TEXTSIZE);
  programa1Buttons[1].drawButton();

  programa1Buttons[2].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Guardar", BUTTON_TEXTSIZE);
  programa1Buttons[2].drawButton();

  programa1Buttons[3].initButtonUL(&tft, 5, 125, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Reestablecer", BUTTON_TEXTSIZE);
  programa1Buttons[3].drawButton();

  programa1Buttons[4].initButtonUL(&tft, 20, 170, 200, 40, WHITE, GREEN, WHITE,
                                   "Confirmar", BUTTON_TEXTSIZE);

  programa1Buttons[0].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY,
                                   WHITE, "Volver", BUTTON_TEXTSIZE);
  programa1Buttons[0].drawButton();
}

void drawPrograma2Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(F("Prog. 2"), 120, 18, &x1, &y1, &w, &h);
  tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("Prog. 2"));

  programa2Buttons[1].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Cargar", BUTTON_TEXTSIZE);
  programa2Buttons[1].drawButton();

  programa2Buttons[2].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Guardar", BUTTON_TEXTSIZE);
  programa2Buttons[2].drawButton();

  programa2Buttons[3].initButtonUL(&tft, 5, 125, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Reestablecer", BUTTON_TEXTSIZE);
  programa2Buttons[3].drawButton();

  programa2Buttons[4].initButtonUL(&tft, 20, 170, 200, 40, WHITE, GREEN, WHITE,
                                   "Confirmar", BUTTON_TEXTSIZE);

  programa2Buttons[0].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY,
                                   WHITE, "Volver", BUTTON_TEXTSIZE);
  programa2Buttons[0].drawButton();
}

void drawPrograma3Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(F("Prog. 3"), 120, 18, &x1, &y1, &w, &h);
  tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("Prog. 3"));

  programa3Buttons[1].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Cargar", BUTTON_TEXTSIZE);
  programa3Buttons[1].drawButton();

  programa3Buttons[2].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Guardar", BUTTON_TEXTSIZE);
  programa3Buttons[2].drawButton();

  programa3Buttons[3].initButtonUL(&tft, 5, 125, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Reestablecer", BUTTON_TEXTSIZE);
  programa3Buttons[3].drawButton();

  programa3Buttons[4].initButtonUL(&tft, 20, 170, 200, 40, WHITE, GREEN, WHITE,
                                   "Confirmar", BUTTON_TEXTSIZE);

  programa3Buttons[0].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY,
                                   WHITE, "Volver", BUTTON_TEXTSIZE);
  programa3Buttons[0].drawButton();
}

void drawPrograma4Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(F("Prog. 4"), 120, 18, &x1, &y1, &w, &h);
  tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("Prog. 4"));

  programa4Buttons[1].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Cargar", BUTTON_TEXTSIZE);
  programa4Buttons[1].drawButton();

  programa4Buttons[2].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Guardar", BUTTON_TEXTSIZE);
  programa4Buttons[2].drawButton();

  programa4Buttons[3].initButtonUL(&tft, 5, 125, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Reestablecer", BUTTON_TEXTSIZE);
  programa4Buttons[3].drawButton();

  programa4Buttons[4].initButtonUL(&tft, 20, 170, 200, 40, WHITE, GREEN, WHITE,
                                   "Confirmar", BUTTON_TEXTSIZE);

  programa4Buttons[0].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY,
                                   WHITE, "Volver", BUTTON_TEXTSIZE);
  programa4Buttons[0].drawButton();
}

void drawResetScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 5) - 3) / 2), 10);
  tft.print(F("Reset"));

  tft.setCursor(5, 35);
  tft.print(
      F("\tEsta seguro que\n desea reiniciar el\n dispositivo a la\n "
        "configuracion de\n fabrica?"));

  resetButtons[0].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                               "Volver", BUTTON_TEXTSIZE);
  resetButtons[0].drawButton();

  resetButtons[1].initButtonUL(&tft, 20, 170, 200, 40, WHITE, RED, WHITE,
                               "Confirmar", BUTTON_TEXTSIZE);
  resetButtons[1].drawButton();

  Serial.println(F("reset dibujado"));
}

void drawZ1Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 6) - 3) / 2), 10);
  tft.print(F("Zona 1"));

  // boton 1 - fase 1
  z1Buttons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                            "Fase 1", BUTTON_TEXTSIZE);
  z1Buttons[0].drawButton();

  // boton 2 - fase 2
  z1Buttons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                            "Fase 2", BUTTON_TEXTSIZE);
  z1Buttons[1].drawButton();

  // boton 3 - fase 3
  z1Buttons[2].initButtonUL(&tft, 5, 125, 230, 40, WHITE, ORANGE, YELLOW,
                            "Fase 3", BUTTON_TEXTSIZE);
  z1Buttons[2].drawButton();

  // boton 4 - fase 4
  z1Buttons[3].initButtonUL(&tft, 5, 170, 230, 40, WHITE, ORANGE, YELLOW,
                            "Fase 4", BUTTON_TEXTSIZE);
  z1Buttons[3].drawButton();

  // boton 5 - control
  z1Buttons[4].initButtonUL(&tft, 5, 215, 230, 40, WHITE, ORANGE, YELLOW,
                            "Control", BUTTON_TEXTSIZE);
  z1Buttons[4].drawButton();

  // boton 6 - volver
  z1Buttons[5].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                            "Volver", BUTTON_TEXTSIZE);
  z1Buttons[5].drawButton();

  Serial.println(F("z1 dibujado"));
}

void drawZ1F1Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 6) - 3) / 2), 10);
  tft.print(F("Fase 1"));

  z1f1Buttons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f1Buttons[0].drawRectButton();
  tft.setCursor(15, 40);
  tft.print(F("Dias"));

  z1f1Buttons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f1Buttons[1].drawRectButton();
  tft.setCursor(15, 85);
  tft.print(F("Horas luz"));

  z1f1Buttons[6].initButtonUL(&tft, 100, 125, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f1Buttons[6].drawRectButton();
  z1f1Buttons[5].initButtonUL(&tft, 175, 125, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f1Buttons[5].drawRectButton();
  z1f1Buttons[2].initButtonUL(&tft, 5, 125, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f1Buttons[2].drawRectButton();
  tft.setCursor(15, 130);
  tft.print(F("Temp."));

  z1f1Buttons[8].initButtonUL(&tft, 100, 170, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f1Buttons[8].drawRectButton();
  z1f1Buttons[7].initButtonUL(&tft, 175, 170, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f1Buttons[7].drawRectButton();
  z1f1Buttons[3].initButtonUL(&tft, 5, 170, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f1Buttons[3].drawRectButton();
  tft.setCursor(15, 175);
  tft.print(F("Riego"));

  z1f1Buttons[11].initButtonUL(&tft, 100, 215, 60, 40, BLACK, BLACK, BLACK, "",
                               2);
  z1f1Buttons[11].drawRectButton();
  z1f1Buttons[10].initButtonUL(&tft, 175, 215, 60, 40, BLACK, BLACK, BLACK, "",
                               2);
  z1f1Buttons[10].drawRectButton();
  z1f1Buttons[9].initButtonUL(&tft, 5, 215, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f1Buttons[9].drawRectButton();
  tft.setCursor(15, 220);
  tft.print(F("Humedad"));

  tft.setTextSize(3);

  //// dias
  sprintf_P(buffer, PSTR("%d"), f1.dias);

  tft.setCursor(228 - (strlen(buffer) * 18), 45);
  tft.print(buffer);

  //// hluz
  if (f1.hLuz > 24) f1.hLuz = 24;
  sprintf_P(buffer, PSTR("%d"), f1.hLuz);
  strcat_P(buffer, STR_hum);
  tft.setCursor(228 - (strlen(buffer) * 18), 90);
  tft.print(buffer);

  //// temp
  if (f1.temph > 80) f1.temph = 80;
  sprintf_P(buffer, PSTR("%d"), f1.temph);
  uint8_t z1f1temphSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("C"));
  tft.setCursor(228 - (strlen(buffer) * 18), 135);
  tft.print(buffer);

  if (f1.templ > 80) f1.templ = 80;
  sprintf_P(buffer, PSTR("%d"), f1.templ);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f1temphSTRlen * 18), 135);
  tft.print(buffer);

  tft.setCursor(192 - (z1f1temphSTRlen * 18), 135);
  tft.print(F("-"));

  //// riego
  sprintf_P(buffer, PSTR("%d"), f1.riegoh);
  uint8_t z1f1riegohSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 180);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), f1.riegol);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f1riegohSTRlen * 18), 180);
  tft.print(buffer);

  tft.setCursor(192 - (z1f1riegohSTRlen * 18), 180);
  tft.print(F("-"));

  //// humedad
  sprintf_P(buffer, PSTR("%d"), f1.humh);
  uint8_t z1f1humhSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 225);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), f1.huml);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f1humhSTRlen * 18), 225);
  tft.print(buffer);

  tft.setCursor(192 - (z1f1humhSTRlen * 18), 225);
  tft.print(F("-"));

  z1f1Buttons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                              "Volver", BUTTON_TEXTSIZE);
  z1f1Buttons[4].drawButton();

  Serial.println(F("z1f1 dibujado"));
}

void drawZ1F2Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 6) - 3) / 2), 10);
  tft.print(F("Fase 2"));

  z1f2Buttons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f2Buttons[0].drawRectButton();
  tft.setCursor(15, 40);
  tft.print(F("Dias"));

  z1f2Buttons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f2Buttons[1].drawRectButton();
  tft.setCursor(15, 85);
  tft.print(F("Horas luz"));

  z1f2Buttons[6].initButtonUL(&tft, 100, 125, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f2Buttons[6].drawRectButton();
  z1f2Buttons[5].initButtonUL(&tft, 175, 125, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f2Buttons[5].drawRectButton();
  z1f2Buttons[2].initButtonUL(&tft, 5, 125, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f2Buttons[2].drawRectButton();
  tft.setCursor(15, 130);
  tft.print(F("Temp."));

  z1f2Buttons[8].initButtonUL(&tft, 100, 170, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f2Buttons[8].drawRectButton();
  z1f2Buttons[7].initButtonUL(&tft, 175, 170, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f2Buttons[7].drawRectButton();
  z1f2Buttons[3].initButtonUL(&tft, 5, 170, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f2Buttons[3].drawRectButton();
  tft.setCursor(15, 175);
  tft.print(F("Riego"));

  z1f2Buttons[11].initButtonUL(&tft, 100, 215, 60, 40, BLACK, BLACK, BLACK, "",
                               2);
  z1f2Buttons[11].drawRectButton();
  z1f2Buttons[10].initButtonUL(&tft, 175, 215, 60, 40, BLACK, BLACK, BLACK, "",
                               2);
  z1f2Buttons[10].drawRectButton();
  z1f2Buttons[9].initButtonUL(&tft, 5, 215, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f2Buttons[9].drawRectButton();
  tft.setCursor(15, 220);
  tft.print(F("Humedad"));

  tft.setTextSize(3);

  //// dias
  sprintf_P(buffer, PSTR("%d"), f2.dias);

  tft.setCursor(228 - (strlen(buffer) * 18), 45);
  tft.print(buffer);

  //// hluz
  if (f2.hLuz > 24) f2.hLuz = 24;
  sprintf_P(buffer, PSTR("%d"), f2.hLuz);
  strcat_P(buffer, STR_hum);
  tft.setCursor(228 - (strlen(buffer) * 18), 90);
  tft.print(buffer);

  //// temp
  if (f2.temph > 80) f2.temph = 80;
  sprintf_P(buffer, PSTR("%d"), f2.temph);
  uint8_t z1f2temphSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("C"));
  tft.setCursor(228 - (strlen(buffer) * 18), 135);
  tft.print(buffer);

  if (f2.templ > 80) f2.templ = 80;
  sprintf_P(buffer, PSTR("%d"), f2.templ);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f2temphSTRlen * 18), 135);
  tft.print(buffer);

  tft.setCursor(192 - (z1f2temphSTRlen * 18), 135);
  tft.print(F("-"));

  //// riego
  sprintf_P(buffer, PSTR("%d"), f2.riegoh);
  uint8_t z1f2riegohSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 180);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), f2.riegol);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f2riegohSTRlen * 18), 180);
  tft.print(buffer);

  tft.setCursor(192 - (z1f2riegohSTRlen * 18), 180);
  tft.print(F("-"));

  //// humedad
  sprintf_P(buffer, PSTR("%d"), f2.humh);
  uint8_t z1f2humhSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 225);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), f2.huml);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f2humhSTRlen * 18), 225);
  tft.print(buffer);

  tft.setCursor(192 - (z1f2humhSTRlen * 18), 225);
  tft.print(F("-"));

  z1f2Buttons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                              "Volver", BUTTON_TEXTSIZE);
  z1f2Buttons[4].drawButton();

  Serial.println(F("z1f2 dibujado"));
}

void drawZ1F3Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 6) - 3) / 2), 10);
  tft.print(F("Fase 3"));

  z1f3Buttons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f3Buttons[0].drawRectButton();
  tft.setCursor(15, 40);
  tft.print(F("Dias"));

  z1f3Buttons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f3Buttons[1].drawRectButton();
  tft.setCursor(15, 85);
  tft.print("Horas luz");

  z1f3Buttons[6].initButtonUL(&tft, 100, 125, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f3Buttons[6].drawRectButton();
  z1f3Buttons[5].initButtonUL(&tft, 175, 125, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f3Buttons[5].drawRectButton();
  z1f3Buttons[2].initButtonUL(&tft, 5, 125, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f3Buttons[2].drawRectButton();
  tft.setCursor(15, 130);
  tft.print(F("Temp."));

  z1f3Buttons[8].initButtonUL(&tft, 100, 170, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f3Buttons[8].drawRectButton();
  z1f3Buttons[7].initButtonUL(&tft, 175, 170, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f3Buttons[7].drawRectButton();
  z1f3Buttons[3].initButtonUL(&tft, 5, 170, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f3Buttons[3].drawRectButton();
  tft.setCursor(15, 175);
  tft.print(F("Riego"));

  z1f3Buttons[11].initButtonUL(&tft, 100, 215, 60, 40, BLACK, BLACK, BLACK, "",
                               2);
  z1f3Buttons[11].drawRectButton();
  z1f3Buttons[10].initButtonUL(&tft, 175, 215, 60, 40, BLACK, BLACK, BLACK, "",
                               2);
  z1f3Buttons[10].drawRectButton();
  z1f3Buttons[9].initButtonUL(&tft, 5, 215, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f3Buttons[9].drawRectButton();
  tft.setCursor(15, 220);
  tft.print(F("Humedad"));

  tft.setTextSize(3);

  //// dias
  sprintf_P(buffer, PSTR("%d"), f3.dias);

  tft.setCursor(228 - (strlen(buffer) * 18), 45);
  tft.print(buffer);

  //// hluz
  if (f3.hLuz > 24) f3.hLuz = 24;
  sprintf_P(buffer, PSTR("%d"), f3.hLuz);
  strcat_P(buffer, STR_hum);
  tft.setCursor(228 - (strlen(buffer) * 18), 90);
  tft.print(buffer);

  //// temp
  if (f3.temph > 80) f3.temph = 80;
  sprintf_P(buffer, PSTR("%d"), f3.temph);
  uint8_t z1f3temphSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("C"));
  tft.setCursor(228 - (strlen(buffer) * 18), 135);
  tft.print(buffer);

  if (f3.templ > 80) f3.templ = 80;
  sprintf_P(buffer, PSTR("%d"), f3.templ);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f3temphSTRlen * 18), 135);
  tft.print(buffer);

  tft.setCursor(192 - (z1f3temphSTRlen * 18), 135);
  tft.print(F("-"));

  //// riego
  sprintf_P(buffer, PSTR("%d"), f3.riegoh);
  uint8_t z1f3riegohSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 180);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), f3.riegol);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f3riegohSTRlen * 18), 180);
  tft.print(buffer);

  tft.setCursor(192 - (z1f3riegohSTRlen * 18), 180);
  tft.print(F("-"));

  //// humedad
  sprintf_P(buffer, PSTR("%d"), f3.humh);
  uint8_t z1f3humhSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 225);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), f3.huml);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f3humhSTRlen * 18), 225);
  tft.print(buffer);

  tft.setCursor(192 - (z1f3humhSTRlen * 18), 225);
  tft.print(F("-"));

  z1f3Buttons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                              "Volver", BUTTON_TEXTSIZE);
  z1f3Buttons[4].drawButton();

  Serial.println(F("z1f3 dibujado"));
}

void drawZ1F4Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 6) - 3) / 2), 10);
  tft.print(F("Fase 4"));

  z1f4Buttons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f4Buttons[0].drawRectButton();
  tft.setCursor(15, 40);
  tft.print(F("Dias"));

  z1f4Buttons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f4Buttons[1].drawRectButton();
  tft.setCursor(15, 85);
  tft.print(F("Horas luz"));

  z1f4Buttons[6].initButtonUL(&tft, 100, 125, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f4Buttons[6].drawRectButton();
  z1f4Buttons[5].initButtonUL(&tft, 175, 125, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f4Buttons[5].drawRectButton();
  z1f4Buttons[2].initButtonUL(&tft, 5, 125, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f4Buttons[2].drawRectButton();
  tft.setCursor(15, 130);
  tft.print(F("Temp."));

  z1f4Buttons[8].initButtonUL(&tft, 100, 170, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f4Buttons[8].drawRectButton();
  z1f4Buttons[7].initButtonUL(&tft, 175, 170, 60, 40, BLACK, BLACK, BLACK, "",
                              2);
  z1f4Buttons[7].drawRectButton();
  z1f4Buttons[3].initButtonUL(&tft, 5, 170, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f4Buttons[3].drawRectButton();
  tft.setCursor(15, 175);
  tft.print(F("Riego"));

  z1f4Buttons[11].initButtonUL(&tft, 100, 215, 60, 40, BLACK, BLACK, BLACK, "",
                               2);
  z1f4Buttons[11].drawRectButton();
  z1f4Buttons[10].initButtonUL(&tft, 175, 215, 60, 40, BLACK, BLACK, BLACK, "",
                               2);
  z1f4Buttons[10].drawRectButton();
  z1f4Buttons[9].initButtonUL(&tft, 5, 215, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f4Buttons[9].drawRectButton();
  tft.setCursor(15, 220);
  tft.print(F("Humedad"));

  tft.setTextSize(3);

  //// dias
  sprintf_P(buffer, PSTR("%d"), f4.dias);

  tft.setCursor(228 - (strlen(buffer) * 18), 45);
  tft.print(buffer);

  //// hluz
  if (f4.hLuz > 24) f4.hLuz = 24;
  sprintf_P(buffer, PSTR("%d"), f4.hLuz);
  strcat_P(buffer, STR_hum);
  tft.setCursor(228 - (strlen(buffer) * 18), 90);
  tft.print(buffer);

  //// temp
  if (f4.temph > 80) f4.temph = 80;
  sprintf_P(buffer, PSTR("%d"), f4.temph);
  uint8_t z1f4temphSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("C"));
  tft.setCursor(228 - (strlen(buffer) * 18), 135);
  tft.print(buffer);

  if (f4.templ > 80) f4.templ = 80;
  sprintf_P(buffer, PSTR("%d"), f4.templ);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f4temphSTRlen * 18), 135);
  tft.print(buffer);

  tft.setCursor(192 - (z1f4temphSTRlen * 18), 135);
  tft.print(F("-"));

  //// riego
  sprintf_P(buffer, PSTR("%d"), f4.riegoh);
  uint8_t z1f4riegohSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 180);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), f4.riegol);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f4riegohSTRlen * 18), 180);
  tft.print(buffer);

  tft.setCursor(192 - (z1f4riegohSTRlen * 18), 180);
  tft.print(F("-"));

  //// humedad
  sprintf_P(buffer, PSTR("%d"), f4.humh);
  uint8_t z1f4humhSTRlen = strlen(buffer);
  strcat_P(buffer, PSTR("%"));
  tft.setCursor(228 - (strlen(buffer) * 18), 225);
  tft.print(buffer);

  sprintf_P(buffer, PSTR("%d"), f4.huml);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f4humhSTRlen * 18), 225);
  tft.print(buffer);

  tft.setCursor(192 - (z1f4humhSTRlen * 18), 225);
  tft.print(F("-"));

  z1f4Buttons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                              "Volver", BUTTON_TEXTSIZE);
  z1f4Buttons[4].drawButton();

  Serial.println(F("z1f4 dibujado"));
}

void drawZ1ControlScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 7) - 3) / 2), 10);
  tft.print(F("Control"));

  z1ControlButtons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Inicio", BUTTON_TEXTSIZE);
  z1ControlButtons[0].drawButton();

  z1ControlButtons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Terminar", BUTTON_TEXTSIZE);
  z1ControlButtons[1].drawButton();

  z1ControlButtons[3].initButtonUL(&tft, 20, 170, 200, 40, WHITE, RED, WHITE,
                                   "Confirmar", BUTTON_TEXTSIZE);

  tft.drawRect(5, 125, 230, 40, WHITE);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.setCursor(15, 135);
  tft.print(F("Ciclos"));

  sprintf_P(buffer, PSTR("%d"), ciclos);
  tft.setCursor(228 - (strlen(buffer) * 18), 135);
  tft.print(buffer);

  z1ControlButtons[2].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY,
                                   WHITE, "Volver", BUTTON_TEXTSIZE);
  z1ControlButtons[2].drawButton();

  Serial.println(F("z1control dibujado"));
}

void drawZ1InicioScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * TITLE_TEXTSIZE * 6) - 3) / 2), 10);
  tft.print(F("Inicio"));

  tft.setTextSize(BUTTON_TEXTSIZE);
  tft.setCursor(120 - (((CHARACTER_WIDTH * BUTTON_TEXTSIZE * 5) - 3) / 2),
                40 - ((7 * BUTTON_TEXTSIZE) / 2));
  tft.print(F("Fases"));

  z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, DARKGREY, WHITE,
                                  "1", BUTTON_TEXTSIZE);

  z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE, DARKGREY, WHITE,
                                  "2", BUTTON_TEXTSIZE);

  z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE, DARKGREY, WHITE,
                                  "3", BUTTON_TEXTSIZE);

  z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE, DARKGREY,
                                  WHITE, "4", BUTTON_TEXTSIZE);

  switch (z1fActiva) {
    case 1:
      z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, OLIVE, WHITE,
                                      "1", BUTTON_TEXTSIZE);
      break;
    case 2:
      z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE, OLIVE,
                                      WHITE, "2", BUTTON_TEXTSIZE);
      break;
    case 3:
      z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE, OLIVE,
                                      WHITE, "3", BUTTON_TEXTSIZE);
      break;
    case 4:
      z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE, OLIVE,
                                      WHITE, "4", BUTTON_TEXTSIZE);
      break;
  }

  z1InicioButtons[0].drawRectButton();
  z1InicioButtons[1].drawRectButton();
  z1InicioButtons[2].drawRectButton();
  z1InicioButtons[3].drawRectButton();

  z1InicioButtons[4].initButtonUL(&tft, 5, 215, 230, 40, WHITE, DARKGREEN,
                                  WHITE, "Confirmar", BUTTON_TEXTSIZE);
  z1InicioButtons[4].drawButton();

  z1InicioButtons[5].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY,
                                  WHITE, "Volver", BUTTON_TEXTSIZE);
  z1InicioButtons[5].drawButton();

  Serial.println(F("z1inicio dibujado"));
}

void drawNumericKeyboardScreen(const char* title) {
  numKBPrevScreen = prevScreen;
  Serial.println(numKBPrevScreen);

  tft.fillScreen(BLACK);

  tft.setTextSize(BUTTON_TEXTSIZE);

  numericKeyboardButtons[0].initButtonUL(&tft, 5, 5, 60, 30, WHITE, ORANGE,
                                         WHITE, "<", BUTTON_TEXTSIZE);
  numericKeyboardButtons[0].drawButton();

  tft.setTextColor(WHITE);
  tft.setTextSize(TITLE_TEXTSIZE);
  tft.setCursor(152 - (CHARACTER_WIDTH * TITLE_TEXTSIZE * strlen(title)) / 2, 20 - (7 * 2) / 2);
  tft.print(title);

  tft.drawRect(5, 40, 230, 40, WHITE);

  tft.setTextSize(BUTTON_TEXTSIZE);
  tft.setCursor(15, 60 - ((7 * BUTTON_TEXTSIZE) / 2));
  tft.print(numKBstr);

  numericKeyboardButtons[1].initButtonUL(&tft, 30, 82, 40, 40, WHITE, BLACK,
                                         WHITE, "0", BUTTON_TEXTSIZE);
  numericKeyboardButtons[1].drawButton();
  // numericKeyboardButtons[2].initButtonUL(&tft, 100, 82, 40, 40, WHITE, BLACK,
  //                                       WHITE, "-", BUTTON_TEXTSIZE);
  // numericKeyboardButtons[2].drawButton();
  numericKeyboardButtons[3].initButtonUL(&tft, 155, 82, 80, 40, WHITE, BLACK,
                                         WHITE, "Del", BUTTON_TEXTSIZE);
  numericKeyboardButtons[3].drawButton();

  numericKeyboardButtons[4].initButtonUL(&tft, 30, 125, 40, 40, WHITE, BLACK,
                                         WHITE, "1", BUTTON_TEXTSIZE);
  numericKeyboardButtons[4].drawButton();
  numericKeyboardButtons[5].initButtonUL(&tft, 100, 125, 40, 40, WHITE, BLACK,
                                         WHITE, "2", BUTTON_TEXTSIZE);
  numericKeyboardButtons[5].drawButton();
  numericKeyboardButtons[6].initButtonUL(&tft, 175, 125, 40, 40, WHITE, BLACK,
                                         WHITE, "3", BUTTON_TEXTSIZE);
  numericKeyboardButtons[6].drawButton();

  numericKeyboardButtons[7].initButtonUL(&tft, 30, 177, 40, 40, WHITE, BLACK,
                                         WHITE, "4", BUTTON_TEXTSIZE);
  numericKeyboardButtons[7].drawButton();
  numericKeyboardButtons[8].initButtonUL(&tft, 100, 177, 40, 40, WHITE, BLACK,
                                         WHITE, "5", BUTTON_TEXTSIZE);
  numericKeyboardButtons[8].drawButton();
  numericKeyboardButtons[9].initButtonUL(&tft, 175, 177, 40, 40, WHITE, BLACK,
                                         WHITE, "6", BUTTON_TEXTSIZE);
  numericKeyboardButtons[9].drawButton();

  numericKeyboardButtons[10].initButtonUL(&tft, 30, 230, 40, 40, WHITE, BLACK,
                                          WHITE, "7", BUTTON_TEXTSIZE);
  numericKeyboardButtons[10].drawButton();
  numericKeyboardButtons[11].initButtonUL(&tft, 100, 230, 40, 40, WHITE, BLACK,
                                          WHITE, "8", BUTTON_TEXTSIZE);
  numericKeyboardButtons[11].drawButton();
  numericKeyboardButtons[12].initButtonUL(&tft, 175, 230, 40, 40, WHITE, BLACK,
                                          WHITE, "9", BUTTON_TEXTSIZE);
  numericKeyboardButtons[12].drawButton();

  numericKeyboardButtons[13].initButtonUL(&tft, 5, 280, 230, 40, WHITE,
                                          DARKCYAN, WHITE, "Aceptar",
                                          BUTTON_TEXTSIZE);
  numericKeyboardButtons[13].drawButton();
}

void drawStartupScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);

  // tft.setCursor(120 - (6 * 3 * 6) / 2, 160 - (7 * 3));
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(F("Silics"), 120, 139, &x1, &y1, &w, &h);
  tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("Silics"));

  tft.setCursor(120 - (6 * 3 * 10) / 2, 170);
  // tft.getTextBounds(F("GrowOS 0.9"), 120, 170, &x1, &y1, &w, &h);
  // tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("GrowOS 0.9"));
}

void drawGoodbyeScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);

  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(F("^_^"), 120, 160, &x1, &y1, &w, &h);
  tft.setCursor(x1 - w / 2, y1 - h / 2);
  tft.print(F("^_^"));
}