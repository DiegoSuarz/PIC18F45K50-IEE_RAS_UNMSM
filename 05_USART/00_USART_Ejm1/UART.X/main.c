/*
 * File:   main.c
 * Author: dg_su
 *
 * Created on 6 de agosto de 2022, 02:34 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config_fuses.h"
#include "USART.h"
#include "ADC_Libreria.h"

#define _XTAL_FREQ 16000000UL

float temp1, temp2, v1;
char LCD_buffer[40];

void Config_Clock(void);
void Config_Port(void);

void main(void) {
    Config_Clock();
    Config_Port();
    ADC_Init();
    USART_init(9600);
    
    while(1){
        temp1=(ADC_Read(_ADC_12)*(VREF/RESOLUCION)*100);
        temp2=((ADC_Read(_ADC_10)*(VREF/RESOLUCION)-0.5)*100);
        v1=ADC_Read(_ADC_8)*(VREF/RESOLUCION);
        printf("\r\rSistema de adquisicion ADC:\r");
        sprintf(LCD_buffer,"LM35: %.2f\r MPC: %.2f\r V1: %.2f",temp1,temp2,v1);
        printf(LCD_buffer);
//        USART_printString(LCD_buffer);
        __delay_ms(1000);
    }
    return;
}


void Config_Clock(void){
    OSCCONbits.IRCF=0b111; //trabajar a 16Mhz Clock interno
    OSCCONbits.SCS=0b10; //Seleccionar el bloque de oscilador interno
}

void Config_Port(void){
    //limpiar los puertos de salida
    
       
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital
    ANSELBbits.ANSB0 = 1; //analogico
    ANSELBbits.ANSB1 = 1;
    ANSELBbits.ANSB2 = 1;
       
    //Establecemos como entrada (1) o salida(0)
    TRISBbits.TRISB0 =1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
  
}
