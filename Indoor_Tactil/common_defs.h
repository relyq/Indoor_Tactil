#ifndef COMMON_DEFS_H
#define COMMON_DEFS_H

// Color Definitions
#define BLACK 0x0000
#define LIGHTGREY 0xC618
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define NAVY 0x000F
#define DARKGREEN 0x03E0
#define DARKCYAN 0x03EF
#define MAROON 0x7800
#define PURPLE 0x780F
#define OLIVE 0x7BE0
#define DARKGREY 0x7BEF
#define ORANGE 0xFD20
#define GREENYELLOW 0xAFE5
#define PINKK 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

#define CHARACTER_WIDTH 6   // including space 1px
#define CHARACTER_HEIGHT 7  // not including space 1px
#define BUTTON_TEXTSIZE 3
#define TITLE_TEXTSIZE 2

typedef struct Fase {
  uint16_t dias;
  uint8_t hLuz;
  uint8_t templ;
  uint8_t temph;
  uint8_t riegol;
  uint8_t riegoh;
  uint8_t huml;
  uint8_t humh;
} Fase;

typedef struct Programa {
  Fase f1;
  Fase f2;
  Fase f3;
  Fase f4;
} Programa;

typedef struct fActiva {
  uint16_t dias;
  uint8_t hLuz;
  uint8_t templ;
  uint8_t temph;
  uint8_t riegol;
  uint8_t riegoh;
  uint8_t huml;
  uint8_t humh;

  uint32_t diaIniciodefase;  // dia en unixtime del inicio de la fase activa
  uint32_t diaFindefase;     // dia en unixtime del fin de la fase activa
  uint32_t sLuz;             // segundo del dia en el que prende la luz
  uint32_t sFinLuz;          // segundo del dia en el que apaga la luz
  uint8_t ciclos;            // cantidad de ciclos - 0 = ciclo continuo
} fActiva;

typedef struct Reloj {
  uint16_t YYYY;
  uint8_t MM;
  uint8_t DD;
  uint8_t hh;
  uint8_t mm;
} Reloj;

typedef struct Estado {
  float t;
  uint16_t dias;
  uint8_t hTierra;
  uint8_t h;
} Estado;

#endif