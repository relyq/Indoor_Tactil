void tsMenu() {
  TSPoint p = ts.getPoint();

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  // pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  // pinMode(YM, OUTPUT);

  // si la pantalla está siendo presionada se mappea el resultado de getPoint()
  // a un punto valido del cursor en la pantalla
  // no es necesario refrescar la pantalla si no cambió nada ni está siendo
  // presionada
  if (p.z > MINPRESSURE) {
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    // if the screen is being touched show cursor position
    Serial.print("(");
    Serial.print(p.x);
    Serial.print(", ");
    Serial.print(p.y);
    Serial.print(", ");
    Serial.print(p.z);
    Serial.println(") ");

    if (currentScreen == "home") {
      if (p.x > (120 - 115) && p.x < (120 + 115) && p.y > 10 && p.y < 50) {
        MenuScreen();
      }
      /*if (homeButtons[0].contains(p.x, p.y)) {
        MenuScreen();
      }*/
    } else if (currentScreen == "menu") {
      if (menuButtons[0].contains(p.x, p.y)) {
        Z1Screen();
      } else if (menuButtons[1].contains(p.x, p.y)) {
        AjustesScreen();
      } else if (menuButtons[2].contains(p.x, p.y)) {
        HomeScreen();
      }
    } else if (currentScreen == "ajustes") {
      if (ajustesButtons[4].contains(p.x, p.y)) {
        MenuScreen();
      } else if (ajustesButtons[0].contains(p.x, p.y)) {
        AlarmasScreen();
      } else if (ajustesButtons[1].contains(p.x, p.y)) {
        RelojScreen();
      } else if (ajustesButtons[2].contains(p.x, p.y)) {
        ProgramasScreen();
      } else if (ajustesButtons[3].contains(p.x, p.y)) {
        ResetScreen();
      }
    } else if (currentScreen == "alarmas") {
      if (alarmasButtons[0].contains(p.x, p.y)) {
        AjustesScreen();
      }
    } else if (currentScreen == "reloj") {
      if (relojButtons[0].contains(p.x, p.y)) {
        AjustesScreen();
      }
    } else if (currentScreen == "programas") {
      if (programasButtons[0].contains(p.x, p.y)) {
        AjustesScreen();
      }
    } else if (currentScreen == "reset") {
      if (resetButtons[0].contains(p.x, p.y)) {
        AjustesScreen();
      }
    } else if (currentScreen == "z1") {
      if (z1Buttons[5].contains(p.x, p.y)) {
        MenuScreen();
      } else if (z1Buttons[0].contains(p.x, p.y)) {
        Z1F1Screen();
      } else if (z1Buttons[1].contains(p.x, p.y)) {
        Z1F2Screen();
      } else if (z1Buttons[2].contains(p.x, p.y)) {
        Z1F3Screen();
      } else if (z1Buttons[3].contains(p.x, p.y)) {
        Z1F4Screen();
      } else if (z1Buttons[4].contains(p.x, p.y)) {
        Z1ControlScreen();
      }
    } else if (currentScreen == "z1f1") {
      if (z1f1Buttons[4].contains(p.x, p.y)) {
        Z1Screen();
      }
    } else if (currentScreen == "z1f2") {
      if (z1f2Buttons[4].contains(p.x, p.y)) {
        Z1Screen();
      }
    } else if (currentScreen == "z1f3") {
      if (z1f3Buttons[4].contains(p.x, p.y)) {
        Z1Screen();
      }
    } else if (currentScreen == "z1f4") {
      if (z1f4Buttons[4].contains(p.x, p.y)) {
        Z1Screen();
      }
    } else if (currentScreen == "z1control") {
      if (z1ControlButtons[2].contains(p.x, p.y)) {
        Z1Screen();
      }
    }
  }
}