/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 10 de julio de 2022, 06:49 PM
 */

#include <xc.h>
#include <stdint.h>
#include "config_fuses.h"
#define _XTAL_FREQ 16000000UL

uint8_t contador=0;
uint8_t flag=0;
void Config_Clock(void);
void Config_Port(void);
void Config_TMRS_Interrupt(void);


void Config_TMR0_Temp(void);

void main(void) {
    
    Config_Clock();
    Config_Port();
    Config_TMRS_Interrupt();
    Config_TMR0_Temp();

    while(1){
        if(flag){
            contador++;
            if(contador==15)contador=0;
            LATD=contador;
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
    LATD = 0x00;
       
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital
    ANSELD = 0x00;
       
    //Establecemos como entrada (1) o salida(0)
    TRISD = 0x00;
  
}

void Config_TMRS_Interrupt(void){
    INTCONbits.GIE = 1; //habilitar interrupciones globales
    INTCONbits.PEIE = 1; //habilitar interrupciones de perifericos
    
    //TMR0
    INTCONbits.TMR0IF = 0; //limpiar bandera de interrupcion del TMR0
    INTCONbits.TMR0IE = 1; //habilitar interrupcion por desbordamiento del TMR0
         
}

void __interrupt() ISR_TMRS(){
    if(INTCONbits.TMR0IF){
        TMR0 = 3036; //carga para 500ms para 16 bits
        INTCONbits.TMR0IF=0;
        flag=1;
        
    }
    
}

void Config_TMR0_Temp(void){
    //TIMER0
    T0CONbits.TMR0ON = 0; //detener TMR0
    T0CONbits.T08BIT = 0; //TMR0 modo 16 bits
    T0CONbits.T0CS = 0; //modo temporizador, oscilador interno FOSC/4
    //T0CONbits.T0SE = 0; //imcremento por flanco de subida (solo para modo contador)
        
    /*Para 500ms, pree: 64
     * TMR0 = 65536-(tiempo(s)/((4/FOSC) * prescaler)) 
     * TMR0 = 65536-(500x10^-3/((4/16x10^6)*32 ))  
     * TMR0 = 3036
    */
    T0CONbits.PSA = 0; //prescaler desactivado
    T0CONbits.T0PS = 0b100; //prescaler 1:32 (si esta desactivado el prescaler no es necesario configurar)
    TMR0 = 3036; //carga para 500ms
    T0CONbits.TMR0ON = 1; //habilitar TMR0
        
}