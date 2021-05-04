#ifndef DRAW_SCREENS_H
#define DRAW_SCREENS_H

#include <RTClib.h>
#include <stdint.h>

#include "common_defs.h"

class Adafruit_TFTLCD;
class Adafruit_GFX_Button;

void drawHomeScreen(Adafruit_TFTLCD* tft, Adafruit_GFX_Button* homeButtons,
                    const uint8_t z1fActiva, const fActiva fActivaSP,
                    const Estado mediciones, const DateTime now);

void drawMenuScreen(Adafruit_TFTLCD* tft, Adafruit_GFX_Button* menuButtons);

void drawAjustesScreen(Adafruit_TFTLCD* tft,
                       Adafruit_GFX_Button* ajustesButtons);

void drawRelojScreen(Adafruit_TFTLCD* tft, Adafruit_GFX_Button* relojButtons,
                     const DateTime now, Reloj* reloj);

void drawProgramasScreen(Adafruit_TFTLCD* tft,
                         Adafruit_GFX_Button* programasButtons);

void drawPrograma1Screen(Adafruit_TFTLCD* tft,
                         Adafruit_GFX_Button* programa1Buttons);

void drawPrograma2Screen(Adafruit_TFTLCD* tft,
                         Adafruit_GFX_Button* programa2Buttons);

void drawPrograma3Screen(Adafruit_TFTLCD* tft,
                         Adafruit_GFX_Button* programa3Buttons);

void drawPrograma4Screen(Adafruit_TFTLCD* tft,
                         Adafruit_GFX_Button* programa4Buttons);

void drawResetScreen(Adafruit_TFTLCD* tft, Adafruit_GFX_Button* resetButtons);

void drawZ1Screen(Adafruit_TFTLCD* tft, Adafruit_GFX_Button* z1Buttons);

void drawZ1F1Screen(Adafruit_TFTLCD* tft, Adafruit_GFX_Button* z1f1Buttons,
                    Programa* pActivo);

void drawZ1F2Screen(Adafruit_TFTLCD* tft, Adafruit_GFX_Button* z1f2Buttons,
                    Programa* pActivo);

void drawZ1F3Screen(Adafruit_TFTLCD* tft, Adafruit_GFX_Button* z1f3Buttons,
                    Programa* pActivo);

void drawZ1F4Screen(Adafruit_TFTLCD* tft, Adafruit_GFX_Button* z1f4Buttons,
                    Programa* pActivo);

void drawZ1ControlScreen(Adafruit_TFTLCD* tft,
                         Adafruit_GFX_Button* z1ControlButtons,
                         fActiva* fActivaSP);

void drawZ1InicioScreen(Adafruit_TFTLCD* tft,
                        Adafruit_GFX_Button* z1InicioButtons,
                        const uint8_t z1fActiva);

void drawNumericKeyboardScreen(Adafruit_TFTLCD* tft,
                               Adafruit_GFX_Button* numericKeyboardButtons,
                               const char* title, uint8_t prevScreen,
                               uint8_t* numKBPrevScreen, char* numKBstr);

void drawStartupScreen(Adafruit_TFTLCD* tft);

void drawGoodbyeScreen(Adafruit_TFTLCD* tft);

#endif