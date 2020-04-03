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
    Serial.print("Unmapped p: ");
    Serial.print("(");
    Serial.print(p.x);
    Serial.print(", ");
    Serial.print(p.y);
    Serial.print(", ");
    Serial.print(p.z);
    Serial.print(") ");

    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY - 60, tft.height(), 0);
    // if the screen is being touched show cursor position

    Serial.print("Mapped p: ");
    Serial.print("(");
    Serial.print(p.x);
    Serial.print(", ");
    Serial.print(p.y);
    Serial.print(", ");
    Serial.print(p.z);
    Serial.println(") ");

    if (currentScreen == "home") {  // deberia usar un switch
      if (homeButtons[0].contains(p.x, p.y)) {
        MenuScreen();
      }
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
      } else if (z1f1Buttons[0].contains(p.x, p.y)) {
        NumericKeyboardScreen(z1f1diasSPstr, 3);
      } else if (z1f1Buttons[1].contains(p.x, p.y)) {
        NumericKeyboardScreen(z1f1hluzSPstr, 2);
      } else if (z1f1Buttons[2].contains(p.x, p.y)) {
        NumericKeyboardScreen(z1f1tempSPstr, 2);
      } else if (z1f1Buttons[3].contains(p.x, p.y)) {
        NumericKeyboardScreen(z1f1riegoSPstr, 2);
      }
    } else if (currentScreen == "z1f2") {
      if (z1f2Buttons[4].contains(p.x, p.y)) {
        Z1Screen();
      } else if (z1f2Buttons[0].contains(p.x, p.y)) {
        NumericKeyboardScreen(z1f2diasSPstr, 3);
      } else if (z1f2Buttons[1].contains(p.x, p.y)) {
        NumericKeyboardScreen(z1f2hluzSPstr, 2);
      } else if (z1f2Buttons[2].contains(p.x, p.y)) {
        NumericKeyboardScreen(z1f2tempSPstr, 2);
      } else if (z1f2Buttons[3].contains(p.x, p.y)) {
        NumericKeyboardScreen(z1f2riegoSPstr, 2);
      }
    } else if (currentScreen == "z1f3") {
      if (z1f3Buttons[4].contains(p.x, p.y)) {
        Z1Screen();
      } else if (z1f2Buttons[0].contains(p.x, p.y)) {
        NumericKeyboardScreen(z1f3diasSPstr, 3);
      } else if (z1f2Buttons[1].contains(p.x, p.y)) {
        NumericKeyboardScreen(z1f3hluzSPstr, 2);
      } else if (z1f2Buttons[2].contains(p.x, p.y)) {
        NumericKeyboardScreen(z1f3tempSPstr, 2);
      } else if (z1f2Buttons[3].contains(p.x, p.y)) {
        NumericKeyboardScreen(z1f3riegoSPstr, 2);
      }
    } else if (currentScreen == "z1f4") {
      if (z1f4Buttons[4].contains(p.x, p.y)) {
        Z1Screen();
      } else if (z1f2Buttons[0].contains(p.x, p.y)) {
        NumericKeyboardScreen(z1f4diasSPstr, 3);
      } else if (z1f2Buttons[1].contains(p.x, p.y)) {
        NumericKeyboardScreen(z1f4hluzSPstr, 2);
      } else if (z1f2Buttons[2].contains(p.x, p.y)) {
        NumericKeyboardScreen(z1f4tempSPstr, 2);
      } else if (z1f2Buttons[3].contains(p.x, p.y)) {
        NumericKeyboardScreen(z1f4riegoSPstr, 2);
      }
    } else if (currentScreen == "z1control") {
      if (z1ControlButtons[2].contains(p.x, p.y)) {
        Z1Screen();
      } else if (z1ControlButtons[0].contains(p.x, p.y)) {
        Z1InicioScreen();
      }
    } else if (currentScreen == "z1inicio") {
      if (z1InicioButtons[5].contains(p.x, p.y)) {
        Z1ControlScreen();
      } else if (z1InicioButtons[0].contains(p.x, p.y) &&
                 (z1fActiva != 1 && z1fSeleccionada != 1)) {
        z1fSeleccionada = 1;
        z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, YELLOW,
                                        WHITE, "1", BUTTON_TEXTSIZE);

        z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE, DARKGREY,
                                        WHITE, "2", BUTTON_TEXTSIZE);

        z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE, DARKGREY,
                                        WHITE, "3", BUTTON_TEXTSIZE);

        z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE, DARKGREY,
                                        WHITE, "4", BUTTON_TEXTSIZE);
        switch (z1fActiva) {
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
            z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                            OLIVE, WHITE, "4", BUTTON_TEXTSIZE);
            break;
        }
        z1InicioButtons[0].drawRectButton();
        z1InicioButtons[1].drawRectButton();
        z1InicioButtons[2].drawRectButton();
        z1InicioButtons[3].drawRectButton();

      } else if (z1InicioButtons[1].contains(p.x, p.y) &&
                 (z1fActiva != 2 && z1fSeleccionada != 2)) {
        z1fSeleccionada = 2;
        z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE, YELLOW,
                                        WHITE, "2", BUTTON_TEXTSIZE);

        z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, DARKGREY,
                                        WHITE, "1", BUTTON_TEXTSIZE);

        z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE, DARKGREY,
                                        WHITE, "3", BUTTON_TEXTSIZE);

        z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE, DARKGREY,
                                        WHITE, "4", BUTTON_TEXTSIZE);

        switch (z1fActiva) {
          case 1:
            z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, OLIVE,
                                            WHITE, "1", BUTTON_TEXTSIZE);
            break;
          case 3:
            z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE, OLIVE,
                                            WHITE, "3", BUTTON_TEXTSIZE);
            break;
          case 4:
            z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                            OLIVE, WHITE, "4", BUTTON_TEXTSIZE);
            break;
        }
        z1InicioButtons[0].drawRectButton();
        z1InicioButtons[1].drawRectButton();
        z1InicioButtons[2].drawRectButton();
        z1InicioButtons[3].drawRectButton();

      } else if (z1InicioButtons[2].contains(p.x, p.y) &&
                 (z1fActiva != 3 && z1fSeleccionada != 3)) {
        z1fSeleccionada = 3;
        z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE, YELLOW,
                                        WHITE, "3", BUTTON_TEXTSIZE);

        z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, DARKGREY,
                                        WHITE, "1", BUTTON_TEXTSIZE);

        z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE, DARKGREY,
                                        WHITE, "2", BUTTON_TEXTSIZE);

        z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE, DARKGREY,
                                        WHITE, "4", BUTTON_TEXTSIZE);

        switch (z1fActiva) {
          case 1:
            z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, OLIVE,
                                            WHITE, "1", BUTTON_TEXTSIZE);
            break;
          case 2:
            z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE, OLIVE,
                                            WHITE, "2", BUTTON_TEXTSIZE);
            break;
          case 4:
            z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                            OLIVE, WHITE, "4", BUTTON_TEXTSIZE);
            break;
        }
        z1InicioButtons[0].drawRectButton();
        z1InicioButtons[1].drawRectButton();
        z1InicioButtons[2].drawRectButton();
        z1InicioButtons[3].drawRectButton();

      } else if (z1InicioButtons[3].contains(p.x, p.y) &&
                 (z1fActiva != 4 && z1fSeleccionada != 4)) {
        z1fSeleccionada = 4;
        z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE, YELLOW,
                                        WHITE, "4", BUTTON_TEXTSIZE);

        z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, DARKGREY,
                                        WHITE, "1", BUTTON_TEXTSIZE);

        z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE, DARKGREY,
                                        WHITE, "2", BUTTON_TEXTSIZE);

        z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE, DARKGREY,
                                        WHITE, "3", BUTTON_TEXTSIZE);

        switch (z1fActiva) {
          case 1:
            z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, OLIVE,
                                            WHITE, "1", BUTTON_TEXTSIZE);
            break;
          case 2:
            z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE, OLIVE,
                                            WHITE, "2", BUTTON_TEXTSIZE);
            break;
          case 3:
            z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE, OLIVE,
                                            WHITE, "3", BUTTON_TEXTSIZE);
            break;
        }
        z1InicioButtons[0].drawRectButton();
        z1InicioButtons[1].drawRectButton();
        z1InicioButtons[2].drawRectButton();
        z1InicioButtons[3].drawRectButton();

      } else if (z1InicioButtons[4].contains(p.x, p.y) &&
                 (z1fActiva != z1fSeleccionada)) {
        z1fActiva = z1fSeleccionada;
        z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, DARKGREY,
                                        WHITE, "1", BUTTON_TEXTSIZE);

        z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE, DARKGREY,
                                        WHITE, "2", BUTTON_TEXTSIZE);

        z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE, DARKGREY,
                                        WHITE, "3", BUTTON_TEXTSIZE);

        z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE, DARKGREY,
                                        WHITE, "4", BUTTON_TEXTSIZE);
        switch (z1fActiva) {
          case 1:
            z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, OLIVE,
                                            WHITE, "1", BUTTON_TEXTSIZE);
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
            z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                            OLIVE, WHITE, "4", BUTTON_TEXTSIZE);
            break;
        }
        z1InicioButtons[0].drawRectButton();
        z1InicioButtons[1].drawRectButton();
        z1InicioButtons[2].drawRectButton();
        z1InicioButtons[3].drawRectButton();
      }
    } else if (currentScreen == "numKB") {
      numKBstrLength = strlen(numKBstr);

      tft.setTextSize(BUTTON_TEXTSIZE);

      if (numericKeyboardButtons[3].contains(p.x, p.y)) {  // del
        if (numKBstrLength != 0) {  // para no borrar de más
          numKBstr[numKBstrLength - 1] = '\0';

          // todo esto tapa el ultimo char de la str
          tft.fillRect(15 + ((numKBstrLength - 1) * 6 * BUTTON_TEXTSIZE),
                       60 - ((7 * BUTTON_TEXTSIZE) / 2), (6 * BUTTON_TEXTSIZE),
                       (7 * BUTTON_TEXTSIZE), BLACK);
          delay(150);
        }
      } else if (numKBstrLength <
                 numKBbufferSize) {  // proteccion contra buffer overflow
        if (numericKeyboardButtons[1].contains(p.x, p.y)) {
          numKBstr[numKBstrLength] = '0';

          tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                        60 - ((7 * BUTTON_TEXTSIZE) / 2));
          tft.print('0');
        } else if (numericKeyboardButtons[4].contains(p.x, p.y)) {
          numKBstr[numKBstrLength] = '1';

          tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                        60 - ((7 * BUTTON_TEXTSIZE) / 2));
          tft.print('1');
        } else if (numericKeyboardButtons[5].contains(p.x, p.y)) {
          numKBstr[numKBstrLength] = '2';

          tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                        60 - ((7 * BUTTON_TEXTSIZE) / 2));
          tft.print('2');
        } else if (numericKeyboardButtons[6].contains(p.x, p.y)) {
          numKBstr[numKBstrLength] = '3';

          tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                        60 - ((7 * BUTTON_TEXTSIZE) / 2));
          tft.print('3');
        } else if (numericKeyboardButtons[7].contains(p.x, p.y)) {
          numKBstr[numKBstrLength] = '4';

          tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                        60 - ((7 * BUTTON_TEXTSIZE) / 2));
          tft.print('4');
        } else if (numericKeyboardButtons[8].contains(p.x, p.y)) {
          numKBstr[numKBstrLength] = '5';

          tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                        60 - ((7 * BUTTON_TEXTSIZE) / 2));
          tft.print('5');
        } else if (numericKeyboardButtons[9].contains(p.x, p.y)) {
          numKBstr[numKBstrLength] = '6';

          tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                        60 - ((7 * BUTTON_TEXTSIZE) / 2));
          tft.print('6');
        } else if (numericKeyboardButtons[10].contains(p.x, p.y)) {
          numKBstr[numKBstrLength] = '7';

          tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                        60 - ((7 * BUTTON_TEXTSIZE) / 2));
          tft.print('7');
        } else if (numericKeyboardButtons[11].contains(p.x, p.y)) {
          numKBstr[numKBstrLength] = '8';

          tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                        60 - ((7 * BUTTON_TEXTSIZE) / 2));
          tft.print('8');
        } else if (numericKeyboardButtons[12].contains(p.x, p.y)) {
          numKBstr[numKBstrLength] = '9';

          tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                        60 - ((7 * BUTTON_TEXTSIZE) / 2));
          tft.print('9');
        }
        delay(150);
      }
      // aca van cada una de las pantallas en las que hay un teclado
      {
        if (numericKeyboardButtons[0].contains(p.x, p.y)) {
          if (numKBPrevScreen == "z1f1") {
            Z1F1Screen();
          } else if (numKBPrevScreen == "z1f2") {
            Z1F2Screen();
          } else if (numKBPrevScreen == "z1f3") {
            Z1F3Screen();
          } else if (numKBPrevScreen == "z1f4") {
            Z1F4Screen();
          }
        } else if (numericKeyboardButtons[13].contains(p.x, p.y)) {
          strcpy(numKBvarptr, numKBstr);
          if (numKBPrevScreen == "z1f1") {
            Z1F1Screen();
          } else if (numKBPrevScreen == "z1f2") {
            Z1F2Screen();
          } else if (numKBPrevScreen == "z1f3") {
            Z1F3Screen();
          } else if (numKBPrevScreen == "z1f4") {
            Z1F4Screen();
          }
        }
      }
    }

    //
  }
}