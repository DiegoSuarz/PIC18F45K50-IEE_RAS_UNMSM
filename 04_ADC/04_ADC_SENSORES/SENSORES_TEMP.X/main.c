/*
 * File:   main.c
 * Author: dg_su
 *
 * Created on 10 de agosto de 2022, 10:37 PM
 */

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config_fuses.h"
#include "lcd.h"
#include "ADC_Libreria.h"

#define _XTAL_FREQ 16000000UL

float LM35 = 0.0, MCP9700A=0.0, LDR=0.0;

char str_buffeLcd[20];

void Config_Clock(void);
void Config_Port(void);

void main(void) {
    Config_Clock();
    Config_Port();
    Lcd_Init();
    Lcd_Clear();
    ADC_Init();
    while(1){
        
        LM35=(ADC_Read(_ADC_12)*(VREF/RESOLUCION)*100);
        MCP9700A=((ADC_Read(_ADC_10)*(VREF/RESOLUCION)-0.5)*100);
        LDR=ADC_Read(_ADC_8)*(VREF/RESOLUCION);
        

        sprintf(str_buffeLcd,"LM:%.1f",LM35);
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String(str_buffeLcd);
        
        sprintf(str_buffeLcd,"MCP:%.1f",MCP9700A);
        Lcd_Set_Cursor(1,9);
        Lcd_Write_String(str_buffeLcd);

        sprintf(str_buffeLcd,"LDR:%.1f",LDR);
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
    ANSELBbits.ANSB0=1; //AN12 como analogico
    ANSELBbits.ANSB1=1; //AN10 como analogico
    ANSELBbits.ANSB2=1; //AN8 como analogico
     
    ANSELD = 0x00; //Puerto D como digital
    
    //Establecemos como entrada (1) o salida(0)
    TRISD=0x00;
    
    TRISBbits.TRISB0 = 1; //entrada AN12
    TRISBbits.TRISB1 = 1; //entrada AN10
    TRISBbits.TRISB2 = 1; //entrada AN8
}


