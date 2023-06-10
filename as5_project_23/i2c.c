#include "AT91SAM7S256.h"
#include  <stdconst.h>
#include  <string.h>
#include  <assert.h>
#include  "i2c.h"
#include  "arm2avr.h"
#include  "pit.h"
#include  "aic.h"

#define BYTES_TO_TX     sizeof(IOTOAVR)
#define BYTES_TO_RX     sizeof(IOFROMAVR)
#define TIMEOUT         2100
#define I2CClk          400000L
#define TIME400KHz      (((OSC/16L)/(I2CClk * 2)) + 1)
#define CLDIV           (((OSC/I2CClk)/2)-3)
#define DEVICE_ADR      0x01
#define COPYRIGHTSTRING "Let's samba nxt arm in arm, (c)LEGO System A/S"
#define COPYRIGHTSTRINGLENGTH 46
const   UBYTE CopyrightStr[] = {"\xCC"COPYRIGHTSTRING};
#define MAX(x,y) (((x)>(y))?(x):(y))
#define piir (*AT91C_PITC_PIIR & AT91C_PITC_CPIV)
#define pimr (*AT91C_PITC_PIMR & AT91C_PITC_CPIV)
#define DISABLEI2cIrqs *AT91C_TWI_IDR = 0x000001C7


__ramfunc void I2cHandler(void);


// send/receive state machine variables
 enum state_t {
  RESET,
  SEND,
  SENDING,
  RECV,
  RECEIVING
}; 

static enum state_t volatile State = RESET;
static UBYTE volatile checksum;
static UBYTE volatile temp[sizeof(CopyrightStr) + 1];
static unsigned volatile total_bytes_to_transmit;
static unsigned volatile transmited_bytes_counter;

#define CHECKSUM(b, len) {\
            for(int i = 0 ; i < len ; ++i )\
              checksum+=b[i];\
            checksum = ~checksum;\
} 

void DataTxInit(UBYTE *buf, UBYTE len) {
  // transmit these bytes
  total_bytes_to_transmit = len + 1 ; 
  
  transmited_bytes_counter = 0 ; 
  
  checksum = 0;


  // checksum <- checksum(temp)
  for (size_t i = 0; i < len; ++i){
      checksum += buf[i];
  }
  checksum = ~checksum;


  // temp <- copy(buf) 
  memcpy((void*)temp , buf , len);

  // CHECKSUM(buf , len);
  // temp.append(checksum)
  temp[len] = checksum;
  // ier <- TxRDY | error
  *AT91C_TWI_IER = AT91C_TWI_TXRDY;


  // aic <- twi
  AICInterruptEnable(AT91C_ID_TWI , (void*)I2cHandler,8 );


  // mmr <- write | device addr 
  *AT91C_TWI_MMR = (DEVICE_ADR << 16);
  // *AT91C_TWI_MMR &= 0xFFFFEFFF;

  // cr <- msen | start
    *AT91C_TWI_CR = AT91C_TWI_START | AT91C_TWI_MSEN;

  while( !(*AT91C_TWI_SR & AT91C_TWI_TXRDY)){;}


  // thr <- temp[0]
  *AT91C_TWI_THR = temp[0];
  transmited_bytes_counter+=1;

  return;
}  

void DataRxInit(void) {
  // receive these bytes
  total_bytes_to_transmit = BYTES_TO_RX + 1 ;
  transmited_bytes_counter = 0;
  checksum = 0;


  // ier <- RxRDY | errors
  *AT91C_TWI_IER = AT91C_TWI_RXRDY;


  // aic <- TWI , handler
  AICInterruptEnable(AT91C_ID_TWI , (void*)I2cHandler,8);

  // mmr <- read | addr
  *AT91C_TWI_MMR = (DEVICE_ADR << 16) | AT91C_TWI_IADRSZ_NO | AT91C_TWI_MREAD;


  // cr <- msen | start
  *AT91C_TWI_CR = AT91C_TWI_START | AT91C_TWI_MSEN;


  return;
}

__ramfunc void I2cHandler(void) {

  switch (State)
  {
  case RECEIVING:
    if(transmited_bytes_counter < total_bytes_to_transmit-2){//if any
      //buf[i]<-rhr
      temp[transmited_bytes_counter] = *AT91C_TWI_RHR;
      //checksum+=buf[i++];
      checksum += temp[transmited_bytes_counter++];

    }
    else if(transmited_bytes_counter == total_bytes_to_transmit-2){//if last-1
      //buf[i]<-rhr
      temp[transmited_bytes_counter] = *AT91C_TWI_RHR;
      // cr<-stop
      *AT91C_TWI_CR = AT91C_TWI_STOP;
      //checksum+=buf[i++];
      checksum += temp[transmited_bytes_counter++];
      checksum = ~checksum;

    }
    else if (transmited_bytes_counter >= total_bytes_to_transmit-1){

      DISABLEI2cIrqs;
      AICInterruptDisable(AT91C_ID_TWI);
      //check if the checksum is correct
      if(*AT91C_TWI_RHR== checksum  ){
        memcpy(&IoFromAvr, (void*)temp , total_bytes_to_transmit-1);
      }
      State = SEND;
    }
  
    break;
  case SENDING:
    
    if(transmited_bytes_counter < total_bytes_to_transmit-1){//if any
      //thr <- buf[i++]
      *AT91C_TWI_THR = temp[transmited_bytes_counter++];
    }
    else if(transmited_bytes_counter == total_bytes_to_transmit-1){//if last
      // cr<-stop
      *AT91C_TWI_CR = AT91C_TWI_STOP;

      //thr <- buf[i]
      *AT91C_TWI_THR = temp[transmited_bytes_counter++];

    }
    else if(transmited_bytes_counter >= total_bytes_to_transmit){//if after last
      DISABLEI2cIrqs;
      AICInterruptDisable(AT91C_ID_TWI);
      State = RECV;
    }

    break;
  }

}

void I2CTransfer(void) {

  //uses DataTxInit() and DataRxInit()
  //the first transaction has to be the 'hello' transaction

  //spin 2ms
  // spindelayms(2);
  while(( PITRead() >> 20) < 2){;}


  switch (State)
  {
  case RESET: 
    // transmit "hello...."
    DataTxInit((UBYTE*)CopyrightStr , sizeof(CopyrightStr));
    State = SENDING;
    break;
  
  case SEND:
    DataTxInit((UBYTE*)&IoToAvr , BYTES_TO_TX);
    State = SENDING;
    break;
  
  case RECV:
    DataRxInit();
    State = RECEIVING;
    break;
  
  case SENDING://nothing
    break;
  
  case RECEIVING://nothing
    break;

  }
  PITReadReset();
  return;
}

void I2CCtrl (enum power_t p) {
  //sets the power mode in AVR and allows the controller to run in regular mode,reprogram mode, or powers the controller down.

  //i want to split the p value , half in power and half in freq.
  IoToAvr.Power = p >>8 ;
  IoToAvr.PwmFreq = p & 0x00FF ;

  if(p == REPROGRAM){
    IoToAvr.PwmValue[0] = 0;
    IoToAvr.PwmValue[1] = 0;
    IoToAvr.PwmValue[2] = 0;
  }

  return;
}

#define WAITClk(t) {\
	  ULONG pit = piir + (t);\
          if (pit >= pimr) pit -= pimr;\
          while (piir < pit){;}\
        }

void I2CInit(void) { 
  //
  // disable I2C on PIO
  // this is called also during an error, so interrupts etc may be enabled
  //
  *AT91C_AIC_IDCR = (1L<<AT91C_ID_TWI);			/* disable AIC irq  */
  DISABLEI2cIrqs;                      			/* disable TWI irq  */
  *AT91C_PMC_PCER  = (1L<<AT91C_ID_TWI);		/* enable TWI Clock */
  *AT91C_PIOA_OER  = AT91C_PA4_TWCK;  		  	/* SCL is output    */
  *AT91C_PIOA_ODR  = AT91C_PA3_TWD;			/* SDA is input     */
  *AT91C_PIOA_MDER = (AT91C_PA4_TWCK | AT91C_PA3_TWD);  /* open drain       */
  *AT91C_PIOA_PPUDR = (AT91C_PA4_TWCK | AT91C_PA3_TWD); /* no pull up       */
  // 
  // synch I2C clocks using PIO
  // generate a 400KHz pulse on SCK and wait until both SCK and SDA are high, 
  // which means the slave ticks with this clock
  //
  *AT91C_PIOA_PER  = (AT91C_PA4_TWCK | AT91C_PA3_TWD); /* enable PIO control for these pins */
  while(((*AT91C_PIOA_PDSR & AT91C_PA3_TWD) == 0) || ((*AT91C_PIOA_PDSR & AT91C_PA4_TWCK) == 0)){
      *AT91C_PIOA_CODR = AT91C_PA4_TWCK; /* drive SCL Low  */
      WAITClk(TIME400KHz);
      *AT91C_PIOA_SODR = AT91C_PA4_TWCK; /* drive SCL High */
      WAITClk(TIME400KHz);
  }
  // 
  // init I2C on PIO
  //
  *AT91C_TWI_CR    =  AT91C_TWI_SWRST;			/* this has to happen before the rest */
  *AT91C_PIOA_PDR   = (AT91C_PA4_TWCK | AT91C_PA3_TWD); /* disable PIO control for these pins */
  *AT91C_PIOA_ASR   = (AT91C_PA4_TWCK | AT91C_PA3_TWD); /* select peripheral A = TWI */
  *AT91C_TWI_CWGR   = (CLDIV | (CLDIV << 8));           /* 400KHz clock    */
  *AT91C_AIC_ICCR   = (1L<<AT91C_ID_TWI);               /* clear AIC irq   */
  AT91C_AIC_SVR[AT91C_ID_TWI] = (unsigned int)I2cHandler;
  AT91C_AIC_SMR[AT91C_ID_TWI] = ((AT91C_AIC_PRIOR_HIGHEST) | (AT91C_AIC_SRCTYPE_INT_EDGE_TRIGGERED));
  *AT91C_AIC_IECR   = (1L<<AT91C_ID_TWI);               /* Enables AIC irq */

  IoToAvr.Power     = 0;


  return;
}

void I2CExit(void) {
  DISABLEI2cIrqs;
  *AT91C_AIC_IDCR   = (1L<<AT91C_ID_TWI);               /* Disable AIC irq  */
  *AT91C_AIC_ICCR   = (1L<<AT91C_ID_TWI);               /* Clear AIC irq    */
  *AT91C_PMC_PCDR   = (1L<<AT91C_ID_TWI);               /* Disable clock    */
  *AT91C_PIOA_MDER  = (AT91C_PA4_TWCK | AT91C_PA3_TWD); /* Open drain       */
  *AT91C_PIOA_PPUDR = (AT91C_PA4_TWCK | AT91C_PA3_TWD); /* no pull up       */
  *AT91C_PIOA_PER   = (AT91C_PA4_TWCK | AT91C_PA3_TWD); /* Disable peripheal*/
}
