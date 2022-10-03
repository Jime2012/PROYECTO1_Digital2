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
#include "I2C.h"
#include "tsl2561.h"
#include "BMP280.h"

/*------------------------------------------------------------------------------
 * Constantes
------------------------------------------------------------------------------*/
#define _XTAL_FREQ 1000000  //Oscilador de 1 MHz
//#define TRIG_DIR TRISBbits.TRISB4
//#define ECHO_DIR TRISBbits.TRISB5
//
//#define TRIG_PIN PORTBbits.RB4
//#define ECHO_PIN PORTBbits.RB5
//
//unsigned int timer_high;
//unsigned int timer_low;
//float dis_hc = 0;
uint8_t high0 = 0;
uint8_t low0 = 0;
uint8_t high1 = 0;
uint8_t low1 = 0;
uint16_t channel0 = 0;
uint16_t channel1 = 0;
uint32_t lux = 0;
uint8_t data1 = 0;
uint8_t data2 = 0;
uint8_t data3 = 0;
uint8_t data4 = 0;
uint8_t temp1 = 0;
uint8_t temp2 = 0;
uint8_t temp3 = 0;
uint8_t var1_1= 0;
uint8_t var1_2 = 0;
uint8_t var2_1= 0;
uint8_t var2_2 = 0;
uint8_t var3_1= 0;
uint8_t var3_2 = 0;
uint16_t dig1 = 0;
uint16_t dig2 = 0;
uint16_t dig3 = 0;
uint32_t temp;
uint8_t temperatura = 0;
uint8_t cont;
uint8_t val;
uint8_t val2;
uint8_t val3;



void setup (void);
void __interrupt() isr(void){ // FUNCION DE INTERRUPCIONES

   if (INTCONbits.T0IF){ // INTERRUPCION DEL TMR0
            tmr0_reload();
            
            
   }
   if(PIR1bits.RCIF){          // Hay datos recibidos?
                            // Guardamos valor recibido en el arreglo mensaje  
       PORTB = RCREG;
        if (RCREG == 49){
           cont = 1;       
        }
        else if (RCREG == 50){
           cont = 2;       
        }
        else if (RCREG == 51){
           cont = 3;         
        }
        else if (RCREG == 52){
           cont = 4;        
        }
        else if (RCREG == 53){
           cont = 5;         
        }
       else if (RCREG == 54){
           cont = 6;         
        }
       else if (RCREG == 55){
           cont = 7;         
        }
       else if (RCREG == 56){
           cont = 8;         
        }
    }     
 return;   
}
void main(void) {
    setup();
    while(1){
        //     Sensor de luz 
        
        I2C_Master_Start(); 
        I2C_Master_Write(0x72);//Slave address in write
        I2C_Master_Write(0x80);//Registro
        I2C_Master_Write(0x03);//Dato
        I2C_Master_Stop();
        __delay_ms(1);
        
        I2C_Master_Start();
        I2C_Master_Write(0x72);
        I2C_Master_Write(0x81);
        I2C_Master_Write(0x00);
        I2C_Master_Stop();
        __delay_ms(1);

        I2C_Master_Start();
        I2C_Master_Write(0x72);
        I2C_Master_Write(0x8C);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0x73);
        low0 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(1);

        I2C_Master_Start();
        I2C_Master_Write(0x72);
        I2C_Master_Write(0x8D);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0x73);
        high0 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(1);
        
        I2C_Master_Start();
        I2C_Master_Write(0x72);
        I2C_Master_Write(0x8E);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0x73);
        low1 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(1);
        
        I2C_Master_Start();
        I2C_Master_Write(0x72);
        I2C_Master_Write(0x8F);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0x73);
        high1 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(1);
    
        
        channel0 = (high0<<8)|low0;
        channel1 = (high1<<8)|low1;
        
        lux = calculateLux(channel0,channel1);
        
        data4 = (uint8_t)(lux);
        data3 = (uint8_t)(lux>>8);
        data2 = (uint8_t)(lux>>16);
        data1 = (uint8_t)(lux>>24);
      
        //Sensor de temperatura
        
        I2C_Master_Start();
        I2C_Master_Write(0xEE);//Slave address in write
        I2C_Master_Write(0xF4);//Registro de control
        I2C_Master_Write(0x57);//Dato
        I2C_Master_Stop();
        __delay_ms(1);
        
        I2C_Master_Start();
        I2C_Master_Write(0xEE);//Slave address in write
        I2C_Master_Write(0xF5);//Registro de configuracion
        I2C_Master_Write(0x90);//Dato
        I2C_Master_Stop();
        __delay_ms(1);
        
        I2C_Master_Start();
        I2C_Master_Write(0xEE);
        I2C_Master_Write(0xFA);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xEF);
        temp1 = I2C_Master_Read(0);
        I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(1);
        
        I2C_Master_Start();
        I2C_Master_Write(0xEE);
        I2C_Master_Write(0xFB);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xEF);
        temp2 = I2C_Master_Read(0);
        I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(1);
        
        I2C_Master_Start();
        I2C_Master_Write(0xEE);
        I2C_Master_Write(0xFC);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xEF);
        temp3 = I2C_Master_Read(0);
        I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(1);
        
        temp = temp1;
        temp = (temp<<8)|temp2;
        temp = (temp<<8)|temp3;
        temp = temp>>4;
        
        PORTB = temp;
        
        I2C_Master_Start();
        I2C_Master_Write(0xEE);
        I2C_Master_Write(0x89);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xEF);
        var1_1 = I2C_Master_Read(0);
        I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(1);
        
        I2C_Master_Start();
        I2C_Master_Write(0xEE);
        I2C_Master_Write(0x88);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xEF);
        var1_2 = I2C_Master_Read(0);
        I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(1);
        
        dig1 = var1_1;
        dig1 = (dig1<<8)|var1_2;
        
        I2C_Master_Start();
        I2C_Master_Write(0xEE);
        I2C_Master_Write(0x8B);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xEF);
        var2_1 = I2C_Master_Read(0);
        I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(1);
        
        I2C_Master_Start();
        I2C_Master_Write(0xEE);
        I2C_Master_Write(0x8A);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xEF);
        var2_2 = I2C_Master_Read(0);
        I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(1);
        
        dig2 = var2_1;
        dig2 = (dig2<<8)|var2_2;
        
        I2C_Master_Start();
        I2C_Master_Write(0xEE);
        I2C_Master_Write(0x8D);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xEF);
        var3_1 = I2C_Master_Read(0);
        I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(1);
        
        I2C_Master_Start();
        I2C_Master_Write(0xEE);
        I2C_Master_Write(0x8C);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xEF);
        var3_2 = I2C_Master_Read(0);
        I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(1);
        
        dig3 = var3_1;
        dig3 = (dig3<<8)|var3_2;
        
        temperatura = calculateTemp(temp, dig1, dig2, dig3)/100;
        
        //Sensor de tiempo
        //Solicitamos segundos RTC
        I2C_Master_Start();     // Start
        I2C_Master_Write(0xd0); // Llamamos a slave RTC
        I2C_Master_Write(0x00); // Pedimos segundos al slave
        __delay_ms(1);
        
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xd1); // Llamamos a slave RTC
        val = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(1);
        
        //Solicitamos minutos RTC
        I2C_Master_Start();     // Start
        I2C_Master_Write(0xd0); // Llamamos a slave RTC
        I2C_Master_Write(0x01); // Pedimos segundos al slave
        __delay_ms(1);
        
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xd1); // Llamamos a slave RTC
        val2 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(1);

         //Solicitamos horas RTC
        I2C_Master_Start();     // Start
        I2C_Master_Write(0xd0); // Llamamos a slave RTC
        I2C_Master_Write(0x02); // Pedimos segundos al slave
        __delay_ms(1);
        
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xd1); // Llamamos a slave RTC
        val3 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(1);

//        TRIG_PIN = 1;
//        __delay_us(10);
//        TRIG_PIN = 0;
//        while(ECHO_PIN == 0);
//        TMR1H = 0x00;
//        TMR1L = 0x00;
//        while(ECHO_PIN == 1);
//        timer_low = TMR1L;
//        timer_high = TMR1H;
//        dis_hc = (((timer_high<<8) + timer_low)*0.032*34.0)/2.0;
//        dis_hc = dis_hc*10;  
//
//        if(dis_hc > 200){
//            PORTBbits.RB6 = 1;
//
//            }
//        else {
//          PORTBbits.RB6 = 0;  
//        }
//        __delay_ms(10);

        
        if (cont == 1){
           TXREG = data1;       
        }
        else if (cont == 2){
           TXREG = data2;     
        }
        else if (cont == 3){
           TXREG = data3;       
        }
        else if (cont == 4){
           TXREG = data4;       
        }
        else if (cont == 5){
           TXREG = temperatura;       
        }
        else if (cont == 6){
           TXREG = val;       
        }
        else if (cont == 7){
           TXREG = val2;       
        }
        else if (cont == 8){
           TXREG = val3;       
        }
        
        
    }
    return;
}

void setup(void) {
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    PORTC = 0;
    TRISCbits.TRISC7 = 1;
    
//    TRISBbits.TRISB6 = 0;
//    TRISBbits.TRISB7 = 0;
//    TRIG_DIR = 0;
//    ECHO_DIR = 1;
//    TRIG_PIN = 0;
//    TMR1H = 255;           
//    TMR1L = 255; 
//    T1CON = 0x31;
//    
    
    init_osc_MHz (1);
    
    I2C_Master_Init(150000);
    
    init_UART();
   
    tmr0_init (7);//INTERRUPCION DEL TMR0 DE 10MS
    tmr0_reload();
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones de perifericos
    PIE1bits.RCIE = 1;          // Habilitamos Interrupciones de recepción
    
    return;
}
