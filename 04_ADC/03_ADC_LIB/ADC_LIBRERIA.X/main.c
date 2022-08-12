/*
 * File:   main.c
 * Author: dg_su
 *
 * Created on 10 de agosto de 2022, 09:48 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config_fuses.h"
#include "lcd.h"
#include "ADC_Libreria.h"

#define _XTAL_FREQ 16000000UL

uint16_t dato1=0, dato2;
float voltaje1, voltaje2;
char str_buffeLcd[16];

void Config_Clock(void);
void Config_Port(void);

void main(void) {
    Config_Clock();
    Config_Port();
    Lcd_Init();
    Lcd_Clear();
    ADC_Init();
    while(1){
        
        dato1=ADC_Read(ADC_0);
        voltaje1=dato1*(5.0/1024);
        sprintf(str_buffeLcd,"AN0: %04d %0.2fv",dato1,voltaje1);
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String(str_buffeLcd);
        __delay_ms(10);
       
        
        dato2=ADC_Read(ADC_1);
        voltaje2=dato2*(VREF/RESOLUCION);
        sprintf(str_buffeLcd,"AN1: %04d %0.2fv",dato2,voltaje2);
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(str_buffeLcd);
        __delay_ms(10);
       
        
    }
    return;
}


void Config_Clock(void){
    OSCCONbits.IRCF=0b111; //trabajar a 16Mhz Clock interno
    OSCCONbits.SCS=0b10; //Seleccionar el bloque de oscilador interno
}

void Config_Port(void){
    //limpiar los puertos de salida
    LATD=0x00;
       
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital
    ANSELAbits.ANSA0=1; //AN0 como analogico
    ANSELAbits.ANSA1=1; //AN1 como analogico
    
    ANSELD = 0x00; //Puerto D como digital
    
    //Establecemos como entrada (1) o salida(0)
    TRISD=0x00;
    
    TRISAbits.TRISA0 = 1; //entrada AN0
    TRISAbits.TRISA1 = 1; //entrada AN1
  
}

