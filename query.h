#ifndef QUERY_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sqlite3.h"
#include <libnova/solar.h>
#include <libnova/lunar.h>
#include <libnova/mercury.h>
#include <libnova/venus.h>
#include <libnova/mars.h>
#include <libnova/jupiter.h>
#include <libnova/saturn.h>
#include <libnova/uranus.h>
#include <libnova/neptune.h>
#include <libnova/pluto.h>
#include <libnova/ln_types.h>
#include <libnova/julian_day.h>
#include <libnova/transform.h>
#include <libnova/utility.h>
#include <libnova/precession.h>
#include <libnova/elliptic_motion.h>
#include "conf.h"
//void squery(char* param,char* out, struct ln_equ_posn *pos);
int squery(char* param,char* out,struct ln_equ_posn *pos,char table);
//void squery(char* param,char* out,struct ln_equ_posn *pos);
int initDB();
void closeDB();
#endif // QUERY_H_INCLUDED
