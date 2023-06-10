#include "AT91SAM7S256.h"
#include "display.h"
#include "aic.h"

void AICInit(void){
    *AT91C_AIC_IECR = 0;
    *AT91C_AIC_IDCR = 0;
}



void AICInterruptEnable(int which, void (*handler)(void), UBYTE priority)
{
    *AT91C_AIC_IECR = 0x1 << which;
    AT91C_AIC_SVR[which] = (AT91_REG)handler;
    AT91C_AIC_SMR[which] = AT91C_AIC_SRCTYPE_INT_EDGE_TRIGGERED | priority;
}

void AICInterruptDisable(int which){
    *AT91C_AIC_IDCR = 0x1 << which;
    *AT91C_AIC_ICCR = 0x1 << which;
    AT91C_AIC_SVR[which] = 0x0;
}