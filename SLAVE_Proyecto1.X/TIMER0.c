
#include <stdint.h>
#include "TIMER0.h"
#define numero 12; //Valor a cargar

void tmr0_init (uint8_t prescaler)
{
    INTCONbits.T0IE = 1;//tmr0 activado
    
    OPTION_REGbits.PSA = 0;  //Presc asignado al TMR0 y no al WDT
    OPTION_REGbits.T0CS = 0;  // bit 5  TMR0 Clock Source Select bit...0 = Internal Clock (CLKO) 1 = Transition on T0CKI pin
    OPTION_REGbits.T0SE = 0;  //incremento en flanco de subida
    
    switch(prescaler){
        case 0:
            OPTION_REGbits.PS2 = 0; // 1:2
            OPTION_REGbits.PS1 = 0;
            OPTION_REGbits.PS0 = 0;
            break;
        case 1:
            OPTION_REGbits.PS2 = 0; // 1:4
            OPTION_REGbits.PS1 = 0;
            OPTION_REGbits.PS0 = 1;
            break;
        case 2:
            OPTION_REGbits.PS2 = 0; // 1:8
            OPTION_REGbits.PS1 = 1;
            OPTION_REGbits.PS0 = 0;
            break;
        case 3:
            OPTION_REGbits.PS2 = 0; // 1:16
            OPTION_REGbits.PS1 = 1;
            OPTION_REGbits.PS0 = 1;
            break;
        case 4:
            OPTION_REGbits.PS2 = 1; // 1:32
            OPTION_REGbits.PS1 = 0;
            OPTION_REGbits.PS0 = 0;
            break;
        case 5:
            OPTION_REGbits.PS2 = 1; // 1:64
            OPTION_REGbits.PS1 = 0;
            OPTION_REGbits.PS0 = 1;
            break;
        case 6:
            OPTION_REGbits.PS2 = 1; // 1:128
            OPTION_REGbits.PS1 = 1;
            OPTION_REGbits.PS0 = 0;
            break;
        case 7:
            OPTION_REGbits.PS2 = 1; // 1:256
            OPTION_REGbits.PS1 = 1;
            OPTION_REGbits.PS0 = 1;
            break;
        default:
            OPTION_REGbits.PS2 = 1; // 1:256
            OPTION_REGbits.PS1 = 1;
            OPTION_REGbits.PS0 = 1;
              
    }
}

void tmr0_reload (void)
{  
  TMR0 = numero;
  INTCONbits.T0IF = 0;//tmr0 int
  return;
}