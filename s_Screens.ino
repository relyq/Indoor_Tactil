void drawHomeScreen() {
  tft.fillScreen(BLACK);

  // menu principal
  homeButtons[0].initButtonUL(&tft, 5, 10, 230, 40, YELLOW, DARKGREEN, YELLOW,
                              "Menu", BUTTON_TEXTSIZE);
  homeButtons[0].drawButton();

  tft.setCursor(10, 65);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.print("Iluminacion");

  tft.fillCircle(180, 69, 10, RED);

  tft.setCursor(10, 90);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.print("Ventilacion");

  if (PINC & FANPIN) {
    tft.fillCircle(180, 94, 10, GREEN);
  } else {
    tft.fillCircle(180, 94, 10, RED);
  }

  tft.setCursor(10, 115);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.print("Vaporizacion");

  if (PINC & VAPPIN) {
    tft.fillCircle(180, 119, 10, GREEN);
  } else {
    tft.fillCircle(180, 119, 10, RED);
  }

  tft.setCursor(10, 140);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.print("Riego");

  if (PINC & RIEGOPIN) {
    tft.fillCircle(180, 144, 10, GREEN);
  } else {
    tft.fillCircle(180, 144, 10, RED);
  }

  tft.setCursor(10, 165);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.print("Fase ");
  if (z1fActiva != 0) {
    tft.print(z1fActiva);
  } else {
    tft.print("-");
  }

  strcpy(buffer, "hh:mm");
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 165);
  tft.print(buffer);

  strcpy(buffer, "DD/MM");
  now.toString(buffer);
  tft.setCursor(170, 183);
  tft.print(buffer);

  sprintf(buffer, "%d", diasSP);
  tft.setCursor(10, 200);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Dias   ");
  tft.setCursor(125 - (strlen(buffer) * 12), 200);
  tft.setTextColor(ORANGE);
  tft.print(buffer);

  tft.setCursor(130, 200);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Riego ");
  sprintf(buffer, "%d", riegohSP);
  strcat(buffer, "%");
  tft.setCursor(130, 220);
  tft.setTextColor(ORANGE);
  tft.print(buffer);  // riego sp h
  sprintf(buffer, "%d", riegolSP);
  strcat(buffer, "%");
  tft.setCursor(130, 240);
  tft.setTextColor(BLUE);
  tft.print(buffer);  // riego sp l

  tft.setCursor(10, 260);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("T.");
  sprintf(buffer, "%d", templSP);
  strcat(buffer, "C");
  tft.setCursor(77 - (strlen(buffer) * 12), 260);
  tft.setTextColor(BLUE);
  tft.print(buffer);  // temp sp l
  sprintf(buffer, "%d", temphSP);
  strcat(buffer, "C");
  tft.setCursor(125 - (strlen(buffer) * 12), 260);
  tft.setTextColor(ORANGE);
  tft.print(buffer);  // temp sp h

  tft.setCursor(130, 260);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Humedad ");
  sprintf(buffer, "%d", humhSP);
  strcat(buffer, "%");
  tft.setCursor(130, 280);
  tft.setTextColor(ORANGE);
  tft.print(buffer);  // humedad sp h
  sprintf(buffer, "%d", humlSP);
  strcat(buffer, "%");
  tft.setCursor(130, 300);
  tft.setTextColor(BLUE);
  tft.print(buffer);  // humedad sp l

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

  // tft.drawFastHLine(0, 190, 240, WHITE);

  // tft.drawFastHLine(20, 255, 200, WHITE);

  // tft.drawFastVLine(120, 210, 90, WHITE);

  Serial.println("home dibujada");
}

void drawMenuScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(120 - (((6 * 2 * 4) - 3) / 2), 10);
  tft.print("Menu");

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

  Serial.println("menu dibujado");
}

void drawAjustesScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(120 - (((6 * 2 * 7) - 3) / 2), 10);
  tft.print("Ajustes");

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
                                 "Reset", BUTTON_TEXTSIZE);
  ajustesButtons[3].drawButton();

  // boton 5 - Volver
  ajustesButtons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                                 "Volver", BUTTON_TEXTSIZE);
  ajustesButtons[4].drawButton();

  Serial.println("ajustes dibujado");
}

void drawAlarmasScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(120 - (((6 * 2 * 7) - 3) / 2), 10);
  tft.print("Alarmas");

  alarmasButtons[0].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                                 "Volver", BUTTON_TEXTSIZE);
  alarmasButtons[0].drawButton();

  Serial.println("alarmas dibujado");
}

void drawRelojScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(120 - (((6 * 2 * 5) - 3) / 2), 10);
  tft.print("Reloj");

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

  sprintf(buffer, "%d", relojYYYY);
  tft.setCursor(28, 60);
  tft.print(buffer);

  sprintf(buffer, "%02d", relojMM);
  tft.setCursor(28 + 72 + 20, 60);
  tft.print(buffer);

  sprintf(buffer, "%02d", relojDD);
  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
  tft.print(buffer);

  tft.fillTriangle(61, 107, 49, 91, 73, 91, ORANGE);     // año--
  tft.fillTriangle(136, 107, 124, 91, 148, 91, ORANGE);  // mes--
  tft.fillTriangle(192, 107, 180, 91, 204, 91, ORANGE);  // dia--

  tft.fillTriangle(89, 124, 77, 140, 101, 140, ORANGE);    // hora++
  tft.fillTriangle(146, 124, 134, 140, 158, 140, ORANGE);  // minuto++

  sprintf(buffer, "%02d", relojhh);
  tft.setCursor(74, 150);
  tft.print(buffer);

  sprintf(buffer, "%02d", relojmm);
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

  Serial.println("reloj dibujado");
}

void drawProgramasScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(110 - (((6 * 2 * 9) - 3) / 2), 10);
  tft.print("Programas");

  programasButtons[0].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY,
                                   WHITE, "Volver", BUTTON_TEXTSIZE);
  programasButtons[0].drawButton();

  Serial.println("programas dibujado");
}

void drawResetScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(120 - (((6 * 2 * 5) - 3) / 2), 10);
  tft.print("Reset");

  resetButtons[0].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                               "Volver", BUTTON_TEXTSIZE);
  resetButtons[0].drawButton();

  Serial.println("reset dibujado");
}

void drawZ1Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(120 - (((6 * 2 * 6) - 3) / 2), 10);
  tft.print("Zona 1");

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

  Serial.println("z1 dibujado");
}

void drawZ1F1Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(120 - (((6 * 2 * 6) - 3) / 2), 10);
  tft.print("Fase 1");

  z1f1Buttons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f1Buttons[0].drawRectButton();
  tft.setCursor(15, 40);
  tft.print("Dias");

  z1f1Buttons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f1Buttons[1].drawRectButton();
  tft.setCursor(15, 85);
  tft.print("Horas luz");

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
  tft.print("Temp.");

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
  tft.print("Riego");

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
  tft.print("Humedad");

  tft.setTextSize(3);

  //// dias
  sprintf(buffer, "%d", z1f1dias);

  tft.setCursor(228 - (strlen(buffer) * 18), 45);
  tft.print(buffer);

  //// hluz
  if (z1f1hLuz > 24) z1f1hLuz = 24;
  sprintf(buffer, "%d", z1f1hLuz);
  strcat(buffer, "H");
  tft.setCursor(228 - (strlen(buffer) * 18), 90);
  tft.print(buffer);

  //// temp
  if (z1f1temph > 80) z1f1temph = 80;
  sprintf(buffer, "%d", z1f1temph);
  uint8_t z1f1temphSTRlen = strlen(buffer);
  strcat(buffer, "C");
  tft.setCursor(228 - (strlen(buffer) * 18), 135);
  tft.print(buffer);

  if (z1f1templ > 80) z1f1templ = 80;
  sprintf(buffer, "%d", z1f1templ);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f1temphSTRlen * 18), 135);
  tft.print(buffer);

  tft.setCursor(192 - (z1f1temphSTRlen * 18), 135);
  tft.print("-");

  //// riego
  sprintf(buffer, "%d", z1f1riegoh);
  uint8_t z1f1riegohSTRlen = strlen(buffer);
  strcat(buffer, "%");
  tft.setCursor(228 - (strlen(buffer) * 18), 180);
  tft.print(buffer);

  sprintf(buffer, "%d", z1f1riegol);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f1riegohSTRlen * 18), 180);
  tft.print(buffer);

  tft.setCursor(192 - (z1f1riegohSTRlen * 18), 180);
  tft.print("-");

  //// humedad
  sprintf(buffer, "%d", z1f1humh);
  uint8_t z1f1humhSTRlen = strlen(buffer);
  strcat(buffer, "%");
  tft.setCursor(228 - (strlen(buffer) * 18), 225);
  tft.print(buffer);

  sprintf(buffer, "%d", z1f1huml);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f1humhSTRlen * 18), 225);
  tft.print(buffer);

  tft.setCursor(192 - (z1f1humhSTRlen * 18), 225);
  tft.print("-");

  z1f1Buttons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                              "Volver", BUTTON_TEXTSIZE);
  z1f1Buttons[4].drawButton();

  Serial.println("z1f1 dibujado");
}

void drawZ1F2Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(120 - (((6 * 2 * 6) - 3) / 2), 10);
  tft.print("Fase 2");

  z1f2Buttons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f2Buttons[0].drawRectButton();
  tft.setCursor(15, 40);
  tft.print("Dias");

  z1f2Buttons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f2Buttons[1].drawRectButton();
  tft.setCursor(15, 85);
  tft.print("Horas luz");

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
  tft.print("Temp.");

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
  tft.print("Riego");

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
  tft.print("Humedad");

  tft.setTextSize(3);

  //// dias
  sprintf(buffer, "%d", z1f2dias);

  tft.setCursor(228 - (strlen(buffer) * 18), 45);
  tft.print(buffer);

  //// hluz
  if (z1f2hLuz > 24) z1f2hLuz = 24;
  sprintf(buffer, "%d", z1f2hLuz);
  strcat(buffer, "H");
  tft.setCursor(228 - (strlen(buffer) * 18), 90);
  tft.print(buffer);

  //// temp
  if (z1f2temph > 80) z1f2temph = 80;
  sprintf(buffer, "%d", z1f2temph);
  uint8_t z1f2temphSTRlen = strlen(buffer);
  strcat(buffer, "C");
  tft.setCursor(228 - (strlen(buffer) * 18), 135);
  tft.print(buffer);

  if (z1f2templ > 80) z1f2templ = 80;
  sprintf(buffer, "%d", z1f2templ);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f2temphSTRlen * 18), 135);
  tft.print(buffer);

  tft.setCursor(192 - (z1f2temphSTRlen * 18), 135);
  tft.print("-");

  //// riego
  sprintf(buffer, "%d", z1f2riegoh);
  uint8_t z1f2riegohSTRlen = strlen(buffer);
  strcat(buffer, "%");
  tft.setCursor(228 - (strlen(buffer) * 18), 180);
  tft.print(buffer);

  sprintf(buffer, "%d", z1f2riegol);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f2riegohSTRlen * 18), 180);
  tft.print(buffer);

  tft.setCursor(192 - (z1f2riegohSTRlen * 18), 180);
  tft.print("-");

  //// humedad
  sprintf(buffer, "%d", z1f2humh);
  uint8_t z1f2humhSTRlen = strlen(buffer);
  strcat(buffer, "%");
  tft.setCursor(228 - (strlen(buffer) * 18), 225);
  tft.print(buffer);

  sprintf(buffer, "%d", z1f2huml);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f2humhSTRlen * 18), 225);
  tft.print(buffer);

  tft.setCursor(192 - (z1f2humhSTRlen * 18), 225);
  tft.print("-");

  z1f2Buttons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                              "Volver", BUTTON_TEXTSIZE);
  z1f2Buttons[4].drawButton();

  Serial.println("z1f2 dibujado");
}

void drawZ1F3Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(120 - (((6 * 2 * 6) - 3) / 2), 10);
  tft.print("Fase 3");

  z1f3Buttons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f3Buttons[0].drawRectButton();
  tft.setCursor(15, 40);
  tft.print("Dias");

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
  tft.print("Temp.");

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
  tft.print("Riego");

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
  tft.print("Humedad");

  tft.setTextSize(3);

  //// dias
  sprintf(buffer, "%d", z1f3dias);

  tft.setCursor(228 - (strlen(buffer) * 18), 45);
  tft.print(buffer);

  //// hluz
  if (z1f3hLuz > 24) z1f3hLuz = 24;
  sprintf(buffer, "%d", z1f3hLuz);
  strcat(buffer, "H");
  tft.setCursor(228 - (strlen(buffer) * 18), 90);
  tft.print(buffer);

  //// temp
  if (z1f3temph > 80) z1f3temph = 80;
  sprintf(buffer, "%d", z1f3temph);
  uint8_t z1f3temphSTRlen = strlen(buffer);
  strcat(buffer, "C");
  tft.setCursor(228 - (strlen(buffer) * 18), 135);
  tft.print(buffer);

  if (z1f3templ > 80) z1f3templ = 80;
  sprintf(buffer, "%d", z1f3templ);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f3temphSTRlen * 18), 135);
  tft.print(buffer);

  tft.setCursor(192 - (z1f3temphSTRlen * 18), 135);
  tft.print("-");

  //// riego
  sprintf(buffer, "%d", z1f3riegoh);
  uint8_t z1f3riegohSTRlen = strlen(buffer);
  strcat(buffer, "%");
  tft.setCursor(228 - (strlen(buffer) * 18), 180);
  tft.print(buffer);

  sprintf(buffer, "%d", z1f3riegol);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f3riegohSTRlen * 18), 180);
  tft.print(buffer);

  tft.setCursor(192 - (z1f3riegohSTRlen * 18), 180);
  tft.print("-");

  //// humedad
  sprintf(buffer, "%d", z1f3humh);
  uint8_t z1f3humhSTRlen = strlen(buffer);
  strcat(buffer, "%");
  tft.setCursor(228 - (strlen(buffer) * 18), 225);
  tft.print(buffer);

  sprintf(buffer, "%d", z1f3huml);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f3humhSTRlen * 18), 225);
  tft.print(buffer);

  tft.setCursor(192 - (z1f3humhSTRlen * 18), 225);
  tft.print("-");

  z1f3Buttons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                              "Volver", BUTTON_TEXTSIZE);
  z1f3Buttons[4].drawButton();

  Serial.println("z1f3 dibujado");
}

void drawZ1F4Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(120 - (((6 * 2 * 6) - 3) / 2), 10);
  tft.print("Fase 4");

  z1f4Buttons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f4Buttons[0].drawRectButton();
  tft.setCursor(15, 40);
  tft.print("Dias");

  z1f4Buttons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f4Buttons[1].drawRectButton();
  tft.setCursor(15, 85);
  tft.print("Horas luz");

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
  tft.print("Temp.");

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
  tft.print("Riego");

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
  tft.print("Humedad");

  tft.setTextSize(3);

  //// dias
  sprintf(buffer, "%d", z1f4dias);

  tft.setCursor(228 - (strlen(buffer) * 18), 45);
  tft.print(buffer);

  //// hluz
  if (z1f4hLuz > 24) z1f4hLuz = 24;
  sprintf(buffer, "%d", z1f4hLuz);
  strcat(buffer, "H");
  tft.setCursor(228 - (strlen(buffer) * 18), 90);
  tft.print(buffer);

  //// temp
  if (z1f4temph > 80) z1f4temph = 80;
  sprintf(buffer, "%d", z1f4temph);
  uint8_t z1f4temphSTRlen = strlen(buffer);
  strcat(buffer, "C");
  tft.setCursor(228 - (strlen(buffer) * 18), 135);
  tft.print(buffer);

  if (z1f4templ > 80) z1f4templ = 80;
  sprintf(buffer, "%d", z1f4templ);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f4temphSTRlen * 18), 135);
  tft.print(buffer);

  tft.setCursor(192 - (z1f4temphSTRlen * 18), 135);
  tft.print("-");

  //// riego
  sprintf(buffer, "%d", z1f4riegoh);
  uint8_t z1f4riegohSTRlen = strlen(buffer);
  strcat(buffer, "%");
  tft.setCursor(228 - (strlen(buffer) * 18), 180);
  tft.print(buffer);

  sprintf(buffer, "%d", z1f4riegol);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f4riegohSTRlen * 18), 180);
  tft.print(buffer);

  tft.setCursor(192 - (z1f4riegohSTRlen * 18), 180);
  tft.print("-");

  //// humedad
  sprintf(buffer, "%d", z1f4humh);
  uint8_t z1f4humhSTRlen = strlen(buffer);
  strcat(buffer, "%");
  tft.setCursor(228 - (strlen(buffer) * 18), 225);
  tft.print(buffer);

  sprintf(buffer, "%d", z1f4huml);
  tft.setCursor(192 - (strlen(buffer) * 18) - (z1f4humhSTRlen * 18), 225);
  tft.print(buffer);

  tft.setCursor(192 - (z1f4humhSTRlen * 18), 225);
  tft.print("-");

  z1f4Buttons[4].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                              "Volver", BUTTON_TEXTSIZE);
  z1f4Buttons[4].drawButton();

  Serial.println("z1f4 dibujado");
}

void drawZ1ControlScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(120 - (((6 * 2 * 7) - 3) / 2), 10);
  tft.print("Control");

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
  tft.print("Ciclos");

  sprintf(buffer, "%d", ciclos);
  tft.setCursor(228 - (strlen(buffer)*18), 135);
  tft.print(buffer);

  z1ControlButtons[2].initButtonUL(&tft, 5, 280, 230, 40, WHITE, LIGHTGREY,
                                   WHITE, "Volver", BUTTON_TEXTSIZE);
  z1ControlButtons[2].drawButton();

  Serial.println("z1control dibujado");
}

void drawZ1InicioScreen() {
  tft.fillScreen(BLACK);
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(120 - (((6 * 2 * 6) - 3) / 2), 10);
  tft.print("Inicio");

  tft.setTextSize(3);
  tft.setCursor(120 - (((6 * 3 * 5) - 3) / 2),
                40 - ((7 * BUTTON_TEXTSIZE) / 2));
  tft.print("Fases");

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

  Serial.println("z1inicio dibujado");
}

void drawNumericKeyboardScreen(const char* title) {
  numKBPrevScreen = prevScreen;
  Serial.println(numKBPrevScreen);

  tft.fillScreen(BLACK);

  tft.setTextSize(BUTTON_TEXTSIZE);

  numericKeyboardButtons[0].initButtonUL(&tft, 5, 5, 60, 30, WHITE, ORANGE,
                                         WHITE, "<", BUTTON_TEXTSIZE);
  numericKeyboardButtons[0].drawButton();

  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(152 - (6 * 2 * strlen(title)) / 2, 20 - (7 * 2) / 2);
  tft.print(title);

  tft.drawRect(5, 40, 230, 40, WHITE);

  tft.setTextSize(3);
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