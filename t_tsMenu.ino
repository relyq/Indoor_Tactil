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
    Serial.print(F("Unmapped p: "));
    Serial.print(F("("));
    Serial.print(p.x);
    Serial.print(F(", "));
    Serial.print(p.y);
    Serial.print(F(", "));
    Serial.print(p.z);
    Serial.print(F(") "));

    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY - 60, tft.height(), 0);
    // if the screen is being touched show cursor position

    Serial.print(F("Mapped p: "));
    Serial.print(F("("));
    Serial.print(p.x);
    Serial.print(F(", "));
    Serial.print(p.y);
    Serial.print(F(", "));
    Serial.print(p.z);
    Serial.println(F(") "));

    if (currentScreen != 0 && (p.y < -1)) {
      HomeScreen();
    }

    switch (currentScreen) {
      case 0:
        if (homeButtons[0].contains(p.x, p.y)) {
          MenuScreen();
        }
        break;
      case 1:
        if (menuButtons[0].contains(p.x, p.y)) {
          Z1Screen();
        } else if (menuButtons[1].contains(p.x, p.y)) {
          AjustesScreen();
        } else if (menuButtons[2].contains(p.x, p.y)) {
          HomeScreen();
        }
        break;
      case 2:
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
        break;
      case 3:
        if (alarmasButtons[0].contains(p.x, p.y)) {
          AjustesScreen();
        }
        break;
      case 4:
        tft.setTextSize(3);
        tft.setTextColor(WHITE, BLACK);
        if (relojButtons[0].contains(p.x, p.y)) {
          AjustesScreen();
        } else if (relojButtons[1].contains(p.x, p.y)) {
          rtc.adjust(
              DateTime(relojYYYY, relojMM, relojDD, relojhh, relojmm, 0));
          now = rtc.now();
          AjustesScreen();
        } else if ((p.x > 39 && p.x < 83) && (p.y > 24 && p.y < 60)) {
          relojYYYY++;
          sprintf_P(buffer, STR_fdecimal, relojYYYY);
          tft.setCursor(28, 60);
          tft.print(buffer);
          if (relojMM == 2) {
            if ((relojYYYY % 4 == 0 && relojYYYY % 100 != 0) ||
                relojYYYY % 400 == 0) {
              if (relojDD > 29) {
                relojDD = 29;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
            } else {
              if (relojDD > 28) {
                relojDD = 28;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
            }
          }
        } else if ((p.x > 39 && p.x < 83) && (p.y > 81 && p.y < 114)) {
          if (relojYYYY > 1970) {
            relojYYYY--;
            sprintf_P(buffer, STR_fdecimal, relojYYYY);
            tft.setCursor(28, 60);
            tft.print(buffer);
          }
        } else if ((p.x > 114 && p.x < 158) && (p.y > 24 && p.y < 60)) {
          if (relojMM < 12) {
            relojMM++;
            sprintf_P(buffer, STR_f02decimal, relojMM);
            tft.setCursor(28 + 72 + 20, 60);
            tft.print(buffer);
          } else {
            relojMM = 1;
            sprintf_P(buffer, STR_f02decimal, relojMM);
            tft.setCursor(28 + 72 + 20, 60);
            tft.print(buffer);
          }
          switch (relojMM) {
            case 4:
            case 6:
            case 9:
            case 11:
              if (relojDD > 30) {
                relojDD = 30;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 2:
              if ((relojYYYY % 4 == 0 && relojYYYY % 100 != 0) ||
                  relojYYYY % 400 == 0) {
                if (relojDD > 29) {
                  relojDD = 29;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              } else {
                if (relojDD > 28) {
                  relojDD = 28;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              }
              break;
          }
        } else if ((p.x > 114 && p.x < 158) && (p.y > 81 && p.y < 114)) {
          if (relojMM > 1) {
            relojMM--;
            sprintf_P(buffer, STR_f02decimal, relojMM);
            tft.setCursor(28 + 72 + 20, 60);
            tft.print(buffer);
          } else {
            relojMM = 12;
            sprintf_P(buffer, STR_f02decimal, relojMM);
            tft.setCursor(28 + 72 + 20, 60);
            tft.print(buffer);
          }
          switch (relojMM) {
            case 4:
            case 6:
            case 9:
            case 11:
              if (relojDD > 30) {
                relojDD = 30;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 2:
              if ((relojYYYY % 4 == 0 && relojYYYY % 100 != 0) ||
                  relojYYYY % 400 == 0) {
                if (relojDD > 29) {
                  relojDD = 29;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              } else {
                if (relojDD > 28) {
                  relojDD = 28;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              }
              break;
          }
        } else if ((p.x > 170 && p.x < 214) && (p.y > 24 && p.y < 60)) {
          switch (relojMM) {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
              if (relojDD < 31) {
                relojDD++;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              } else {
                relojDD = 1;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 4:
            case 6:
            case 9:
            case 11:
              if (relojDD < 30) {
                relojDD++;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              } else {
                relojDD = 1;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 2:
              if ((relojYYYY % 4 == 0 && relojYYYY % 100 != 0) ||
                  relojYYYY % 400 == 0) {
                if (relojDD < 29) {
                  relojDD++;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                } else {
                  relojDD = 1;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              } else {
                if (relojDD < 28) {
                  relojDD++;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                } else {
                  relojDD = 1;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              }
              break;
          }
        } else if ((p.x > 170 && p.x < 214) && (p.y > 81 && p.y < 114)) {
          switch (relojMM) {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
              if (relojDD > 1) {
                relojDD--;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              } else {
                relojDD = 31;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 4:
            case 6:
            case 9:
            case 11:
              if (relojDD > 1) {
                relojDD--;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              } else {
                relojDD = 30;
                sprintf_P(buffer, STR_f02decimal, relojDD);
                tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                tft.print(buffer);
              }
              break;
            case 2:
              if ((relojYYYY % 4 == 0 && relojYYYY % 100 != 0) ||
                  relojYYYY % 400 == 0) {
                if (relojDD > 1) {
                  relojDD--;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                } else {
                  relojDD = 29;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              } else {
                if (relojDD > 1) {
                  relojDD--;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                } else {
                  relojDD = 28;
                  sprintf_P(buffer, STR_f02decimal, relojDD);
                  tft.setCursor(28 + 72 + 36 + 20 * 2, 60);
                  tft.print(buffer);
                }
              }
              break;
          }
        } else if ((p.x > 67 && p.x < 111) && (p.y > 116 && p.y < 150)) {
          if (relojhh < 23) {
            relojhh++;
            sprintf_P(buffer, STR_f02decimal, relojhh);
            tft.setCursor(74, 150);
            tft.print(buffer);
          } else {
            relojhh = 0;
            sprintf_P(buffer, STR_f02decimal, relojhh);
            tft.setCursor(74, 150);
            tft.print(buffer);
          }
        } else if ((p.x > 67 && p.x < 111) && (p.y > 171 && p.y < 207)) {
          if (relojhh > 0) {
            relojhh--;
            sprintf_P(buffer, STR_f02decimal, relojhh);
            tft.setCursor(74, 150);
            tft.print(buffer);
          } else {
            relojhh = 23;
            sprintf_P(buffer, STR_f02decimal, relojhh);
            tft.setCursor(74, 150);
            tft.print(buffer);
          }
        } else if ((p.x > 124 && p.x < 168) && (p.y > 116 && p.y < 150)) {
          if (relojmm < 59) {
            relojmm++;
            sprintf_P(buffer, STR_f02decimal, relojmm);
            tft.setCursor(74 + 36 + 20, 150);
            tft.print(buffer);
          } else {
            relojmm = 0;
            sprintf_P(buffer, STR_f02decimal, relojmm);
            tft.setCursor(74 + 36 + 20, 150);
            tft.print(buffer);
          }
        } else if ((p.x > 124 && p.x < 168) && (p.y > 171 && p.y < 207)) {
          if (relojmm > 0) {
            relojmm--;
            sprintf_P(buffer, STR_f02decimal, relojmm);
            tft.setCursor(74 + 36 + 20, 150);
            tft.print(buffer);
          } else {
            relojmm = 59;
            sprintf_P(buffer, STR_f02decimal, relojmm);
            tft.setCursor(74 + 36 + 20, 150);
            tft.print(buffer);
          }
        }
        delay(150);
        break;
      case 5:
        if (programasButtons[0].contains(p.x, p.y)) {
          AjustesScreen();
        } else if (programasButtons[1].contains(p.x, p.y)) {
          Programa1Screen();
        } else if (programasButtons[2].contains(p.x, p.y)) {
          Programa2Screen();
        } else if (programasButtons[3].contains(p.x, p.y)) {
          Programa3Screen();
        } else if (programasButtons[4].contains(p.x, p.y)) {
          Programa4Screen();
        }
        break;
      case 6:
        if (resetButtons[0].contains(p.x, p.y)) {
          AjustesScreen();
        } else if (resetButtons[1].contains(p.x, p.y)) {
          eeprom_hardReset();
        }
        break;
      case 7:
        if (programa1Buttons[0].contains(p.x, p.y)) {
          programasConfirmar = 0;
          ProgramasScreen();
        } else if (programa1Buttons[1].contains(p.x, p.y) &&
                   programasConfirmar != 1) {
          programa1Buttons[4].drawButton();
          programasConfirmar = 1;
        } else if (programa1Buttons[2].contains(p.x, p.y) &&
                   programasConfirmar != 2) {
          programa1Buttons[4].drawButton();
          programasConfirmar = 2;
        } else if (programa1Buttons[4].contains(p.x, p.y) &&
                   programasConfirmar) {
          switch (programasConfirmar) {
            case 1:  // cargar
              Programa p1;
              EEPROM.get(110, p1.f1);
              EEPROM.get(130, p1.f2);
              EEPROM.get(150, p1.f3);
              EEPROM.get(170, p1.f4);

              EEPROM.put(30, p1.f1);
              EEPROM.put(50, p1.f2);
              EEPROM.put(70, p1.f3);
              EEPROM.put(90, p1.f4);

              cargarFases();

              Serial.println(F("Programa 1 cargado"));
              break;
            case 2:  // guardar
              EEPROM.put(110, f1);
              EEPROM.put(130, f2);
              EEPROM.put(150, f3);
              EEPROM.put(170, f4);

              Serial.println(F("Programa 1 guardado"));
              break;
          }
          programasConfirmar = 0;
          tft.fillRect(5, 170, 230, 40, BLACK);
        }
        break;
      case 8:
        if (programa2Buttons[0].contains(p.x, p.y)) {
          programasConfirmar = 0;
          ProgramasScreen();
        } else if (programa2Buttons[1].contains(p.x, p.y) &&
                   programasConfirmar != 1) {
          programa2Buttons[4].drawButton();
          programasConfirmar = 1;
        } else if (programa2Buttons[2].contains(p.x, p.y) &&
                   programasConfirmar != 2) {
          programa2Buttons[4].drawButton();
          programasConfirmar = 2;
        } else if (programa2Buttons[4].contains(p.x, p.y) &&
                   programasConfirmar) {
          switch (programasConfirmar) {
            case 1:  // cargar
              Programa p2;
              EEPROM.get(210, p2.f1);
              EEPROM.get(230, p2.f2);
              EEPROM.get(250, p2.f3);
              EEPROM.get(270, p2.f4);

              EEPROM.put(30, p2.f1);
              EEPROM.put(50, p2.f2);
              EEPROM.put(70, p2.f3);
              EEPROM.put(90, p2.f4);

              cargarFases();

              Serial.println(F("Programa 2 cargado"));
              break;
            case 2:  // guardar
              EEPROM.put(210, f1);
              EEPROM.put(230, f2);
              EEPROM.put(250, f3);
              EEPROM.put(270, f4);

              Serial.println(F("Programa 2 guardado"));
              break;
          }
          programasConfirmar = 0;
          tft.fillRect(5, 170, 230, 40, BLACK);
        }
        break;
      case 9:
        if (programa3Buttons[0].contains(p.x, p.y)) {
          programasConfirmar = 0;
          ProgramasScreen();
        } else if (programa3Buttons[1].contains(p.x, p.y) &&
                   programasConfirmar != 1) {
          programa3Buttons[4].drawButton();
          programasConfirmar = 1;
        } else if (programa3Buttons[2].contains(p.x, p.y) &&
                   programasConfirmar != 2) {
          programa3Buttons[4].drawButton();
          programasConfirmar = 2;
        } else if (programa3Buttons[4].contains(p.x, p.y) &&
                   programasConfirmar) {
          switch (programasConfirmar) {
            case 1:  // cargar
              Programa p3;
              EEPROM.get(310, p3.f1);
              EEPROM.get(330, p3.f2);
              EEPROM.get(350, p3.f3);
              EEPROM.get(370, p3.f4);

              EEPROM.put(30, p3.f1);
              EEPROM.put(50, p3.f2);
              EEPROM.put(70, p3.f3);
              EEPROM.put(90, p3.f4);

              cargarFases();

              Serial.println(F("Programa 3 cargado"));
              break;
            case 2:  // guardar
              EEPROM.put(310, f1);
              EEPROM.put(330, f2);
              EEPROM.put(350, f3);
              EEPROM.put(370, f4);

              Serial.println(F("Programa 3 guardado"));
              break;
          }
          programasConfirmar = 0;
          tft.fillRect(5, 170, 230, 40, BLACK);
        }
        break;
      case 10:
        if (programa4Buttons[0].contains(p.x, p.y)) {
          programasConfirmar = 0;
          ProgramasScreen();
        } else if (programa4Buttons[1].contains(p.x, p.y) &&
                   programasConfirmar != 1) {
          programa4Buttons[4].drawButton();
          programasConfirmar = 1;
        } else if (programa4Buttons[2].contains(p.x, p.y) &&
                   programasConfirmar != 2) {
          programa4Buttons[4].drawButton();
          programasConfirmar = 2;
        } else if (programa4Buttons[4].contains(p.x, p.y) &&
                   programasConfirmar) {
          switch (programasConfirmar) {
            case 1:  // cargar
              Programa p4;
              EEPROM.get(410, p4.f1);
              EEPROM.get(430, p4.f2);
              EEPROM.get(450, p4.f3);
              EEPROM.get(470, p4.f4);

              EEPROM.put(30, p4.f1);
              EEPROM.put(50, p4.f2);
              EEPROM.put(70, p4.f3);
              EEPROM.put(90, p4.f4);

              cargarFases();

              Serial.println(F("Programa 4 cargado"));
              break;
            case 2:  // guardar
              EEPROM.put(410, f1);
              EEPROM.put(430, f2);
              EEPROM.put(450, f3);
              EEPROM.put(470, f4);

              Serial.println(F("Programa 4 guardado"));
              break;
          }
          programasConfirmar = 0;
          tft.fillRect(5, 170, 230, 40, BLACK);
        }
        break;
      case 30:
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
        break;
      case 31:
        if (z1ControlButtons[2].contains(p.x, p.y)) {
          z1TerminarConfirmar = 0;
          Z1Screen();
        } else if (z1ControlButtons[0].contains(p.x, p.y)) {
          z1TerminarConfirmar = 0;
          Z1InicioScreen();
        } else if (z1ControlButtons[1].contains(p.x, p.y) &&
                   z1TerminarConfirmar == 0) {
          z1ControlButtons[3].drawButton();
          z1TerminarConfirmar = 1;
        } else if ((p.x > 170 && p.x < 235) && (p.y > 125 && p.y < 165)) {
          z1TerminarConfirmar = 0;
          NumericKeyboardScreen(&ciclos, 2, "Ciclos");
        } else if (z1TerminarConfirmar == 1 &&
                   z1ControlButtons[3].contains(p.x, p.y)) {
          for (uint8_t i = 10; i < 29; i++) {
            EEPROM.update(i, 0x00);
          }
          EEPROM.update(22, 0x01);
          Serial.println(F("eeprom 10 to 29 cleared to 0x00\n"));
          eeprom_read();
          z1fActiva = 0;

          z1TerminarConfirmar = 0;
          tft.fillRect(5, 170, 230, 40, BLACK);
        }
        break;
      case 32:
        if (z1InicioButtons[5].contains(p.x, p.y)) {
          Z1ControlScreen();
        } else if (z1InicioButtons[0].contains(p.x, p.y) &&
                   (z1fActiva != 1 && z1fSeleccionada != 1)) {
          z1fSeleccionada = 1;
          z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, YELLOW,
                                          WHITE, PSTR("1"), BUTTON_TEXTSIZE);

          z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE,
                                          DARKGREY, WHITE, PSTR("2"),
                                          BUTTON_TEXTSIZE);

          z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE,
                                          DARKGREY, WHITE, PSTR("3"),
                                          BUTTON_TEXTSIZE);

          z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                          DARKGREY, WHITE, PSTR("4"),
                                          BUTTON_TEXTSIZE);
          switch (z1fActiva) {
            break;
            case 2:
              z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE,
                                              OLIVE, WHITE, PSTR("2"),
                                              BUTTON_TEXTSIZE);
              break;
            case 3:
              z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE,
                                              OLIVE, WHITE, PSTR("3"),
                                              BUTTON_TEXTSIZE);
              break;
            case 4:
              z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                              OLIVE, WHITE, PSTR("4"),
                                              BUTTON_TEXTSIZE);
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
                                          WHITE, PSTR("2"), BUTTON_TEXTSIZE);

          z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, DARKGREY,
                                          WHITE, PSTR("1"), BUTTON_TEXTSIZE);

          z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE,
                                          DARKGREY, WHITE, PSTR("3"),
                                          BUTTON_TEXTSIZE);

          z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                          DARKGREY, WHITE, PSTR("4"),
                                          BUTTON_TEXTSIZE);

          switch (z1fActiva) {
            case 1:
              z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE,
                                              OLIVE, WHITE, PSTR("1"),
                                              BUTTON_TEXTSIZE);
              break;
            case 3:
              z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE,
                                              OLIVE, WHITE, PSTR("3"),
                                              BUTTON_TEXTSIZE);
              break;
            case 4:
              z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                              OLIVE, WHITE, PSTR("4"),
                                              BUTTON_TEXTSIZE);
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
                                          WHITE, PSTR("3"), BUTTON_TEXTSIZE);

          z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, DARKGREY,
                                          WHITE, PSTR("1"), BUTTON_TEXTSIZE);

          z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE,
                                          DARKGREY, WHITE, PSTR("2"),
                                          BUTTON_TEXTSIZE);

          z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                          DARKGREY, WHITE, PSTR("4"),
                                          BUTTON_TEXTSIZE);

          switch (z1fActiva) {
            case 1:
              z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE,
                                              OLIVE, WHITE, PSTR("1"),
                                              BUTTON_TEXTSIZE);
              break;
            case 2:
              z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE,
                                              OLIVE, WHITE, PSTR("2"),
                                              BUTTON_TEXTSIZE);
              break;
            case 4:
              z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                              OLIVE, WHITE, PSTR("4"),
                                              BUTTON_TEXTSIZE);
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
                                          WHITE, PSTR("4"), BUTTON_TEXTSIZE);

          z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE, DARKGREY,
                                          WHITE, PSTR("1"), BUTTON_TEXTSIZE);

          z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE,
                                          DARKGREY, WHITE, PSTR("2"),
                                          BUTTON_TEXTSIZE);

          z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE,
                                          DARKGREY, WHITE, PSTR("3"),
                                          BUTTON_TEXTSIZE);

          switch (z1fActiva) {
            case 1:
              z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE,
                                              OLIVE, WHITE, PSTR("1"),
                                              BUTTON_TEXTSIZE);
              break;
            case 2:
              z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE,
                                              OLIVE, WHITE, PSTR("2"),
                                              BUTTON_TEXTSIZE);
              break;
            case 3:
              z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE,
                                              OLIVE, WHITE, PSTR("3"),
                                              BUTTON_TEXTSIZE);
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
                                          WHITE, PSTR("1"), BUTTON_TEXTSIZE);

          z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE,
                                          DARKGREY, WHITE, PSTR("2"),
                                          BUTTON_TEXTSIZE);

          z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE,
                                          DARKGREY, WHITE, PSTR("3"),
                                          BUTTON_TEXTSIZE);

          z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                          DARKGREY, WHITE, PSTR("4"),
                                          BUTTON_TEXTSIZE);
          switch (z1fActiva) {
            case 1:
              z1InicioButtons[0].initButtonUL(&tft, 35, 55, 50, 50, WHITE,
                                              OLIVE, WHITE, PSTR("1"),
                                              BUTTON_TEXTSIZE);
              break;
            case 2:
              z1InicioButtons[1].initButtonUL(&tft, 155, 55, 50, 50, WHITE,
                                              OLIVE, WHITE, PSTR("2"),
                                              BUTTON_TEXTSIZE);
              break;
            case 3:
              z1InicioButtons[2].initButtonUL(&tft, 35, 130, 50, 50, WHITE,
                                              OLIVE, WHITE, PSTR("3"),
                                              BUTTON_TEXTSIZE);
              break;
            case 4:
              z1InicioButtons[3].initButtonUL(&tft, 155, 130, 50, 50, WHITE,
                                              OLIVE, WHITE, PSTR("4"),
                                              BUTTON_TEXTSIZE);
              break;
          }
          z1InicioButtons[0].drawRectButton();
          z1InicioButtons[1].drawRectButton();
          z1InicioButtons[2].drawRectButton();
          z1InicioButtons[3].drawRectButton();
        }
        break;
      case 33:
        if (z1f1Buttons[4].contains(p.x, p.y)) {
          Z1Screen();
        } else if (z1f1Buttons[0].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f1.dias, 3, "Dias");
        } else if (z1f1Buttons[1].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f1.hLuz, 2, "Horas luz");
        } else if (z1f1Buttons[6].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f1.templ, 2, "Temp baja");
        } else if (z1f1Buttons[5].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f1.temph, 2, "Temp alta");
        } else if (z1f1Buttons[8].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f1.riegol, 2, "Riego bajo");
        } else if (z1f1Buttons[7].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f1.riegoh, 2, "Riego alto");
        } else if (z1f1Buttons[11].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f1.huml, 2, "Hum baja");
        } else if (z1f1Buttons[10].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f1.humh, 2, "Hum alta");
        }

        break;
      case 34:
        if (z1f2Buttons[4].contains(p.x, p.y)) {
          Z1Screen();
        } else if (z1f2Buttons[0].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f2.dias, 3, "Dias");
        } else if (z1f2Buttons[1].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f2.hLuz, 2, "Horas luz");
        } else if (z1f2Buttons[6].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f2.templ, 2, "Temp baja");
        } else if (z1f2Buttons[5].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f2.temph, 2, "Temp alta");
        } else if (z1f2Buttons[8].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f2.riegol, 2, "Riego bajo");
        } else if (z1f2Buttons[7].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f2.riegoh, 2, "Riego alto");
        } else if (z1f2Buttons[11].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f2.huml, 2, "Hum baja");
        } else if (z1f2Buttons[10].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f2.humh, 2, "Hum alta");
        }
        break;
      case 35:
        if (z1f3Buttons[4].contains(p.x, p.y)) {
          Z1Screen();
        } else if (z1f3Buttons[0].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f3.dias, 3, "Dias");
        } else if (z1f3Buttons[1].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f3.hLuz, 2, "Horas luz");
        } else if (z1f3Buttons[6].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f3.templ, 2, "Temp baja");
        } else if (z1f3Buttons[5].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f3.temph, 2, "Temp alta");
        } else if (z1f3Buttons[8].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f3.riegol, 2, "Riego bajo");
        } else if (z1f3Buttons[7].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f3.riegoh, 2, "Riego alto");
        } else if (z1f3Buttons[11].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f3.huml, 2, "Hum baja");
        } else if (z1f3Buttons[10].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f3.humh, 2, "Hum alta");
        }
        break;
      case 36:
        if (z1f4Buttons[4].contains(p.x, p.y)) {
          Z1Screen();
        } else if (z1f4Buttons[0].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f4.dias, 3, "Dias");
        } else if (z1f4Buttons[1].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f4.hLuz, 2, "Horas luz");
        } else if (z1f4Buttons[6].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f4.templ, 2, "Temp baja");
        } else if (z1f4Buttons[5].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f4.temph, 2, "Temp alta");
        } else if (z1f4Buttons[8].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f4.riegol, 2, "Riego bajo");
        } else if (z1f4Buttons[7].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f4.riegoh, 2, "Riego alto");
        } else if (z1f4Buttons[11].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f4.huml, 2, "Hum baja");
        } else if (z1f4Buttons[10].contains(p.x, p.y)) {
          NumericKeyboardScreen(&f4.humh, 2, "Hum alta");
        }

        break;
      case 255:
        numKBstrLength = strlen(numKBstr);

        tft.setTextSize(BUTTON_TEXTSIZE);

        if (numericKeyboardButtons[3].contains(p.x, p.y)) {  // del
          if (numKBstrLength != 0) {  // para no borrar de más
            numKBstr[numKBstrLength - 1] = '\0';

            // todo esto tapa el ultimo char de la str
            tft.fillRect(15 + ((numKBstrLength - 1) * 6 * BUTTON_TEXTSIZE),
                         60 - ((7 * BUTTON_TEXTSIZE) / 2),
                         (6 * BUTTON_TEXTSIZE), (7 * BUTTON_TEXTSIZE), BLACK);
            delay(150);
          }
        } else if (numKBstrLength <
                   numKBbufferSize) {  // proteccion contra buffer overflow
          if (numericKeyboardButtons[1].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '0';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("0"));
          } else if (numericKeyboardButtons[4].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '1';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("1"));
          } else if (numericKeyboardButtons[5].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '2';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("2"));
          } else if (numericKeyboardButtons[6].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '3';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("3"));
          } else if (numericKeyboardButtons[7].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '4';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("4"));
          } else if (numericKeyboardButtons[8].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '5';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("5"));
          } else if (numericKeyboardButtons[9].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '6';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("6"));
          } else if (numericKeyboardButtons[10].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '7';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("7"));
          } else if (numericKeyboardButtons[11].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '8';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("8"));
          } else if (numericKeyboardButtons[12].contains(p.x, p.y)) {
            numKBstr[numKBstrLength] = '9';

            tft.setCursor(15 + ((numKBstrLength)*6 * BUTTON_TEXTSIZE),
                          60 - ((7 * BUTTON_TEXTSIZE) / 2));
            tft.print(F("9"));
          }
          delay(150);
        }
        // aca van cada una de las pantallas en las que hay un teclado
        if (numericKeyboardButtons[0].contains(p.x, p.y)) {
          if (numKBPrevScreen == 33) {
            Z1F1Screen();
          } else if (numKBPrevScreen == 34) {
            Z1F2Screen();
          } else if (numKBPrevScreen == 35) {
            Z1F3Screen();
          } else if (numKBPrevScreen == 36) {
            Z1F4Screen();
          } else if (numKBPrevScreen == 31) {
            Z1ControlScreen();
          }
        } else if (numericKeyboardButtons[13].contains(p.x, p.y)) {
          if (numKBvarptr8b != NULL)
            *numKBvarptr8b = atoi(numKBstr);
          else if (numKBvarptr16b != NULL)
            *numKBvarptr16b = atoi(numKBstr);
          if (numKBPrevScreen == 33) {
            Z1F1Screen();
          } else if (numKBPrevScreen == 34) {
            Z1F2Screen();
          } else if (numKBPrevScreen == 35) {
            Z1F3Screen();
          } else if (numKBPrevScreen == 36) {
            Z1F4Screen();
          } else if (numKBPrevScreen == 31) {
            Z1ControlScreen();
          }
        }
        break;
    }
  }
}