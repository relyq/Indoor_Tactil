#ifndef _OUTPUT_THINGS_H_
#define _OUTPUT_THINGS_H_

#define POSITIVE_LOGIC 0
#define FAN_ON()          \
  {                       \
    if (POSITIVE_LOGIC) { \
      PORTC |= FANPIN;    \
    } else {              \
      PORTC &= ~FANPIN;   \
    }                     \
  }
#define FAN_OFF()         \
  {                       \
    if (POSITIVE_LOGIC) { \
      PORTC &= ~FANPIN;   \
    } else {              \
      PORTC |= FANPIN;    \
    }                     \
  }
#define HEAT_ON()         \
  {                       \
    if (POSITIVE_LOGIC) { \
      PORTC |= HEATPIN;   \
    } else {              \
      PORTC &= ~HEATPIN;  \
    }                     \
  }
#define HEAT_OFF()        \
  {                       \
    if (POSITIVE_LOGIC) { \
      PORTC &= ~HEATPIN;  \
    } else {              \
      PORTC |= HEATPIN;   \
    }                     \
  }
#define VAP_ON()          \
  {                       \
    if (POSITIVE_LOGIC) { \
      PORTC |= VAPPIN;    \
    } else {              \
      PORTC &= ~VAPPIN;   \
    }                     \
  }
#define VAP_OFF()         \
  {                       \
    if (POSITIVE_LOGIC) { \
      PORTC &= ~VAPPIN;   \
    } else {              \
      PORTC |= VAPPIN;    \
    }                     \
  }
#define RIEGO_ON()        \
  {                       \
    if (POSITIVE_LOGIC) { \
      PORTC |= RIEGOPIN;  \
    } else {              \
      PORTC &= ~RIEGOPIN; \
    }                     \
  }
#define RIEGO_OFF()       \
  {                       \
    if (POSITIVE_LOGIC) { \
      PORTC &= ~RIEGOPIN; \
    } else {              \
      PORTC |= RIEGOPIN;  \
    }                     \
  }
#define LUZ_ON()          \
  {                       \
    if (POSITIVE_LOGIC) { \
      PORTC |= LUZPIN;    \
    } else {              \
      PORTC &= ~LUZPIN;   \
    }                     \
  }
#define LUZ_OFF()         \
  {                       \
    if (POSITIVE_LOGIC) { \
      PORTC &= ~LUZPIN;   \
    } else {              \
      PORTC |= LUZPIN;    \
    }                     \
  }
#define SYSTEM_OFF()                                             \
  {                                                              \
    if (POSITIVE_LOGIC) {                                        \
      PORTC &= ~(FANPIN | HEATPIN | VAPPIN | RIEGOPIN | LUZPIN); \
    } else {                                                     \
      PORTC |= (FANPIN | HEATPIN | VAPPIN | RIEGOPIN | LUZPIN);  \
    }                                                            \
  }

#endif  // _OUTPUT_THINGS_H_