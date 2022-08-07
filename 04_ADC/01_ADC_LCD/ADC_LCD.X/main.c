/*
 * File:   main.c
 * Author: dg_su
 *
 * Created on 6 de agosto de 2022, 08:28 PM
 */

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config_fuses.h"
#include "lcd.h"

#define _XTAL_FREQ 16000000UL

uint16_t dato1=0;
char str_buffeLcd[16];

void Config_Clock(void);
void Config_Port(void);

void Config_ADC(void);

void main(void) {
    Config_Clock();
    Config_Port();
    Lcd_Init();
    Lcd_Clear();
    Config_ADC();
    while(1){
        ADCON0bits.CHS =0b00000; //Seleccionamos AN0 para su conversion
        ADCON0bits.GO_DONE=1; //Inicia la conversion ADC
        while(ADCON0bits.GO_DONE); //Esperar que terminade de convertir el voltaje analogico GO_DONE=0
        dato1=(ADRESH<<8)+(ADRESL);
        sprintf(str_buffeLcd,"ADC: %04d",dato1);
        Lcd_Set_Cursor(1,1);
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
    ANSELD = 0x00; //Puerto D como digital
    
    //Establecemos como entrada (1) o salida(0)
    TRISD=0x00;
    TRISAbits.TRISA0 = 1; //entrada
  
}

void Config_ADC(void){
    
    ADCON1bits.PVCFG = 0b00; // Voltaje positivo de referencia interno AVDD
    ADCON1bits.NVCFG = 0b00; // voltaje negativo de referencia interno AVss
    ADCON2bits.ADFM = 1; //Justificado a la derecha
    ADCON2bits.ACQT = 0b100;  //Tiempo de adqusición de 8 Tad (8us)
    ADCON2bits.ADCS = 0b101; //Seleccionar Fosc/16 = 16Mhz/16 = 1Mhz, tad = 1us
    ADCON0bits.ADON = 1; //habiiltar el modulo ADC
    
}   

        
