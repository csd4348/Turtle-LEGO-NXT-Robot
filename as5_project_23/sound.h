#ifndef   SOUND
#define   SOUND

#include <stdconst.h>

void SoundInit(void);
void SoundSync(ULONG *pattern, UBYTE length, UBYTE rate, UWORD duration);
void SoundAsync(ULONG *pattern, UBYTE length, UBYTE rate, UWORD duration);
void SoundExit(void);

#endif
