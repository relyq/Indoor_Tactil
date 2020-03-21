void NumericKeyboardScreen(TSPoint p) {
  if (!numericScreenInit) {
    tft.fillScreen(BLACK);

    // create buttons
    for (uint8_t row = 0; row < 5; row++) {
      for (uint8_t col = 0; col < 3; col++) {
        buttons[col + row * 3].initButton(
            &tft, BUTTON_X + col * (BUTTON_W + BUTTON_SPACING_X),
            BUTTON_Y +
                row * (BUTTON_H + BUTTON_SPACING_Y),  // x, y, w, h,
                                                      // outline, fill, text
            BUTTON_W, BUTTON_H, ILI9341_WHITE, buttoncolors[col + row * 3],
            ILI9341_WHITE, buttonlabels[col + row * 3], BUTTON_TEXTSIZE1);
        buttons[col + row * 3].drawButton();
      }
    }

    // create 'text field'
    tft.drawRect(TEXT_X, TEXT_Y, TEXT_W, TEXT_H, ILI9341_WHITE);
    numericScreenInit = true;
  }

  // go thru all the buttons, checking if they were pressed
  for (uint8_t b = 0; b < 15; b++) {
    if (buttons[b].contains(p.x, p.y)) {
      // Serial.print("Pressing: "); Serial.println(b);
      buttons[b].press(true);  // tell the button it is pressed
    } else {
      buttons[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // now we can ask the buttons if their state has changed
  for (uint8_t b = 0; b < 15; b++) {
    if (buttons[b].justReleased()) {
      // Serial.print("Released: "); Serial.println(b);
      buttons[b].drawButton();  // draw normal
    }

    if (buttons[b].justPressed()) {
      buttons[b].drawButton(true);  // draw invert!

      // if a numberpad button, append the relevant # to the textfield
      if (b >= 3) {
        if (textfield_i < TEXT_LEN) {
          textfield[textfield_i] = buttonlabels[b][0];
          textfield_i++;
          textfield[textfield_i] = 0;  // zero terminate

          // fona.playDTMF(buttonlabels[b][0]);
        }
      }

      // clr button! delete char
      if (b == 1) {
        textfield[textfield_i] = 0;
        if (textfield > 0) {
          textfield_i--;
          textfield[textfield_i] = ' ';
        }
      }

      // update the current text field
      Serial.println(textfield);
      tft.setCursor(TEXT_X + 2, TEXT_Y + 10);
      tft.setTextColor(TEXT_TCOLOR, ILI9341_BLACK);
      tft.setTextSize(TEXT_TSIZE);
      tft.print(textfield);

      // Esc. Button
      if (b == 2) {
        // status(F("Hanging up"));
        // fona.hangUp();
        if (screens[22]) {
          // temperatureSP = textfield;
          screens[2] = true;
          settingsScreenInit = false;
          screens[22] = false;
        }

        if (screens[23]) {
          // temperatureSP = textfield;
          screens[2] = true;
          settingsScreenInit = false;
          screens[23] = false;
        }

        for (int i = 0; i <= 12; i++) {
          textfield[i] = ' ';
        }
        textfield_i = 0;
      }
      // Enter Button
      if (b == 0) {
        // status(F("Calling"));
        // Serial.print("Calling "); Serial.print(textfield);
        if (screens[22]) {
          temperatureSP = textfield;
          screens[2] = true;
          settingsScreenInit = false;
          screens[22] = false;
        }

        if (screens[23]) {
          humiditySP = textfield;
          screens[2] = true;
          settingsScreenInit = false;
          screens[23] = false;
        }

        for (int i = 0; i <= 12; i++) {
          textfield[i] = ' ';
        }
        textfield_i = 0;

        // fona.callPhone(textfield);
        // temperatureSP,screens[2],settingsScreenInit
      }

      delay(50);  // UI debouncing
    }
  }
}

/////  Pantalla Principal  //

void HomeScreen(String temperature, String humidity, String tempSP,
                String humSP, bool hOn, bool fOn) {
  if (!homeScreenInit) {
    tft.fillScreen(BLACK);

    /////  Boton del Menu  ///////////
    settingsButton.initButton(&tft, 120,
                              30,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_YELLOW, ILI9341_DARKGREEN,
                              ILI9341_YELLOW, "Menu", BUTTON_TEXTSIZE);
    settingsButton.drawButton();
    /////  Boton del Menu  ///////////

    tft.setCursor(10, 65);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE, BLACK);
    tft.print("Iluminacion   ");

    tft.setCursor(10, 90);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE, BLACK);
    tft.print("Ventilacion   ");

    tft.setCursor(10, 115);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE, BLACK);
    tft.print("Vaporizacion   ");

    tft.setCursor(10, 140);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE, BLACK);
    tft.print("Riego   ");

    tft.drawCircle(180, 125, 10, GREEN);
    tft.fillCircle(180, 125, 10, GREEN);

    tft.drawCircle(180, 150, 10, RED);
    tft.fillCircle(180, 150, 10, RED);

    tft.drawCircle(215, 125, 10, GREEN);
    tft.fillCircle(215, 125, 10, GREEN);

    tft.drawCircle(215, 150, 10, RED);
    tft.fillCircle(215, 150, 10, RED);

    tft.setCursor(10, 175);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Temp. ");
    tft.setTextColor(ILI9341_ORANGE);
    tft.print("(SP: " + tempSP + " C)");

    tft.setCursor(10, 250);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Humidity ");
    tft.setTextColor(ILI9341_BLUE);
    tft.print("(SP: " + humSP + "%)");

    homeScreenInit = true;
  }

  if (fOn) {
    tft.drawCircle(215, 100, 10, GREEN);
    tft.fillCircle(215, 100, 10, GREEN);
  } else {
    tft.drawCircle(215, 100, 10, RED);
    tft.fillCircle(215, 100, 10, RED);
  }

  if (hOn) {
    tft.drawCircle(215, 75, 10, GREEN);
    tft.fillCircle(215, 75, 10, GREEN);
  } else {
    tft.drawCircle(215, 75, 10, RED);
    tft.fillCircle(215, 75, 10, RED);
  }

  if (hOn) {
    tft.drawCircle(180, 75, 10, GREEN);
    tft.fillCircle(180, 75, 10, GREEN);
  } else {
    tft.drawCircle(180, 75, 10, RED);
    tft.fillCircle(180, 75, 10, RED);
  }

  if (fOn) {
    tft.drawCircle(180, 100, 10, GREEN);
    tft.fillCircle(180, 100, 10, GREEN);
  } else {
    tft.drawCircle(180, 100, 10, RED);
    tft.fillCircle(180, 100, 10, RED);
  }

  tft.setCursor(10, 200);
  tft.setTextSize(5);
  tft.setTextColor(ILI9341_WHITE, BLACK);
  tft.print(temperature + " C");

  tft.setCursor(10, 275);
  tft.setTextSize(5);
  tft.setTextColor(ILI9341_WHITE, BLACK);
  tft.print(humidity + " %");

  delay(50);  // debouncing
}

/////  Pantalla Menu  /////
void MenuScreen(String temp, String hum) {
  if (!menuScreenInit) {
    tft.fillScreen(BLACK);
    tft.setCursor(35, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Menu Principal");

    /////  Boton Primera Opcion del Menu  /////
    opcion1Button.initButton(&tft, 120,
                             55,  // x, y, w, h, outline, fill, text
                             230, 40, ILI9341_WHITE, ILI9341_ORANGE,
                             ILI9341_YELLOW, "Zona 1", BUTTON_TEXTSIZE);
    opcion1Button.drawButton();
    /////  Boton Primera Opcion del Menu  /////

    /////  Boton Segunda Opcion del Menu  /////
    opcion2Button.initButton(&tft, 120,
                             100,  // x, y, w, h, outline, fill, text
                             230, 40, ILI9341_WHITE, ILI9341_ORANGE,
                             ILI9341_YELLOW, "Zona 2", BUTTON_TEXTSIZE);
    opcion2Button.drawButton();
    /////  Boton Segunda Opcion del Menu  /////

    /////  Boton Cuarta Opcion del Menu  //////
    opcion4Button.initButton(&tft, 120,
                             190,  // x, y, w, h, outline, fill, text
                             230, 40, ILI9341_WHITE, ILI9341_ORANGE,
                             ILI9341_YELLOW, "Opciones", BUTTON_TEXTSIZE);
    opcion4Button.drawButton();
    /////  Boton Cuarta Opcion del Menu  //////

    /////  Boton Salir del Menu  /////
    backHomeButton.initButton(&tft, 120,
                              300,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_LIGHTGREY,
                              ILI9341_WHITE, "Salir", BUTTON_TEXTSIZE);
    backHomeButton.drawButton();
    /////  Boton Salir del Menu  /////

    menuScreenInit = true;
  }
  delay(50);  // debouncing
}

/////  Pantalla Opcion 1 - zona 1  /////
void Opcion1Screen(String temp, String hum) {
  if (!opcion1ScreenInit) {
    tft.fillScreen(BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Programa Zona 1");

    /////  Boton Verano  /////////////
    veranoz2Button.initButton(&tft, 120,
                              55,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_ORANGE,
                              ILI9341_YELLOW, "Verano", BUTTON_TEXTSIZE);
    veranoz2Button.drawButton();
    /////  Boton Quinta Opcion del Menu  //////

    /////  Boton Otonio  /////////////
    otonioz2Button.initButton(&tft, 120,
                              100,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_ORANGE,
                              ILI9341_YELLOW, "Otonio", BUTTON_TEXTSIZE);
    otonioz2Button.drawButton();
    /////  Boton Quinta Opcion del Menu  //////

    /////  Boton Invierno  ///////////
    inviernoz2Button.initButton(&tft, 120,
                                145,  // x, y, w, h, outline, fill, text
                                230, 40, ILI9341_WHITE, ILI9341_ORANGE,
                                ILI9341_YELLOW, "Invierno", BUTTON_TEXTSIZE);
    inviernoz2Button.drawButton();
    /////  Boton Quinta Opcion del Menu  //////

    /////  Boton Primavera  //////////
    primaveraz2Button.initButton(&tft, 120,
                                 190,  // x, y, w, h, outline, fill, text
                                 230, 40, ILI9341_WHITE, ILI9341_ORANGE,
                                 ILI9341_YELLOW, "Primavera", BUTTON_TEXTSIZE);
    primaveraz2Button.drawButton();
    /////  Boton Quinta Opcion del Menu  //////

    /////  Boton Tercera Opcion del Menu  /////
    controlz2Button.initButton(&tft, 120,
                               245,  // x, y, w, h, outline, fill, text
                               230, 40, ILI9341_WHITE, ILI9341_ORANGE,
                               ILI9341_YELLOW, "Control", BUTTON_TEXTSIZE);
    controlz2Button.drawButton();
    /////  Boton Tercera Opcion del Menu  /////

    /////  Boton Salir del Menu  /////
    backMenuButton.initButton(&tft, 120,
                              300,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_LIGHTGREY,
                              ILI9341_WHITE, "Volver", BUTTON_TEXTSIZE);
    backMenuButton.drawButton();
    /////  Boton Salir del Menu  /////

    opcion1ScreenInit = true;
  }
  delay(50);  // debouncing
}

/////  Pantalla Opcion 2 - zona 2  /////
void Opcion2Screen(String temp, String hum) {
  if (!opcion2ScreenInit) {
    tft.fillScreen(BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Programa Zona 2");

    /////  Boton Verano  /////////////
    veranoz2Button.initButton(&tft, 120,
                              55,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_ORANGE,
                              ILI9341_YELLOW, "Verano", BUTTON_TEXTSIZE);
    veranoz2Button.drawButton();
    /////  Boton Quinta Opcion del Menu  //////

    /////  Boton Otonio  /////////////
    otonioz2Button.initButton(&tft, 120,
                              100,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_ORANGE,
                              ILI9341_YELLOW, "Otonio", BUTTON_TEXTSIZE);
    otonioz2Button.drawButton();
    /////  Boton Quinta Opcion del Menu  //////

    /////  Boton Invierno  ///////////
    inviernoz2Button.initButton(&tft, 120,
                                145,  // x, y, w, h, outline, fill, text
                                230, 40, ILI9341_WHITE, ILI9341_ORANGE,
                                ILI9341_YELLOW, "Invierno", BUTTON_TEXTSIZE);
    inviernoz2Button.drawButton();
    /////  Boton Quinta Opcion del Menu  //////

    /////  Boton Primavera  //////////
    primaveraz2Button.initButton(&tft, 120,
                                 190,  // x, y, w, h, outline, fill, text
                                 230, 40, ILI9341_WHITE, ILI9341_ORANGE,
                                 ILI9341_YELLOW, "Primavera", BUTTON_TEXTSIZE);
    primaveraz2Button.drawButton();
    /////  Boton Quinta Opcion del Menu  //////

    /////  Boton Tercera Opcion del Menu  /////
    controlz2Button.initButton(&tft, 120,
                               245,  // x, y, w, h, outline, fill, text
                               230, 40, ILI9341_WHITE, ILI9341_ORANGE,
                               ILI9341_YELLOW, "Control", BUTTON_TEXTSIZE);
    controlz2Button.drawButton();
    /////  Boton Tercera Opcion del Menu  /////

    /////  Boton Salir del Menu  /////
    backMenuButton.initButton(&tft, 120,
                              300,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_LIGHTGREY,
                              ILI9341_WHITE, "Volver", BUTTON_TEXTSIZE);
    backMenuButton.drawButton();
    /////  Boton Salir del Menu  /////

    opcion2ScreenInit = true;
  }
  delay(50);  // debouncing
}

/*/////////////////////////////////////////////////////////////////////////////
/////  Pantalla Opcion 3
//////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void Opcion3Screen(String temp, String hum){
  if(!opcion3ScreenInit){
    tft.fillScreen(BLACK);
    tft.setCursor(40, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Opcion 3");
///////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////  Boton Salir del Menu
///////////////////////////////////////////////////////////////////
    backMenuButton.initButton(&tft, 120,300,    // x, y, w, h, outline, fill,
text 230,40, ILI9341_WHITE, ILI9341_LIGHTGREY, ILI9341_WHITE, "Volver",
BUTTON_TEXTSIZE); backMenuButton.drawButton();
    /////  Boton Salir del Menu
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    opcion3ScreenInit = true;
    }

  }

/////////////////////////////////////////////////////////////////////////////
/////  Fin Pantalla Opcion 3
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
*/

/////  Pantalla Opcion 4 - opciones  //////
void Opcion4Screen(String temp, String hum) {
  if (!opcion4ScreenInit) {
    tft.fillScreen(BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Opciones");

    /////  Boton Alarmas  /////////////
    alarmasButton.initButton(&tft, 120,
                             55,  // x, y, w, h, outline, fill, text
                             230, 40, ILI9341_WHITE, ILI9341_ORANGE,
                             ILI9341_YELLOW, "Alarmas", BUTTON_TEXTSIZE);
    alarmasButton.drawButton();
    /////  Boton Quinta Opcion del Menu  //////

    /////  Boton Wifi  /////////////
    wifiButton.initButton(&tft, 120,
                          100,  // x, y, w, h, outline, fill, text
                          230, 40, ILI9341_WHITE, ILI9341_ORANGE,
                          ILI9341_YELLOW, "WiFi", BUTTON_TEXTSIZE);
    wifiButton.drawButton();
    /////  Boton Quinta Opcion del Menu  //////

    /////  Boton iot  ///////////
    iotButton.initButton(&tft, 120, 145,  // x, y, w, h, outline, fill, text
                         230, 40, ILI9341_WHITE, ILI9341_ORANGE, ILI9341_YELLOW,
                         "Iot", BUTTON_TEXTSIZE);
    iotButton.drawButton();
    /////  Boton Quinta Opcion del Menu  //////

    /////  Boton Salir del Menu  /////
    backMenuButton.initButton(&tft, 120,
                              300,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_LIGHTGREY,
                              ILI9341_WHITE, "Volver", BUTTON_TEXTSIZE);
    backMenuButton.drawButton();
    /////  Boton Salir del Menu  /////

    opcion4ScreenInit = true;
  }
  delay(50);  // debouncing
}

/*
/////////////////////////////////////////////////////////////////////////////
/////  Pantalla Opcion 5
//////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void Opcion5Screen(String temp, String hum){
  if(!opcion5ScreenInit){
    tft.fillScreen(BLACK);
    tft.setCursor(40, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Opcion 5");
///////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////  Boton Salir del Menu
///////////////////////////////////////////////////////////////////
    backMenuButton.initButton(&tft, 120,300,    // x, y, w, h, outline, fill,
text 230,40, ILI9341_WHITE, ILI9341_LIGHTGREY, ILI9341_WHITE, "Volver",
BUTTON_TEXTSIZE); backMenuButton.drawButton();
    /////  Boton Salir del Menu
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    opcion5ScreenInit = true;
    }

  }

/////////////////////////////////////////////////////////////////////////////
/////  Fin Pantalla Opcion 5
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
*/

/////  Pantalla Verano Z1  /////
void Veranoz1Screen(String temp, String hum) {
  if (!veranoz1ScreenInit) {
    tft.fillScreen(BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Ciclo Verano Z1");

    /////  Boton Salir del Menu  /////
    backMenuButton.initButton(&tft, 120,
                              300,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_LIGHTGREY,
                              ILI9341_WHITE, "Volver", BUTTON_TEXTSIZE);
    backMenuButton.drawButton();
    /////  Boton Salir del Menu  /////

    veranoz1ScreenInit = true;
  }
  delay(50);  // debouncing
}

/////  Pantalla Otoño Z1  //////
void Otonioz1Screen(String temp, String hum) {
  if (!otonioz1ScreenInit) {
    tft.fillScreen(BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Ciclo Otonio Z1");

    /////  Boton Salir del Menu  /////
    backMenuButton.initButton(&tft, 120,
                              300,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_LIGHTGREY,
                              ILI9341_WHITE, "Volver", BUTTON_TEXTSIZE);
    backMenuButton.drawButton();
    /////  Boton Salir del Menu  /////

    otonioz1ScreenInit = true;
  }
  delay(50);  // debouncing
}

/////  Pantalla Invierno Z1  ///
void Inviernoz1Screen(String temp, String hum) {
  if (!inviernoz1ScreenInit) {
    tft.fillScreen(BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Ciclo Invierno Z1");

    /////  Boton Salir del Menu  /////
    backMenuButton.initButton(&tft, 120,
                              300,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_LIGHTGREY,
                              ILI9341_WHITE, "Volver", BUTTON_TEXTSIZE);
    backMenuButton.drawButton();
    /////  Boton Salir del Menu  /////

    inviernoz1ScreenInit = true;
  }
  delay(50);  // debouncing
}

/////  Pantalla Primavera Z1  ////
void Primaveraz1Screen(String temp, String hum) {
  if (!primaveraz1ScreenInit) {
    tft.fillScreen(BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Ciclo Primavera Z1");

    /////  Boton Salir del Menu  /////
    backMenuButton.initButton(&tft, 120,
                              300,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_LIGHTGREY,
                              ILI9341_WHITE, "Volver", BUTTON_TEXTSIZE);
    backMenuButton.drawButton();
    /////  Boton Salir del Menu  /////

    primaveraz1ScreenInit = true;
  }
  delay(50);  // debouncing
}

/////  Pantalla Verano Z2  /////
void Veranoz2Screen(String temp, String hum) {
  if (!veranoz2ScreenInit) {
    tft.fillScreen(BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Ciclo Verano Z2");

    /////  Boton Salir del Menu  /////
    backMenuButton.initButton(&tft, 120,
                              300,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_LIGHTGREY,
                              ILI9341_WHITE, "Volver", BUTTON_TEXTSIZE);
    backMenuButton.drawButton();
    /////  Boton Salir del Menu  /////

    veranoz2ScreenInit = true;
  }
  delay(50);  // debouncing
}

/////  Pantalla Otoño Z2  /////
void Otonioz2Screen(String temp, String hum) {
  if (!otonioz2ScreenInit) {
    tft.fillScreen(BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Ciclo Otonio Z2");

    /////  Boton Salir del Menu  /////
    backMenuButton.initButton(&tft, 120,
                              300,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_LIGHTGREY,
                              ILI9341_WHITE, "Volver", BUTTON_TEXTSIZE);
    backMenuButton.drawButton();
    /////  Boton Salir del Menu  /////

    otonioz2ScreenInit = true;
  }
  delay(50);  // debouncing
}

/////  Pantalla Invierno Z2  /////
void Inviernoz2Screen(String temp, String hum) {
  if (!inviernoz2ScreenInit) {
    tft.fillScreen(BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Ciclo Invierno Z2");

    /////  Boton Salir del Menu  /////
    backMenuButton.initButton(&tft, 120,
                              300,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_LIGHTGREY,
                              ILI9341_WHITE, "Volver", BUTTON_TEXTSIZE);
    backMenuButton.drawButton();
    /////  Boton Salir del Menu  /////

    inviernoz2ScreenInit = true;
  }
  delay(50);  // debouncing
}

/////  Pantalla Primavera Z2  /////
void Primaveraz2Screen(String temp, String hum) {
  if (!primaveraz2ScreenInit) {
    tft.fillScreen(BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Ciclo Primavera Z2");

    /////  Boton Salir del Menu  /////
    backMenuButton.initButton(&tft, 120,
                              300,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_LIGHTGREY,
                              ILI9341_WHITE, "Volver", BUTTON_TEXTSIZE);
    backMenuButton.drawButton();
    /////  Boton Salir del Menu  /////

    primaveraz2ScreenInit = true;
  }
  delay(50);  // debouncing
}

/////  Pantalla control Z1    /////
void Controlz1Screen(String temp, String hum) {
  if (!controlz1ScreenInit) {
    tft.fillScreen(BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Iot");

    /////  Boton Salir del Menu  /////
    backMenuButton.initButton(&tft, 120,
                              300,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_LIGHTGREY,
                              ILI9341_WHITE, "Volver", BUTTON_TEXTSIZE);
    backMenuButton.drawButton();
    /////  Boton Salir del Menu  /////

    controlz1ScreenInit = true;
  }
  delay(50);  // debouncing
}

/////  Pantalla control Z2    /////
void Controlz2Screen(String temp, String hum) {
  if (!controlz2ScreenInit) {
    tft.fillScreen(BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Iot");

    /////  Boton Salir del Menu  /////
    backMenuButton.initButton(&tft, 120,
                              300,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_LIGHTGREY,
                              ILI9341_WHITE, "Volver", BUTTON_TEXTSIZE);
    backMenuButton.drawButton();
    /////  Boton Salir del Menu  /////

    controlz2ScreenInit = true;
  }
  delay(50);  // debouncing
}

/////  Pantalla Alarmas       /////
void AlarmasScreen(String temp, String hum) {
  if (!alarmasScreenInit) {
    tft.fillScreen(BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Alarmas");

    /////  Boton Salir del Menu  /////
    backMenuButton.initButton(&tft, 120,
                              300,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_LIGHTGREY,
                              ILI9341_WHITE, "Volver", BUTTON_TEXTSIZE);
    backMenuButton.drawButton();
    /////  Boton Salir del Menu  /////

    alarmasScreenInit = true;
  }
  delay(50);  // debouncing
}

/////  Pantalla WiFi          /////
void WiFiScreen(String temp, String hum) {
  if (!wifiScreenInit) {
    tft.fillScreen(BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("WiFi");

    /////  Boton Salir del Menu  /////
    backMenuButton.initButton(&tft, 120,
                              300,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_LIGHTGREY,
                              ILI9341_WHITE, "Volver", BUTTON_TEXTSIZE);
    backMenuButton.drawButton();
    /////  Boton Salir del Menu  /////

    wifiScreenInit = true;
  }
  delay(50);  // debouncing
}

/////  Pantalla Iot           /////
void IotScreen(String temp, String hum) {
  if (!iotScreenInit) {
    tft.fillScreen(BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Iot");

    /////  Boton Salir del Menu  /////
    backMenuButton.initButton(&tft, 120,
                              300,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_LIGHTGREY,
                              ILI9341_WHITE, "Volver", BUTTON_TEXTSIZE);
    backMenuButton.drawButton();
    /////  Boton Salir del Menu  /////

    iotScreenInit = true;
  }
  delay(50);  // debouncing
}

/////  Pantalla Configuracion temp hum  /////
void SettingsScreen(String temp, String hum) {
  if (!settingsScreenInit) {
    tft.fillScreen(BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Temp. Setpoint");
    tft.drawRect(10, 30, TEXT_W, TEXT_H, ILI9341_WHITE);
    tft.setCursor(10, 90);
    tft.print("Humidity Setpoint");
    tft.drawRect(10, 120, TEXT_W, TEXT_H, ILI9341_WHITE);

    backHomeButton.initButton(&tft, 120,
                              300,  // x, y, w, h, outline, fill, text
                              230, 40, ILI9341_WHITE, ILI9341_LIGHTGREY,
                              ILI9341_WHITE, "Salir", BUTTON_TEXTSIZE);
    backHomeButton.drawButton();

    settingsScreenInit = true;
  }
  tft.setCursor(TEXT_X + 2, 40);
  tft.setTextColor(TEXT_TCOLOR, ILI9341_BLACK);
  tft.setTextSize(TEXT_TSIZE);
  tft.print(temp);

  tft.setCursor(TEXT_X + 2, 130);
  tft.setTextColor(TEXT_TCOLOR, ILI9341_BLACK);
  tft.setTextSize(TEXT_TSIZE);
  tft.print(hum);

  delay(50);  // debouncing
}