void HomeScreen() {
  currentScreen = "home";
  drawHomeScreen();
  prevScreen = currentScreen;
}

void MenuScreen() {
  currentScreen = "menu";
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
  currentScreen = "ajustes";
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
  currentScreen = "alarmas";
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
  currentScreen = "reloj";
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
  currentScreen = "programas";
  drawProgramasScreen();
  strcpy(buffer, "hh:mm");
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void ResetScreen() {
  currentScreen = "reset";
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
  currentScreen = "z1";
  drawZ1Screen();
  strcpy(buffer, "hh:mm");
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Z1F1Screen() {
  currentScreen = "z1f1";
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
  currentScreen = "z1f2";
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
  currentScreen = "z1f3";
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
  currentScreen = "z1f4";
  drawZ1F4Screen();
  strcpy(buffer, "hh:mm");
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Z1ControlScreen() {
  currentScreen = "z1control";
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
  currentScreen = "z1inicio";
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

// pantalla de teclado numerico para modificar valores
// str          string a modificar
// bufferSize   el tamaño maximo de la string a modificar
void NumericKeyboardScreen(uint8_t* intptr, uint8_t bufferSize, char* title) {
  currentScreen = "numKB";
  sprintf(numKBstr, "%d", *intptr);
  numKBvarptr16b = NULL;
  numKBvarptr8b = intptr;
  numKBbufferSize = bufferSize;
  drawNumericKeyboardScreen(title);
  prevScreen = currentScreen;
}

void NumericKeyboardScreen(uint16_t* intptr, uint8_t bufferSize, char* title) {
  currentScreen = "numKB";
  sprintf(numKBstr, "%d", *intptr);
  numKBvarptr16b = intptr;
  numKBvarptr8b = NULL;
  numKBbufferSize = bufferSize;
  drawNumericKeyboardScreen(title);
  prevScreen = currentScreen;
}