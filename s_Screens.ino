void drawHomeScreen() {
  tft.fillScreen(BLACK);

  // menu principal
  homeButtons[0].initButton(&tft, 120, 30, 230, 40, YELLOW, DARKGREEN, YELLOW,
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

  if (digitalRead(FANPIN)) {
    tft.fillCircle(180, 94, 10, GREEN);
  } else {
    tft.fillCircle(180, 94, 10, RED);
  }

  tft.setCursor(10, 115);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.print("Vaporizacion");

  if (digitalRead(VAPPIN)) {
    tft.fillCircle(180, 119, 10, GREEN);
  } else {
    tft.fillCircle(180, 119, 10, RED);
  }

  tft.setCursor(10, 140);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.print("Riego");

  if (digitalRead(RIEGOPIN)) {
    tft.fillCircle(180, 144, 10, GREEN);
  } else {
    tft.fillCircle(180, 144, 10, RED);
  }

  strcpy(buffer, tempSPstr);
  strcat(buffer, "C");
  tft.setCursor(10, 175);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Temp. ");
  tft.setTextColor(ORANGE);
  tft.print(buffer);  // temperatura configurada

  strcpy(buffer, humSPstr);
  strcat(buffer, "%");
  tft.setCursor(10, 250);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Humedad ");
  tft.setTextColor(BLUE);
  tft.print(buffer);  // humedad configurada

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

  Serial.println("home dibujada");
}

void drawMenuScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(120 - (((6 * 2 * 14) - 3) / 2), 10);
  tft.print("Menu Principal");

  // boton 1 - zona 1
  menuButtons[0].initButton(&tft, 120, 55, 230, 40, WHITE, ORANGE, YELLOW,
                            "Zona 1", BUTTON_TEXTSIZE);
  menuButtons[0].drawButton();

  // boton 2 - ajustes
  menuButtons[1].initButton(&tft, 120, 100, 230, 40, WHITE, ORANGE, YELLOW,
                            "Ajustes", BUTTON_TEXTSIZE);
  menuButtons[1].drawButton();

  menuButtons[2].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
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
  ajustesButtons[0].initButton(&tft, 120, 55, 230, 40, WHITE, ORANGE, YELLOW,
                               "Alarmas", BUTTON_TEXTSIZE);
  ajustesButtons[0].drawButton();

  // boton 2 - reloj
  ajustesButtons[1].initButton(&tft, 120, 100, 230, 40, WHITE, ORANGE, YELLOW,
                               "Reloj", BUTTON_TEXTSIZE);
  ajustesButtons[1].drawButton();

  // boton 3 - programas
  ajustesButtons[2].initButton(&tft, 120, 145, 230, 40, WHITE, ORANGE, YELLOW,
                               "Programas", BUTTON_TEXTSIZE);
  ajustesButtons[2].drawButton();

  // boton 4 - hard reset
  ajustesButtons[3].initButton(&tft, 120, 190, 230, 40, WHITE, ORANGE, YELLOW,
                               "Reset", BUTTON_TEXTSIZE);
  ajustesButtons[3].drawButton();

  // boton 5 - Volver
  ajustesButtons[4].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
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

  alarmasButtons[0].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
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

  relojButtons[0].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                             "Volver", BUTTON_TEXTSIZE);
  relojButtons[0].drawButton();

  Serial.println("reloj dibujado");
}

void drawProgramasScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(120 - (((6 * 2 * 9) - 3) / 2), 10);
  tft.print("Programas");

  programasButtons[0].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY,
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

  resetButtons[0].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
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
  z1Buttons[0].initButton(&tft, 120, 55, 230, 40, WHITE, ORANGE, YELLOW,
                          "Fase 1", BUTTON_TEXTSIZE);
  z1Buttons[0].drawButton();

  // boton 2 - fase 2
  z1Buttons[1].initButton(&tft, 120, 100, 230, 40, WHITE, ORANGE, YELLOW,
                          "Fase 2", BUTTON_TEXTSIZE);
  z1Buttons[1].drawButton();

  // boton 3 - fase 3
  z1Buttons[2].initButton(&tft, 120, 145, 230, 40, WHITE, ORANGE, YELLOW,
                          "Fase 3", BUTTON_TEXTSIZE);
  z1Buttons[2].drawButton();

  // boton 4 - fase 4
  z1Buttons[3].initButton(&tft, 120, 190, 230, 40, WHITE, ORANGE, YELLOW,
                          "Fase 4", BUTTON_TEXTSIZE);
  z1Buttons[3].drawButton();

  // boton 5 - control
  z1Buttons[4].initButton(&tft, 120, 235, 230, 40, WHITE, ORANGE, YELLOW,
                          "Control", BUTTON_TEXTSIZE);
  z1Buttons[4].drawButton();

  // boton 6 - volver
  z1Buttons[5].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                          "Volver", BUTTON_TEXTSIZE);
  z1Buttons[5].drawButton();

  Serial.println("z1 dibujado");
}

void drawZ1F1Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(120 - (((6 * 2 * 15) - 3) / 2), 10);
  tft.print("Zona 1 - Fase 1");

  z1f1Buttons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f1Buttons[0].drawRectButton();
  tft.setCursor(15, 40);
  tft.print("Dias");

  z1f1Buttons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f1Buttons[1].drawRectButton();
  tft.setCursor(15, 85);
  tft.print("Horas luz");

  z1f1Buttons[2].initButtonUL(&tft, 5, 125, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f1Buttons[2].drawRectButton();

  tft.setCursor(15, 130);
  tft.print("Temp.");

  z1f1Buttons[3].initButtonUL(&tft, 5, 170, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f1Buttons[3].drawRectButton();
  tft.setCursor(15, 175);
  tft.print("Riego");

  tft.setTextSize(3);

  strcpy(buffer, z1f1diasSPstr);

  tft.setCursor(175, 45);
  tft.print(buffer);

  if (strtol(z1f1hluzSPstr, 0, 10) > 24) strcpy(z1f1hluzSPstr, "24");

  strcpy(buffer, z1f1hluzSPstr);
  strcat(buffer, "H");

  tft.setCursor(175, 90);
  tft.print(buffer);

  strcpy(buffer, z1f1tempSPstr);
  strcat(buffer, "C");

  tft.setCursor(175, 135);
  tft.print(buffer);

  strcpy(buffer, z1f1riegoSPstr);
  strcat(buffer, "%");

  tft.setCursor(175, 180);
  tft.print(buffer);

  z1f1Buttons[4].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                            "Volver", BUTTON_TEXTSIZE);
  z1f1Buttons[4].drawButton();

  Serial.println("z1f1 dibujado");
}

void drawZ1F2Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(120 - (((6 * 2 * 15) - 3) / 2), 10);
  tft.print("Zona 1 - Fase 2");

  z1f2Buttons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f2Buttons[0].drawRectButton();
  tft.setCursor(15, 40);
  tft.print("Dias");

  z1f2Buttons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f2Buttons[1].drawRectButton();
  tft.setCursor(15, 85);
  tft.print("Horas luz");

  z1f2Buttons[2].initButtonUL(&tft, 5, 125, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f2Buttons[2].drawRectButton();

  tft.setCursor(15, 130);
  tft.print("Temp.");

  z1f2Buttons[3].initButtonUL(&tft, 5, 170, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f2Buttons[3].drawRectButton();
  tft.setCursor(15, 175);
  tft.print("Riego");

  tft.setTextSize(3);

  strcpy(buffer, z1f2diasSPstr);

  tft.setCursor(175, 45);
  tft.print(buffer);

  if (strtol(z1f2hluzSPstr, 0, 10) > 24) strcpy(z1f2hluzSPstr, "24");

  strcpy(buffer, z1f2hluzSPstr);
  strcat(buffer, "H");

  tft.setCursor(175, 90);
  tft.print(buffer);

  strcpy(buffer, z1f2tempSPstr);
  strcat(buffer, "C");

  tft.setCursor(175, 135);
  tft.print(buffer);

  strcpy(buffer, z1f2riegoSPstr);
  strcat(buffer, "%");

  tft.setCursor(175, 180);
  tft.print(buffer);

  z1f2Buttons[4].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                            "Volver", BUTTON_TEXTSIZE);
  z1f2Buttons[4].drawButton();

  Serial.println("z1f2 dibujado");
}

void drawZ1F3Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(120 - (((6 * 2 * 15) - 3) / 2), 10);
  tft.print("Zona 1 - Fase 3");

  z1f3Buttons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f3Buttons[0].drawRectButton();
  tft.setCursor(15, 40);
  tft.print("Dias");

  z1f3Buttons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f3Buttons[1].drawRectButton();
  tft.setCursor(15, 85);
  tft.print("Horas luz");

  z1f3Buttons[2].initButtonUL(&tft, 5, 125, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f3Buttons[2].drawRectButton();

  tft.setCursor(15, 130);
  tft.print("Temp.");

  z1f3Buttons[3].initButtonUL(&tft, 5, 170, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f3Buttons[3].drawRectButton();
  tft.setCursor(15, 175);
  tft.print("Riego");

  tft.setTextSize(3);

  strcpy(buffer, z1f3diasSPstr);

  tft.setCursor(175, 45);
  tft.print(buffer);

  if (strtol(z1f3hluzSPstr, 0, 10) > 24) strcpy(z1f3hluzSPstr, "24");

  strcpy(buffer, z1f3hluzSPstr);
  strcat(buffer, "H");

  tft.setCursor(175, 90);
  tft.print(buffer);

  strcpy(buffer, z1f3tempSPstr);
  strcat(buffer, "C");

  tft.setCursor(175, 135);
  tft.print(buffer);

  strcpy(buffer, z1f3riegoSPstr);
  strcat(buffer, "%");

  tft.setCursor(175, 180);
  tft.print(buffer);

  z1f3Buttons[4].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                            "Volver", BUTTON_TEXTSIZE);
  z1f3Buttons[4].drawButton();

  Serial.println("z1f3 dibujado");
}

void drawZ1F4Screen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(120 - (((6 * 2 * 15) - 3) / 2), 10);
  tft.print("Zona 1 - Fase 4");

  z1f4Buttons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f4Buttons[0].drawRectButton();
  tft.setCursor(15, 40);
  tft.print("Dias");

  z1f4Buttons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, BLACK, WHITE, "", 2);
  z1f4Buttons[1].drawRectButton();
  tft.setCursor(15, 85);
  tft.print("Horas luz");

  z1f4Buttons[2].initButtonUL(&tft, 5, 125, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f4Buttons[2].drawRectButton();

  tft.setCursor(15, 130);
  tft.print("Temp.");

  z1f4Buttons[3].initButtonUL(&tft, 5, 170, 230, 40, WHITE, BLACK, WHITE, "",
                              2);
  z1f4Buttons[3].drawRectButton();
  tft.setCursor(15, 175);
  tft.print("Riego");

  tft.setTextSize(3);

  strcpy(buffer, z1f4diasSPstr);

  tft.setCursor(175, 45);
  tft.print(buffer);

  if (strtol(z1f4hluzSPstr, 0, 10) > 24) strcpy(z1f4hluzSPstr, "24");

  strcpy(buffer, z1f4hluzSPstr);
  strcat(buffer, "H");

  tft.setCursor(175, 90);
  tft.print(buffer);

  strcpy(buffer, z1f4tempSPstr);
  strcat(buffer, "C");

  tft.setCursor(175, 135);
  tft.print(buffer);

  strcpy(buffer, z1f4riegoSPstr);
  strcat(buffer, "%");

  tft.setCursor(175, 180);
  tft.print(buffer);

  z1f4Buttons[4].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                            "Volver", BUTTON_TEXTSIZE);
  z1f4Buttons[4].drawButton();

  Serial.println("z1f4 dibujado");
}

void drawZ1ControlScreen() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(120 - (((6 * 2 * 16) - 3) / 2), 10);
  tft.print("Zona 1 - Control");

  z1ControlButtons[0].initButtonUL(&tft, 5, 35, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Inicio", BUTTON_TEXTSIZE);
  z1ControlButtons[0].drawButton();

  z1ControlButtons[1].initButtonUL(&tft, 5, 80, 230, 40, WHITE, ORANGE, YELLOW,
                                   "Terminar", BUTTON_TEXTSIZE);
  z1ControlButtons[1].drawButton();

  z1ControlButtons[2].initButtonUL(&tft, 5, 260, 230, 40, WHITE, LIGHTGREY,
                                   WHITE, "Volver", BUTTON_TEXTSIZE);
  z1ControlButtons[2].drawButton();

  Serial.println("z1control dibujado");
}

void drawZ1InicioScreen() {
  tft.fillScreen(BLACK);
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(120 - (((6 * 2 * 15) - 3) / 2), 10);
  tft.print("Zona 1 - Inicio");

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

  z1InicioButtons[5].initButtonUL(&tft, 5, 260, 230, 40, WHITE, LIGHTGREY,
                                  WHITE, "Volver", BUTTON_TEXTSIZE);
  z1InicioButtons[5].drawButton();

  Serial.println("z1inicio dibujado");
}

void drawNumericKeyboardScreen() {
  numKBPrevScreen = prevScreen;
  Serial.println(numKBPrevScreen);

  tft.fillScreen(BLACK);

  tft.setTextSize(BUTTON_TEXTSIZE);

  numericKeyboardButtons[0].initButtonUL(&tft, 5, 5, 60, 30, WHITE, ORANGE,
                                         WHITE, "<", BUTTON_TEXTSIZE);
  numericKeyboardButtons[0].drawButton();

  tft.drawRect(5, 40, 230, 40, WHITE);

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