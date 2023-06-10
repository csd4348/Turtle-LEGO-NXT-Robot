#ifndef   _INPUT_H
#define   _INPUT_H
#include <stdconst.h>

void InputInit(void);
void InputExit(void);
void InputGetSensorValue(UWORD *value, UBYTE port);

#endif
