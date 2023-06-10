#include "button.h"
#include "arm2avr.h"

void   ButtonInit(void){
    IoFromAvr.Buttons = 0;
}
void   ButtonExit(void){

    ;
}
enum button_t ButtonRead(void){
    if(IoFromAvr.Buttons == 127) return BUTTON_LEFT;
    else if(IoFromAvr.Buttons == 1023) return BUTTON_EXIT;
    else if(IoFromAvr.Buttons == 2047) return BUTTON_ENTER;
    else if(IoFromAvr.Buttons == 406) return BUTTON_RIGHT;
    else return BUTTON_NONE;

    return 0;
}