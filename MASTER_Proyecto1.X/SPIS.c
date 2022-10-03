#include <xc.h>
#include "SPIS.h"

/*------------------------------------------------------------------------------
 * Funciones
------------------------------------------------------------------------------*/

void init_SPI (void){   
    TRISC = 0b10011000; // -> SDI y SCK entradas, SD0 como salida
    PORTC = 0;
    // SSPCON <5:0>
    SSPCONbits.SSPM = 0b0100;   // -> SPI Esclavo, SS entrada o salida
    SSPCONbits.CKP = 0;         // -> Reloj inactivo en 0
    SSPCONbits.SSPEN = 1;       // -> Habilitamos pines de SPI
    // SSPSTAT<7:6>
    SSPSTATbits.CKE = 1;        // -> Dato enviado cada flanco de subida
    SSPSTATbits.SMP = 0;        // -> Dato al final del pulso de reloj

    PIR1bits.SSPIF = 0;         // Limpiamos bandera de SPI
    PIE1bits.SSPIE = 1;         // Habilitamos int. de SPI
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
  
    
    return;
}

void spiReceiveWait(void)
{
    while ( !SSPSTATbits.BF );
}

void spiWrite(char dat)
{
    SSPBUF = dat;
}

unsigned spiDataReady(void)
{
    if(SSPSTATbits.BF)
        return 1;
    else
        return 0;
}

char spiRead(void)
{
    spiReceiveWait(); 
    return(SSPBUF);
}



