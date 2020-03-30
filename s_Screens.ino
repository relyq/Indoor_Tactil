void drawHomeScreen() {
  tft.fillScreen(BLACK);

  // menu principal
  homeButtons[0].initButton(&tft, 120, 30, 230, 40, YELLOW, DARKGREEN, YELLOW,
                            "Menu", BUTTON_TEXTSIZE);
  homeButtons[0].drawButton();

  tft.setCursor(10, 65);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.print("Iluminacion   ");

  tft.fillCircle(180, 69, 10, RED);

  tft.setCursor(10, 90);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.print("Ventilacion   ");

  if (digitalRead(FANPIN)) {
    tft.fillCircle(180, 94, 10, GREEN);
  } else {
    tft.fillCircle(180, 94, 10, RED);
  }

  tft.setCursor(10, 115);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.print("Vaporizacion   ");

  if (digitalRead(VAPPIN)) {
    tft.fillCircle(180, 119, 10, GREEN);
  } else {
    tft.fillCircle(180, 119, 10, RED);
  }

  tft.setCursor(10, 140);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.print("Riego   ");

  if (digitalRead(RIEGOPIN)) {
    tft.fillCircle(180, 144, 10, GREEN);
  } else {
    tft.fillCircle(180, 144, 10, RED);
  }

  sprintf(tempSPstr, "%d", tempSP);
  sprintf(humSPstr, "%d",
          humSP);  // sprintf(*str, format, value1, value2, etc...)

  tft.setCursor(10, 175);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Temp. ");
  tft.setTextColor(ORANGE);
  tft.print(strcat(tempSPstr, "C"));  // temperatura configurada

  tft.setCursor(10, 250);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Humedad ");
  tft.setTextColor(BLUE);
  tft.print(strcat(humSPstr, "%"));  // humedad configurada

  dtostrf(t, 5, 1, temperatura);
  dtostrf(h, 5, 0, humedad);
  tft.setCursor(10, 200);
  tft.setTextSize(5);
  tft.setTextColor(WHITE, BLACK);
  tft.print(strcat(temperatura, " C"));  // temperatura leida por el DHT

  tft.setCursor(10, 275);
  tft.setTextSize(5);
  tft.setTextColor(WHITE, BLACK);
  tft.print(strcat(humedad, " %"));  // humedad leida por el DHT

  Serial.println("home dibujada");
}

void drawMenuScreen() {
  tft.fillScreen(BLACK);
  tft.setCursor(35, 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
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
  tft.setCursor(35, 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
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
  tft.setCursor(35, 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Alarmas");

  alarmasButtons[0].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                               "Volver", BUTTON_TEXTSIZE);
  alarmasButtons[0].drawButton();

  Serial.println("alarmas dibujado");
}

void drawRelojScreen() {
  tft.fillScreen(BLACK);
  tft.setCursor(35, 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Reloj");

  relojButtons[0].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                             "Volver", BUTTON_TEXTSIZE);
  relojButtons[0].drawButton();

  Serial.println("reloj dibujado");
}

void drawProgramasScreen() {
  tft.fillScreen(BLACK);
  tft.setCursor(35, 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Programas");

  programasButtons[0].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY,
                                 WHITE, "Volver", BUTTON_TEXTSIZE);
  programasButtons[0].drawButton();

  Serial.println("programas dibujado");
}

void drawResetScreen() {
  tft.fillScreen(BLACK);
  tft.setCursor(35, 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Reset");

  resetButtons[0].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                             "Volver", BUTTON_TEXTSIZE);
  resetButtons[0].drawButton();

  Serial.println("reset dibujado");
}

void drawZ1Screen() {
  tft.fillScreen(BLACK);
  tft.setCursor(35, 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
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
  tft.setCursor(35, 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Zona 1 - Fase 1");

  tft.drawRect(5, 35, 230, 40, WHITE);
  tft.setCursor(15, 40);
  tft.print("Dias");

  tft.drawRect(5, 80, 230, 40, WHITE);
  tft.setCursor(15, 85);
  tft.print("Horas luz");

  tft.drawRect(5, 125, 230, 40, WHITE);
  tft.setCursor(15, 130);
  tft.print("Temp.");

  tft.drawRect(5, 170, 230, 40, WHITE);
  tft.setCursor(15, 175);
  tft.print("Riego");

  tft.setTextSize(3);

  tft.setCursor(175, 45);
  tft.print("90");

  tft.setCursor(175, 90);
  tft.print("16");

  tft.setCursor(175, 135);
  tft.print(tempSPstr);

  tft.setCursor(175, 180);
  tft.print("60%");

  z1f1Buttons[4].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                            "Volver", BUTTON_TEXTSIZE);
  z1f1Buttons[4].drawButton();

  Serial.println("z1f1 dibujado");
}

void drawZ1F2Screen() {
  tft.fillScreen(BLACK);
  tft.setCursor(35, 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Zona 1 - Fase 2");

  z1f2Buttons[0].initButton(&tft, 120, 55, 230, 40, WHITE, ORANGE, YELLOW,
                            "Dias", BUTTON_TEXTSIZE);
  z1f2Buttons[0].drawButton();

  z1f2Buttons[1].initButton(&tft, 120, 100, 230, 40, WHITE, ORANGE, YELLOW,
                            "Horas luz", BUTTON_TEXTSIZE);
  z1f2Buttons[1].drawButton();

  z1f2Buttons[2].initButton(&tft, 120, 145, 230, 40, WHITE, ORANGE, YELLOW,
                            "Temp.", BUTTON_TEXTSIZE);
  z1f2Buttons[2].drawButton();

  z1f2Buttons[3].initButton(&tft, 120, 190, 230, 40, WHITE, ORANGE, YELLOW,
                            "H. Tierra", BUTTON_TEXTSIZE);
  z1f2Buttons[3].drawButton();

  z1f2Buttons[4].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                            "Volver", BUTTON_TEXTSIZE);
  z1f2Buttons[4].drawButton();

  Serial.println("z1f2 dibujado");
}

void drawZ1F3Screen() {
  tft.fillScreen(BLACK);
  tft.setCursor(35, 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Zona 1 - Fase 3");

  z1f3Buttons[0].initButton(&tft, 120, 55, 230, 40, WHITE, ORANGE, YELLOW,
                            "Dias", BUTTON_TEXTSIZE);
  z1f3Buttons[0].drawButton();

  z1f3Buttons[1].initButton(&tft, 120, 100, 230, 40, WHITE, ORANGE, YELLOW,
                            "Horas luz", BUTTON_TEXTSIZE);
  z1f3Buttons[1].drawButton();

  z1f3Buttons[2].initButton(&tft, 120, 145, 230, 40, WHITE, ORANGE, YELLOW,
                            "Temp.", BUTTON_TEXTSIZE);
  z1f3Buttons[2].drawButton();

  z1f3Buttons[3].initButton(&tft, 120, 190, 230, 40, WHITE, ORANGE, YELLOW,
                            "H. Tierra", BUTTON_TEXTSIZE);
  z1f3Buttons[3].drawButton();

  z1f3Buttons[4].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                            "Volver", BUTTON_TEXTSIZE);
  z1f3Buttons[4].drawButton();

  Serial.println("z1f3 dibujado");
}

void drawZ1F4Screen() {
  tft.fillScreen(BLACK);
  tft.setCursor(35, 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Zona 1 - Fase 4");

  z1f4Buttons[0].initButton(&tft, 120, 55, 230, 40, WHITE, ORANGE, YELLOW,
                            "Dias", BUTTON_TEXTSIZE);
  z1f4Buttons[0].drawButton();

  z1f4Buttons[1].initButton(&tft, 120, 100, 230, 40, WHITE, ORANGE, YELLOW,
                            "Horas luz", BUTTON_TEXTSIZE);
  z1f4Buttons[1].drawButton();

  z1f4Buttons[2].initButton(&tft, 120, 145, 230, 40, WHITE, ORANGE, YELLOW,
                            "Temp.", BUTTON_TEXTSIZE);
  z1f4Buttons[2].drawButton();

  z1f4Buttons[3].initButton(&tft, 120, 190, 230, 40, WHITE, ORANGE, YELLOW,
                            "H. Tierra", BUTTON_TEXTSIZE);
  z1f4Buttons[3].drawButton();

  z1f4Buttons[4].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY, WHITE,
                            "Volver", BUTTON_TEXTSIZE);
  z1f4Buttons[4].drawButton();

  Serial.println("z1f4 dibujado");
}

void drawZ1ControlScreen() {
  tft.fillScreen(BLACK);
  tft.setCursor(35, 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Zona 1 - Control");

  z1ControlButtons[0].initButton(&tft, 120, 55, 230, 40, WHITE, ORANGE, YELLOW,
                                 "Inicio", BUTTON_TEXTSIZE);
  z1ControlButtons[0].drawButton();

  z1ControlButtons[1].initButton(&tft, 120, 100, 230, 40, WHITE, ORANGE, YELLOW,
                                 "Terminar", BUTTON_TEXTSIZE);
  z1ControlButtons[1].drawButton();

  z1ControlButtons[2].initButton(&tft, 120, 280, 230, 40, WHITE, LIGHTGREY,
                                 WHITE, "Volver", BUTTON_TEXTSIZE);
  z1ControlButtons[2].drawButton();

  Serial.println("z1control dibujado");
}