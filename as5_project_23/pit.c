#include"AT91SAM7S256.h"
#include"pit.h"
#include"display.h"
#include"aic.h"

void PITEnable(void) {
    *AT91C_PITC_PIMR = AT91C_PITC_PITEN + 2999;
}

void PITDisable(void) {
    *AT91C_PITC_PIMR = 0;
}

ULONG PITRead(void) {
    return *AT91C_PITC_PIIR;
}

UWORD PITTicks2ms(ULONG ticks){
    return ((UWORD)ticks / 3000);
}

UWORD PITTicks2s(ULONG ticks){
    return ((UWORD)ticks / 3000000);
}

void spindelayms(ULONG ms){
    // reset counters by reading the value register
    PITReadReset();

    while(( PITRead() >> 20) < ms){
    }

    // reset counters by reading the value register
    PITReadReset();

    return;
}

ULONG PITReadReset(void) {
    return *AT91C_PITC_PIVR;
}

void PITInterruptEnable(ULONG period, void (*handler)(void)){   
    *AT91C_PITC_PIMR = AT91C_PITC_PITEN | AT91C_PITC_PITIEN | (period - 1);
    AICInterruptEnable(1, handler, 7);
}

void PITInterruptDisable(void){
    *AT91C_PITC_PIMR = *AT91C_PITC_PIMR - AT91C_PITC_PITIEN;
    AICInterruptDisable(1);
}

void PITAckInterrupt(void){
    PITReadReset();
}