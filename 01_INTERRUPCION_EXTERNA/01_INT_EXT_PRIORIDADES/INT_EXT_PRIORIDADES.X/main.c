/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 28 de junio de 2022, 02:48 PM
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "config_fuses.h"
#include "lcd.h"

#define _XTAL_FREQ 16000000UL

uint8_t flag=0;

void Config_Clock(void);
void Config_Port(void);
void Config_Interrupt(void);

void main(void) {
    Config_Clock();
    Config_Port();
    Config_Interrupt();
    
    while(1){
        if(flag==1){
            LATD^=(1<<0);
            flag=0;
        }
    }
    return;
}

void Config_Clock(void){
    OSCCONbits.IRCF=0b111; //trabajar a 16Mhz Clock interno
    OSCCONbits.SCS=0b10; //Seleccionar el bloque de oscilador interno
}

void Config_Port(void){
    
    //limpiar los puertos de salida
    LATA=0;
    LATD=0;
    LATCbits.LATC0=0;
    
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital(0=digital, 1= analogico)
    ANSELA = 0;
    ANSELD = 0;
    ANSELBbits.ANSB0=0;
    ANSELBbits.ANSB1=0;
    ANSELBbits.ANSB2=0;
    //Establecemos como entrada o salida (0=salida, 1= entrada)
    TRISA=0;
    TRISD=0;
    TRISCbits.TRISC0=0;
    /*Establemos pull up internas*/
//    WPUBbits.WPUB0=1; //rb0 con pull up activa
//    WPUBbits.WPUB1=1; //rb1 con pull up activa
//    INTCON2bits.NOT_RBPU=0; //habilitar  las pull up del puerto b
       
}

void Config_Interrupt(void){
    /*Configuracion de prioridades*/
    RCON|=(1<<7); //RCONbits.IPEN=1 ,habilitar las interrupciones por prioridad (alta o baja)
    INTCONbits.GIEH = 1; //habilitar interrucpiones de alta prioridad
    INTCONbits.GIEL = 1; //habilitar interrupciones de baja prioridad
    
    /*Configuracion de interrupciones de baja y alta prioridad*/
    //INT0 tiene alta prioridad por defecto, no tiene un bit asociado
    INTCON3bits.INT1IP = 0; //interrupcion externa 1 como baja prioridad
    INTCON3bits.INT2IP = 1; //interrupcion externa 2 como alta prioridad
       
    /*Configuración Interrupcion externa 0 INT0*/
    INTCONbits.INT0E=1; //habilitar interrupcion externa 0
    INTCONbits.INT0F=0; //limpiar bandera de interrupcion INT0
    INTCON2bits.INTEDG0=1; //configurar INT0 por flanco de subida
    
    /*Configuración Interrupcion externa 1 INT1*/
    INTCON3bits.INT1E=1; //habilitar interrupcion externa 1
    INTCON3bits.INT1F=0; //limpiar bandera de interrupcion INT1
    INTCON2bits.INTEDG1=1; //configurar INT1 por flanco de subida 
    
    /*Configuración Interrupcion externa 2 INT2*/
    INTCON3bits.INT2E=1; //habilitar interrupcion externa 2
    INTCON3bits.INT2F=0; //limpiar bandera de interrupcion INT2
    INTCON2bits.INTEDG2=1; //configurar INT2 por flanco de subida 
 }

void __interrupt()ISR(){
    
    if(INTCONbits.INT0F){
        flag=1;
        INTCONbits.INT0F=0;
    }
}