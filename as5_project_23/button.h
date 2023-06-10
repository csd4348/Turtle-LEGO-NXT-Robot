#ifndef   _BUTTON_H_
#define   _BUTTON_H_

enum button_t {
  BUTTON_NONE,
  BUTTON_LEFT,
  BUTTON_ENTER,
  BUTTON_RIGHT,
  BUTTON_EXIT 
};
extern char *ButtonToString[];

void   ButtonInit(void);
void   ButtonExit(void);
enum button_t ButtonRead(void);

#endif
