#include "AT91SAM7S256.h"
#include  "spi.h"

#define SPI_BITRATE 2000000



void SPIInit(void) {
	*AT91C_PMC_PCER             = (1L << AT91C_ID_SPI);   /* Enable MCK clock     */
	// map the multiplexed pins of PIO to SPI
	*AT91C_PIOA_PER             = AT91C_PIO_PA12;         /* Enable A0 on PA12    */
	*AT91C_PIOA_OER             = AT91C_PIO_PA12;
	*AT91C_PIOA_CODR            = AT91C_PIO_PA12;
	*AT91C_PIOA_PDR             = AT91C_PA14_SPCK;        /* Enable SPCK on PA14  */
	*AT91C_PIOA_ASR             = AT91C_PA14_SPCK;
	*AT91C_PIOA_ODR             = AT91C_PA14_SPCK;
	*AT91C_PIOA_OWER            = AT91C_PA14_SPCK;
	*AT91C_PIOA_MDDR            = AT91C_PA14_SPCK;
	*AT91C_PIOA_PPUDR           = AT91C_PA14_SPCK;
	*AT91C_PIOA_IFDR            = AT91C_PA14_SPCK;
	*AT91C_PIOA_CODR            = AT91C_PA14_SPCK;
	*AT91C_PIOA_IDR             = AT91C_PA14_SPCK;
	*AT91C_PIOA_PDR             = AT91C_PA13_MOSI;        /* Enable mosi on PA13  */
	*AT91C_PIOA_ASR             = AT91C_PA13_MOSI;
	*AT91C_PIOA_ODR             = AT91C_PA13_MOSI;
	*AT91C_PIOA_OWER            = AT91C_PA13_MOSI;
	*AT91C_PIOA_MDDR            = AT91C_PA13_MOSI;
	*AT91C_PIOA_PPUDR           = AT91C_PA13_MOSI;
	*AT91C_PIOA_IFDR            = AT91C_PA13_MOSI;
	*AT91C_PIOA_CODR            = AT91C_PA13_MOSI;
	*AT91C_PIOA_IDR             = AT91C_PA13_MOSI;
	*AT91C_PIOA_PDR             = AT91C_PA10_NPCS2;       /* Enable npcs0 on PA11  */
	*AT91C_PIOA_BSR             = AT91C_PA10_NPCS2;
	*AT91C_PIOA_ODR             = AT91C_PA10_NPCS2;
	*AT91C_PIOA_OWER            = AT91C_PA10_NPCS2;
	*AT91C_PIOA_MDDR            = AT91C_PA10_NPCS2;
	*AT91C_PIOA_PPUDR           = AT91C_PA10_NPCS2;
	*AT91C_PIOA_IFDR            = AT91C_PA10_NPCS2;
	*AT91C_PIOA_CODR            = AT91C_PA10_NPCS2;
	*AT91C_PIOA_IDR             = AT91C_PA10_NPCS2;
	// init the SPI
	*AT91C_SPI_CR               = AT91C_SPI_SWRST;        /* Soft reset           */
	*AT91C_SPI_CR               = AT91C_SPI_SPIEN;        /* Enable spi           */
	*AT91C_SPI_MR               = AT91C_SPI_MSTR  | AT91C_SPI_MODFDIS | (0xB << 16);
	AT91C_SPI_CSR[2]              = ((OSC / SPI_BITRATE) << 8) | AT91C_SPI_CPOL;

	return;
}

unsigned int SPITxReady(void){
	// Check ony the TXEMPTY bit of the SR Register
	return ((*AT91C_SPI_SR & AT91C_SPI_TXEMPTY));
}

unsigned int SPIRxReady(void){
	// Check only the RDRF bit of the SR Register
    return (*AT91C_SPI_SR & AT91C_SPI_RDRF);


}

void SPIWrite(UBYTE *buffer , UBYTE length){
	//write all the bytes 1by1 to the  TDR . Each time wait until the next transfer is ready 
	for( int i = 0 ; i < length ; i++){
		*AT91C_SPI_TDR = buffer[i];
		while(SPITxReady() == 0){/*spin*/}
	}

}

void SPIRead(UBYTE *buffer , UBYTE length){
	//read 1 byte from the RDR each time , wait until the next read is ready
	for(int i = 0 ; i < length ; i ++){
		while(SPIRxReady() == 0){/*spin*/}
		buffer[i] = *AT91C_SPI_RDR >> 8;
	}

}

void SPIPIOSetData(void){
	// Set the Data to the drivven MISO 
	// Assign MISO to the SODR
	// Wait for the tranfer to complete before Set
	while(SPITxReady() == 0){/*spin*/}

	*AT91C_PIOA_SODR = AT91C_PIO_PA12;
}

void SPIPIOClearData(void){
	// Clear the Data to be drivven to MISO 
	// Assign MISO to the CODR
	// Wait for the tranfer to complete before Clear
	while(SPITxReady() == 0){/*spin*/}

	*AT91C_PIOA_CODR = AT91C_PIO_PA12;
}