/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 17 de junio de 2022, 09:39 PM
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "config_fuses.h"
#include "lcd.h"
#define _XTAL_FREQ 16000000UL

void Config_Clock(void);
void Config_Port(void);

void main(void) {
    Config_Clock();
    Config_Port();
    Lcd_Init();
    Lcd_Clear();
    
    while(1){
        Lcd_Set_Cursor(1,4);
        Lcd_Write_Char('R');
        Lcd_Write_Char('A');
        Lcd_Write_Char('S');
        Lcd_Write_Char(' ');
        Lcd_Write_Char('U');
        Lcd_Write_Char('N');
        Lcd_Write_Char('M');
        Lcd_Write_Char('S');
        Lcd_Write_Char('M');
        
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("curso uC PIC");
    }
    return;
}

void Config_Clock(void){
    OSCCONbits.IRCF=0b111; //trabajar a 16Mhz Clock interno
    OSCCONbits.SCS=0b10; //Seleccionar el bloque de oscilador interno
}

void Config_Port(void){
    //limpiar los puertos de salida
    LATD= 0b00000000;
     
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital
    ANSELD=0b00000000;
   
    //Establecemos como entrada o salida
    TRISD=0b00000000;
       
}