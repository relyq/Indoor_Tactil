void HomeScreen() {
  currentScreen = 0;
  drawHomeScreen();
  prevScreen = currentScreen;
}

void MenuScreen() {
  currentScreen = 1;
  drawMenuScreen();
  strcpy_P(buffer, STR_hhmm);
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
  strcpy_P(buffer, STR_hhmm);
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
  strcpy_P(buffer, STR_hhmm);
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
  strcpy_P(buffer, STR_hhmm);
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
  strcpy_P(buffer, STR_hhmm);
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
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Programa2Screen() {
  currentScreen = 8;
  drawPrograma2Screen();
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Programa3Screen() {
  currentScreen = 9;
  drawPrograma3Screen();
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

void Programa4Screen() {
  currentScreen = 10;
  drawPrograma4Screen();
  strcpy_P(buffer, STR_hhmm);
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
  strcpy_P(buffer, STR_hhmm);
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
  strcpy_P(buffer, STR_hhmm);
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
  strcpy_P(buffer, STR_hhmm);
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
  strcpy_P(buffer, STR_hhmm);
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
  strcpy_P(buffer, STR_hhmm);
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
  strcpy_P(buffer, STR_hhmm);
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
  strcpy_P(buffer, STR_hhmm);
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
  strcpy_P(buffer, STR_hhmm);
  now.toString(buffer);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 10);
  tft.print(buffer);
  prevScreen = currentScreen;
}

// pantalla de teclado numerico para modificar valores
// quiza puedo hacer una sola funcion con void*
// intptr       ptr al int a modificar
// eepromdir    dirección eeprom en la que guardar el valor
// bufferSize   digitos maximos de la string a modificar
// title        titulo de la pantalla de teclado
void NumericKeyboardScreen(uint8_t* intptr, uint16_t eepromdir,
                           uint8_t bufferSize, const char* title) {
  currentScreen = 255;
  sprintf_P(numKBstr, STR_fdecimal, *intptr);
  numKBvarptr16b = NULL;
  numKBvarptr8b = intptr;
  numKBeeprom = eepromdir;
  numKBbufferSize = bufferSize;
  drawNumericKeyboardScreen(title);
  prevScreen = currentScreen;
}

void NumericKeyboardScreen(uint16_t* intptr, uint16_t eepromdir,
                           uint8_t bufferSize, const char* title) {
  currentScreen = 255;
  sprintf_P(numKBstr, STR_fdecimal, *intptr);
  numKBvarptr16b = intptr;
  numKBvarptr8b = NULL;
  numKBeeprom = eepromdir;
  numKBbufferSize = bufferSize;
  drawNumericKeyboardScreen(title);
  prevScreen = currentScreen;
}