void HomeScreen() {
  currentScreen = 0;
  drawHomeScreen();
  prevScreen = currentScreen;
}

void MenuScreen() {
  currentScreen = 1;
  drawMenuScreen();
  strcpy(buffer, "hh:mm");
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void AjustesScreen() {
  currentScreen = 2;
  drawAjustesScreen();
  strcpy(buffer, "hh:mm");
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void AlarmasScreen() {
  currentScreen = 3;
  drawAlarmasScreen();
  strcpy(buffer, "hh:mm");
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void RelojScreen() {
  currentScreen = 4;
  drawRelojScreen();
  strcpy(buffer, "hh:mm");
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void ProgramasScreen() {
  currentScreen = 5;
  drawProgramasScreen();
  strcpy(buffer, "hh:mm");
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Programa1Screen() {
  currentScreen = 7;
  drawPrograma1Screen();
  strcpy(buffer, "hh:mm");
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void ResetScreen() {
  currentScreen = 6;
  drawResetScreen();
  strcpy(buffer, "hh:mm");
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Z1Screen() {
  currentScreen = 30;
  drawZ1Screen();
  strcpy(buffer, "hh:mm");
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Z1ControlScreen() {
  currentScreen = 31;
  drawZ1ControlScreen();
  strcpy(buffer, "hh:mm");
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Z1InicioScreen() {
  currentScreen = 32;
  z1fSeleccionada = z1fActiva;
  drawZ1InicioScreen();
  strcpy(buffer, "hh:mm");
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Z1F1Screen() {
  currentScreen = 33;
  drawZ1F1Screen();
  strcpy(buffer, "hh:mm");
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Z1F2Screen() {
  currentScreen = 34;
  drawZ1F2Screen();
  strcpy(buffer, "hh:mm");
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Z1F3Screen() {
  currentScreen = 35;
  drawZ1F3Screen();
  strcpy(buffer, "hh:mm");
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Z1F4Screen() {
  currentScreen = 36;
  drawZ1F4Screen();
  strcpy(buffer, "hh:mm");
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

// pantalla de teclado numerico para modificar valores
// str          string a modificar
// bufferSize   el tamaño maximo de la string a modificar
void NumericKeyboardScreen(uint8_t* intptr, uint8_t bufferSize,
                           const char* title) {
  currentScreen = 255;
  sprintf(numKBstr, "%d", *intptr);
  numKBvarptr16b = NULL;
  numKBvarptr8b = intptr;
  numKBbufferSize = bufferSize;
  drawNumericKeyboardScreen(title);
  prevScreen = currentScreen;
}

void NumericKeyboardScreen(uint16_t* intptr, uint8_t bufferSize,
                           const char* title) {
  currentScreen = 255;
  sprintf(numKBstr, "%d", *intptr);
  numKBvarptr16b = intptr;
  numKBvarptr8b = NULL;
  numKBbufferSize = bufferSize;
  drawNumericKeyboardScreen(title);
  prevScreen = currentScreen;
}