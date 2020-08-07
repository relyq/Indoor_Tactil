#ifndef EEPROMTHINGS_H
#define EEPROMTHINGS_H

#include "Arduino.h"
#include "common_defs.h"

void eeprom_cargarfActivaSP(fActiva* fActivaSP, uint8_t f);
void eeprom_cargarPrograma(Programa* p);
void eeprom_hardReset();
void eeprom_clear();
void eeprom_read(uint8_t length = 0);
uint16_t eeprom_dirFase(uint8_t fase);
extern void drawGoodbyeScreen();

#endif