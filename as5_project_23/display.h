#ifndef   DISPLAY
#define   DISPLAY

#include <stdconst.h>

void      DisplayInit(void);
void      DisplayExit(void);
void      DisplayErase(void);
void      DisplayUpdateSync(void);
void      DisplayChar(UBYTE X,UBYTE Y,UBYTE Char);
void      DisplayNum(UBYTE X,UBYTE Y,ULONG Num);
void      DisplayString(UBYTE X,UBYTE Y,UBYTE *pString);
void      DisplaySetPixel(UBYTE X,UBYTE Y);
void      DisplayClrPixel(UBYTE X,UBYTE Y);
void      DisplayLineX(UBYTE X1,UBYTE X2,UBYTE Y);
void      DisplayLineY(UBYTE X,UBYTE Y1,UBYTE Y2);
void      DisplayLineXY(UBYTE X1 ,UBYTE Y1,UBYTE X2,UBYTE Y2 );
#endif
