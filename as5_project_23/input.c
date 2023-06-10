#include "input.h"
#include "arm2avr.h"

void InputInit(void){
    IoFromAvr.AdValue[0] = 1023;
    IoFromAvr.AdValue[1] = 1023;
    IoFromAvr.AdValue[2] = 1023;
    IoFromAvr.AdValue[3] = 1023;
}
void InputExit(void){
    ;
}
void InputGetSensorValue(UWORD *value, UBYTE port){
    *value = IoFromAvr.AdValue[port];
}
