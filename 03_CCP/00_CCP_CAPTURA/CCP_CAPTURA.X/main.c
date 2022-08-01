/*
 * File:   main.c
 * Author: dg_su
 *
 * Created on 31 de julio de 2022, 06:26 PM
 */

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config_fuses.h"
#include "lcd.h"
#define _XTAL_FREQ 16000000UL

char str_buffer[20];
float frecuency=0.0;

void Config_Clock(void);
void Config_Port(void);
void Config_CCP_Captura(void);
float Read_Captura(void);

void main(void) {
    Config_Clock();
    Config_Port();
    Config_CCP_Captura();
    Lcd_Init();
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("Frecuencia:");
    while(1){
        frecuency = Read_Captura();
        sprintf(str_buffer,"%.2f",frecuency);
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(str_buffer);
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
    LATD = 0x00;
       
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital
    ANSELD = 0x00;
       
    //Establecemos como entrada (1) o salida(0)
    TRISD = 0x00;
  
}

void Config_CCP_Captura(void){
    //Configuracion de pin CCP1, funciona para TMR1 y TMR3
    ANSELCbits.ANSC2 = 0; //pin modo digital
    TRISCbits.TRISC2 = 1; //entrada
    
    //Configuración de CCP1
    CCP1CONbits.DC1B = 0b00; //solo funcionan para  modo PWM
    CCP1CONbits.CCP1M = 0b0101; //modo captura por flanco de subida
    CCPTMRSbits.C1TSEL = 0; //modo captura trabaja con TMR1

    //Configuracion TMR
    T1CONbits.TMR1CS = 0b00; //clock interno FOSC/4
    T1CONbits.T1CKPS = 0b00; //prescaler 1:1
    T1CONbits.SOSCEN = 0; //oscilador secundario desactivado
    T1CONbits.T1SYNC = 0; //sincronizacion del oscilador externo desactivado
    T1CONbits.RD16 = 1; //TMR1 modo 16bits
    T1CONbits.TMR1ON = 0; //TMR1 deshabilitado
    
    CCPR1 = 0; //cargamos el valor del registro CCPR1 en 0
    TMR1 = 0; //Caragamos el valor del TMR
    
    //Interrupciones CCP1
    PIR1bits.CCP1IF = 0; //limpiamos bandera
    T1CONbits.TMR1ON = 1; //TMR1 habilitado
        
}   

float Read_Captura(void){
    float frecuencia;
    float periodo=0;
    uint16_t flanco_inicial=0;
    uint16_t flanco_final=0;
    
    //TMR0=0;
    //T1CONbits.TMR1ON = 1; //TMR1 habilitado
    while(!PIR1bits.CCP1IF); //esperar a que ocurra la captura del primer flanco de subida(interrupcion)
    PIR1bits.CCP1IF=0;
    flanco_inicial = CCPR1;
    
    while(!PIR1bits.CCP1IF); //esperar a que ocurra la captura del segundo flanco de subida(interrupcion)
    PIR1bits.CCP1IF=0;
    flanco_final = CCPR1;
    
//    T1CONbits.TMR1ON = 0;
//    periodo =(flanco_final - flanco_inicial)/4000000.0; //obteniendo el periodo de la señal, tiempo de timer 0.25us 1/(FOSC/4) -> 1/0.25us= 4000000 
     periodo =(flanco_final - flanco_inicial)/(float)(_XTAL_FREQ/4); // 1/(_XTAL_FREQ/4) es el tiempo que tarda en incrementarse el TMR1
    frecuencia=1/periodo;
    return frecuencia;
}