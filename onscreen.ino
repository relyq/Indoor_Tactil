////////////////////////////////////
bool numericScreenOn = false;
bool numericScreenInit = false;
////////////////////////////////////
bool homeScreenOn = true;
bool homeScreenInit = false;
////////////////////////////////////
bool settingsScreenOn = false;
bool settingsScreenInit = false;
////////////////////////////////////
bool menuScreenOn = false;
bool menuScreenInit = false;
////////////////////////////////////
bool opcion1ScreenOn = false;
bool opcion1ScreenInit = false;
////////////////////////////////////
bool opcion2ScreenOn = false;
bool opcion2ScreenInit = false;
////////////////////////////////////
bool opcion3ScreenOn = false;
bool opcion3ScreenInit = false;
////////////////////////////////////
bool opcion4ScreenOn = false;
bool opcion4ScreenInit = false;
////////////////////////////////////
bool opcion5ScreenOn = false;
bool opcion5ScreenInit = false;
////////////////////////////////////
bool veranoz1ScreenOn = false;
bool veranoz1ScreenInit = false;
////////////////////////////////////
bool otonioz1ScreenOn = false;
bool otonioz1ScreenInit = false;
////////////////////////////////////
bool inviernoz1ScreenOn = false;
bool inviernoz1ScreenInit = false;
////////////////////////////////////
bool primaveraz1ScreenOn = false;
bool primaveraz1ScreenInit = false;
////////////////////////////////////
bool veranoz2ScreenOn = false;
bool veranoz2ScreenInit = false;
////////////////////////////////////
bool otonioz2ScreenOn = false;
bool otonioz2ScreenInit = false;
////////////////////////////////////
bool inviernoz2ScreenOn = false;
bool inviernoz2ScreenInit = false;
////////////////////////////////////
bool primaveraz2ScreenOn = false;
bool primaveraz2ScreenInit = false;
////////////////////////////////////
bool controlz1ScreenOn = false;
bool controlz1ScreenInit = false;
////////////////////////////////////
bool controlz2ScreenOn = false;
bool controlz2ScreenInit = false;
////////////////////////////////////
bool alarmasScreenOn = false;
bool alarmasScreenInit = false;
////////////////////////////////////
bool wifiScreenOn = false;
bool wifiScreenInit = false;
////////////////////////////////////
bool iotScreenOn = false;
bool iotScreenInit = false;
////////////////////////////////////
bool numericScreenOnTemperature = false;
bool numericScreenOnHumidity = false;
////////////////////////////////////


homeScreenOn        // 0
menuScreenOn        // 1
settingsScreenOn    // 2
opcion1ScreenOn     // 3
opcion2ScreenOn     // 4
opcion3ScreenOn     // 5
opcion4ScreenOn     // 6
opcion5ScreenOn     // 7
veranoz1ScreenOn    // 8
otonioz1ScreenOn    // 9
inviernoz1ScreenOn  // 10
primaveraz1ScreenOn // 11
veranoz2ScreenOn    // 12
otonioz2ScreenOn    // 13
inviernoz2ScreenOn  // 14
primaveraz2ScreenOn // 15
controlz1ScreenOn   // 16
controlz2ScreenOn   // 17
alarmasScreenOn     // 18
wifiScreenOn        // 19
iotScreenOn         // 20
numericScreenOn     // 21
numericScreenOnTemperature  // 22
numericScreenOnHumidity     // 23



uint8_t screensTotal = 24;    // cantidad de pantallas
bool screens[screensTotal];   // array donde se guarda la pantalla activa
                              // entonces, screens[1] = true significa menuScreenOn = true

// cambia la pantalla activa
void changeActiveScreenTo(uint8_t newActiveScreen){
  for(uint8_t i = 0; i < screensTotal; i++){
    if     (i == newActiveScreen) screens[i] = true;
    else if(i != newActiveScreen) screens[i] = false;
  }
}