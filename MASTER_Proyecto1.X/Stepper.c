#include <xc.h>
#include "Stepper.h"

/*------------------------------------------------------------------------------
 * Funciones
------------------------------------------------------------------------------*/

void stepper_step (uint8_t step){   
    TRISA = 0;                 
    
    switch(step){
        case 1:
            TRISAbits.TRISA0 = 1; 
            TRISAbits.TRISA1 = 0;
            TRISAbits.TRISA2 = 0;
            TRISAbits.TRISA3 = 0;
            break;
        
        case 2:
            TRISAbits.TRISA0 = 0; 
            TRISAbits.TRISA1 = 1;
            TRISAbits.TRISA2 = 0;
            TRISAbits.TRISA3 = 0;   
            break;
        
        case 3:
            TRISAbits.TRISA0 = 0; 
            TRISAbits.TRISA1 = 0;
            TRISAbits.TRISA2 = 1;
            TRISAbits.TRISA3 = 0;    
            break;
            
        case 4:
            TRISAbits.TRISA0 = 0; 
            TRISAbits.TRISA1 = 0;
            TRISAbits.TRISA2 = 0;
            TRISAbits.TRISA3 = 1;    
            break;
                  
        default:
            break;
    }
    
    return;
}
