/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */
#ifndef CONTROL_H
#define CONTROL_H

#define REGISTER 10
#define REGOK 11
#define START 20
#define STOP 30
#define ERROR 50

#define READ_DET 100

#include "Config.h"

struct cPkt {
  short cmd;
  unsigned short scid;
};
typedef struct cPkt cPkt_t;

#endif
