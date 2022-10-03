/* 
 * File:   MASTER_Proyecto.c
 * Author: Compu Fire
 *
 * Created on 4 de septiembre de 2022, 19:37
 */


// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


/*------------------------------------------------------------------------------
 * Includes & Librerías
------------------------------------------------------------------------------*/

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <stdint.h>

#include "UART.h"
#include "Oscilador.h"
#include "TIMER0.h"
#include "SPIS.h"
#include "LCD.h"
#include "Stepper.h"
#include "PWM.h"



/*------------------------------------------------------------------------------
 * Constantes
------------------------------------------------------------------------------*/
#define _XTAL_FREQ 1000000  //Oscilador de 1 MHz
uint8_t cont = 49;
uint8_t data1 = 0;
uint8_t data2 = 0;
uint8_t data3 = 0;
uint8_t data4 = 0;
uint8_t temp = 0;
uint16_t lux = 0;
uint8_t temperatura[4] = {0,0,0,0}; 
uint8_t luz[5] = {0,0,0,0,0};
uint32_t lumens = 0;
uint8_t cont_100 = 10;
uint8_t cont_lcd = 50;
uint8_t val;
uint8_t val2;
uint8_t val3;
uint8_t sec_decenas = 0;
uint8_t sec_unidades = 0;
uint8_t min_decenas = 0;
uint8_t min_unidades = 0;
uint8_t hour_unidades = 0;
uint8_t hour_decenas = 0;
uint8_t mover = 0;
uint8_t cont_mover = 4;
uint8_t cont_mover2 = 4;
uint8_t contador2 = 0; 


#define FLAG_SPI 0xFF
char val_temporal = 0;



/*
 * 
 */
void setup (void); 
void servo(unsigned short mov);
void valor_temperatura(int temp); 
void valor_lux(uint32_t lux);



void __interrupt() isr(void){ // FUNCION DE INTERRUPCIONES

   if (INTCONbits.T0IF){ // INTERRUPCION DEL TMR0 250ms
        tmr0_reload();
        if (cont_100 == 0){
            if (cont < 57){
            cont++;}
            else{
            cont = 48;}
            TXREG = cont;
            cont_100 = 10;
        }
        else{
            cont_100--;
        }
        
        if (cont_lcd == 0){
           mover = 1;
           cont_lcd = 50;
        }
        else{
            cont_lcd--;
        }
        
         contador2++;                          // Incremento de bandera
        if(contador2>3){                      // Reinicio de bandera
            contador2 = 0; 
        }
                //Motor SERVO
        if (min_unidades == 50){
            PORTBbits.RB7= 1;
            servo(1);
        }
        else{
            PORTBbits.RB7 = 0;  
            servo(0);
        }
        //Motor Stepper
        if (min_unidades == 50){
                    switch(contador2){
        case 0:                                 // centenas
            PORTB = 0b00000001;       
            break;
        case 1:                                 // decenas
            PORTB = 0b00000010;                 
            break;  
        case 2:                                 // unidades
            PORTB = 0b00000100;                
            break;
        case 3:                                 // unidades
            PORTB = 0b00001000;                
            break;
        default:
            PORTB = 0b00000000; 
            break;
            }
            }
        
            
            
   }
   else if(PIR1bits.RCIF){          // Hay datos recibidos?
                            // Guardamos valor recibido en el arreglo mensaje 
        if (cont == 49){
           data1 = RCREG;
        }
        else if (cont == 50){
           data2 = RCREG; 

        }
        else if (cont == 51){
           data3 = RCREG;

        }
        else if (cont == 52){
           data4 = RCREG;

        }
        else if (cont == 53){
           temp = RCREG;

        }
        else if (cont == 54){
           val = RCREG;

        }
        else if (cont == 55){
           val2 = RCREG;

        }
        else if (cont == 56){
           val3 = RCREG;

        }
        
    } 
   
   else if (spiDataReady()){
        
        val_temporal = spiRead();
        if (val_temporal != FLAG_SPI){  // revisa si es un dato
            if (val_temporal == 8){
                spiWrite(data1);
            }
            else if (val_temporal == 1){
                spiWrite(data2);
            }
            else if (val_temporal == 2){
                spiWrite(data3);
            }
            else if (val_temporal == 3){
                spiWrite(data4);
            }
            else if (val_temporal == 4){
                spiWrite(temp);
            }
            else if (val_temporal == 5){
                spiWrite(val);
            }
            else if (val_temporal == 6){
                spiWrite(val2);
            }
            else if (val_temporal == 7){
                spiWrite(val3);
            }
        }
        PIR1bits.SSPIF = 0;             // Limpiamos bandera de interrupci n?
    }
 return;   
}
void main(void) {
    setup();
    
    Lcd_Set_Cursor(1,1);                  // Iniciamos la escritura en la primera posición
    Lcd_Write_String("TEMP  LUX    TIME   DIST");   // Escribimos los datos deseados en el display
    while(1){

        lux = data1;
        lux = (lux<<8)|data2;
        lux = (lux<<8)|data3;
        lumens = (lux<<8)|data4;


        valor_lux (lumens);
        valor_temperatura(temp);
        
        sec_unidades = (val&0x0F)+48;  //Mascara para unidades o 4 LSB --> Revisar datasheet DS3231
        sec_decenas = (val >> 4) + 48;  //Shifteo para decenas o 4 MSB
        min_unidades = (val2&0x0F)+48;  //Mascara para unidades o 4 LSB --> Revisar datasheet DS3231
        min_decenas = (val2 >> 4) + 48;  //Shifteo para decenas o 4 MSB
        hour_unidades = (val3&0x0F)+48;  //Mascara para unidades o 4 LSB --> Revisar datasheet DS3231
        hour_decenas = (val3 >> 4) + 48;  //Shifteo para decenas o 4 MSB
        
        Lcd_Set_Cursor(2,1);
        if(temperatura [0] == 0){
            Lcd_Write_String(" ");
        }
        else{
            Lcd_Write_Char(temperatura [0] + 48); //ubicar ek valor del caracter en el ASCII
        }
        Lcd_Write_Char(temperatura [1] + 48);
        Lcd_Write_Char(temperatura[2] + 48);
        Lcd_Write_Char(67);//Caracter que representa la C en el ASCII
        Lcd_Write_String(" ");//Caracter que representa la C en el ASCII
        if(lumens == 65535){
            Lcd_Write_String("ERROR");
        }
        else{
            Lcd_Write_Char(luz [0] + 48); //ubicar ek valor del caracter en el ASCII
            Lcd_Write_Char(luz [1] + 48);
            Lcd_Write_Char(luz [2] + 48);
            Lcd_Write_Char(luz [3] + 48);
            Lcd_Write_Char(luz [4] + 48);
        }
        Lcd_Write_String(" ");
        Lcd_Write_Char(hour_decenas); //:
        Lcd_Write_Char(hour_unidades); //:
        Lcd_Write_Char(58); //:
        Lcd_Write_Char(min_decenas);
        Lcd_Write_Char(min_unidades); //:
        Lcd_Write_Char(58); //:
        Lcd_Write_Char(sec_decenas); //Decenas
        Lcd_Write_Char(sec_unidades); //Unidades
        if (mover == 1){
            if (cont_mover > 0){
            Lcd_Shift_Left();
            cont_mover--;
            mover = 0;
            cont_mover2 = 4;}
            else{
            
            if (cont_mover2 > 0){
            Lcd_Shift_Right();
            cont_mover2--;
            mover = 0;}
            else{
            cont_mover = 4; 
            }
           }
        }
    }     
    return;
}

void setup(void) {
    ANSEL = 0;
    ANSELH = 0;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    TRISC = 0;
    TRISCbits.TRISC7 = 1;
    PORTC = 0;
    TRISE = 0;
    TRISB = 0;
    TRISA = 0b00100000;
    PORTA = 0;
    
    
   
    pwm_init(1);
    init_osc_MHz (1);
    
    init_SPI ();
    init_UART();
   
    tmr0_init (7);//INTERRUPCION DEL TMR0 DE 10MS
    tmr0_reload();
    Lcd_Init();
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones de perifericos
    PIE1bits.RCIE = 1;          // Habilitamos Interrupciones de recepción
    
    return;
}

void valor_temperatura (int temp) // funcion que encuentra el valor del voltaje
{
   
        temperatura [0] =  (uint8_t)(temp/100);
        temperatura [3] =   temp%100;

        temperatura [1] =   temperatura[3]/10;
        temperatura [3] =   temperatura[3]%10;

        temperatura [2] =  temperatura[3];
    return;
}

void valor_lux (uint32_t lux) // funcion que encuentra el valor del voltaje
{
        uint32_t valor;
        
        luz[0] =  (uint8_t)(lux/10000);
        valor =   lux%10000;

        luz [1] = (uint8_t)(valor/1000);
        valor =  valor%1000;

        luz [2] = (uint8_t)(valor/100);
        valor = valor%100;
        
        luz [3] = (uint8_t)(valor/10);
        valor =   valor%10;
        
        luz [4] =  (uint8_t)(valor);
    return;
}
void servo(unsigned short mov){
    if (mov == 1){
        pwm_duty_cycle(94);             // Se mueve a 90°
    }
    else{
        pwm_duty_cycle(31);             // Se mueve a 0°
    }        
}