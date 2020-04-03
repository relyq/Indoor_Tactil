void HomeScreen(){
  currentScreen = "home";
  drawHomeScreen();
  prevScreen = currentScreen;
}

void MenuScreen(){
  currentScreen = "menu";
  drawMenuScreen();
  prevScreen = currentScreen;
}

void AjustesScreen(){
  currentScreen = "ajustes";
  drawAjustesScreen();
  prevScreen = currentScreen;
}

void AlarmasScreen(){
  currentScreen = "alarmas";
  drawAlarmasScreen();
  prevScreen = currentScreen;
}

void RelojScreen(){
  currentScreen = "reloj";
  drawRelojScreen();
  prevScreen = currentScreen;
}

void ProgramasScreen(){
  currentScreen = "programas";
  drawProgramasScreen();
  prevScreen = currentScreen;
}

void ResetScreen(){
  currentScreen = "reset";
  drawResetScreen();
  prevScreen = currentScreen;
}

void Z1Screen(){
  currentScreen = "z1";
  drawZ1Screen();
  prevScreen = currentScreen;
}

void Z1F1Screen(){
  currentScreen = "z1f1";
  drawZ1F1Screen();
  prevScreen = currentScreen;
}

void Z1F2Screen(){
  currentScreen = "z1f2";
  drawZ1F2Screen();
  prevScreen = currentScreen;
}

void Z1F3Screen(){
  currentScreen = "z1f3";
  drawZ1F3Screen();
  prevScreen = currentScreen;
}

void Z1F4Screen(){
  currentScreen = "z1f4";
  drawZ1F4Screen();
  prevScreen = currentScreen;
}

void Z1ControlScreen(){
  currentScreen = "z1control";
  drawZ1ControlScreen();
  prevScreen = currentScreen;
}

void Z1InicioScreen(){
  currentScreen = "z1inicio";
  z1fSeleccionada = z1fActiva;
  drawZ1InicioScreen();
  prevScreen = currentScreen;
}



// pantalla de teclado numerico para modificar valores
// str          string a modificar
// bufferSize   el tamaño maximo de la string a modificar
void NumericKeyboardScreen(char* str, uint8_t bufferSize, char* title){
  currentScreen = "numKB";
  strcpy(numKBstr, str);
  numKBvarptr = str;
  numKBbufferSize = bufferSize;
  drawNumericKeyboardScreen(title);
  prevScreen = currentScreen;
}