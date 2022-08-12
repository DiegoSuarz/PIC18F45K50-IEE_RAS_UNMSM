/*
 * File:   main.c
 * Author: dg_su
 *
 * Created on 11 de agosto de 2022, 10:01 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config_fuses.h"

#include "ADC_Libreria.h"
#include "lcd.h"

#define _XTAL_FREQ 16000000UL

float voltaje;
float LDR;

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
        voltaje = ADC_Read(ADC_0)*(12.0/RESOLUCION); //12 es el nivel de voltaje que hay en mi circuito
        sprintf(str_buffeLcd,"Volt: %.2f",voltaje);
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String(str_buffeLcd);
        
        LDR = ADC_Read(ADC_1)*(VREF/RESOLUCION);
        sprintf(str_buffeLcd,"LDR: %.2f",LDR);
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(str_buffeLcd);
        
        if(LDR > 3.0){
            LATCbits.LATC0 = 1;
        }else{
            LATCbits.LATC0 = 0;
        }
        __delay_ms(100);
        
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
    LATCbits.LATC0 = 0;
    
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital
    ANSELBbits.ANSB0=1; //AN12 como analogico
    ANSELBbits.ANSB1=1; //AN10 como analogico
    ANSELBbits.ANSB2=1; //AN8 como analogico
     
    ANSELD = 0x00; //Puerto D como digital
    
    //Establecemos como entrada (1) o salida(0)
    TRISD=0x00;
    TRISCbits.TRISC0 = 0;
    
    TRISAbits.TRISA0 = 1; //entrada AN12
    TRISAbits.TRISA1 = 1; //entrada AN10


}


