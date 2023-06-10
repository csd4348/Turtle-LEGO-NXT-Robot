#include "output.h"
#include "arm2avr.h"

void      OutputInit(void){
    IoToAvr.Power = 0;
    IoToAvr.PwmFreq = 0;
    IoToAvr.PwmValue[0] = 0;
    IoToAvr.PwmValue[1] = 0;
    IoToAvr.PwmValue[2] = 0;
    IoToAvr.PwmValue[3] = 0;
    IoToAvr.OutputMode = 0;
    IoToAvr.InputPower = 0;

    

}

void      OutputExit(void){
    ;
}
void      OutputSetSpeed (UBYTE MotorNr, SBYTE Speed){
    IoToAvr.PwmValue[MotorNr] = Speed;

}