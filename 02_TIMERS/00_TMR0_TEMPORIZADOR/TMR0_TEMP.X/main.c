/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 10 de julio de 2022, 11:37 AM
 */

#include <xc.h>
#include <stdint.h>
#include "config_fuses.h"
#define _XTAL_FREQ 16000000UL

void Config_Clock(void);
void Config_Port(void);
void Config_TMR0_Interrupt(void);

void Config_TMR0_8bits_Temporizador(void);
void Config_TMR0_16bits_Temporizador(void);

void main(void) {
    
    Config_Clock();
    Config_Port();
    Config_TMR0_Interrupt();
    
//    Config_TMR0_8bits_Temporizador();
    Config_TMR0_16bits_Temporizador();
    while(1){
       
    }
    return;
}

void Config_Clock(void){
    OSCCONbits.IRCF=0b111; //trabajar a 16Mhz Clock interno
    OSCCONbits.SCS=0b10; //Seleccionar el bloque de oscilador interno
}

void Config_Port(void){
    //limpiar los puertos de salida
    LATCbits.LATC0= 0;
     
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital
    ANSELD=0b00000000;
        
    //Establecemos como entrada o salida
    TRISCbits.TRISC0=0;
    
}

void Config_TMR0_Interrupt(void){
    INTCONbits.GIE = 1; //habilitar interrupciones globales
    INTCONbits.PEIE = 1; //habilitar interrupciones de perifericos
    
    INTCONbits.TMR0IF = 0; //limpiar bandera de interrupcion del TMR0
    INTCONbits.TMR0IE = 1; //habilitar interrupcion por desbordamiento del TMR0
        
}

void __interrupt() ISR_TMR0(){
    if(INTCONbits.TMR0IF){
        INTCONbits.TMR0IF=0;
        LATC^=(1<<0);
//        TMR0 = 131;  //carga para 1ms para 8 bits
        
        TMR0 = 62411;  //carga para 100ms para 16 bits
    }
    
}

void Config_TMR0_8bits_Temporizador(void){
    T0CONbits.TMR0ON = 0; //detener TMR0
    T0CONbits.T08BIT = 1; //TMR0 modo 8 bits
    T0CONbits.T0CS = 0; //modo temporizador, oscilador interno FOSC/4
    //T0CONbits.T0SE = 0; //imcremento por flanco de subida (solo para modo contador)
    T0CONbits.PSA = 0; //prescaler desactivado
    T0CONbits.T0PS = 0b100; //prescaler 1:32 (si esta desactivado el prescaler no es necesario configurar)
    
    /*
     * Formula de TMR0:
     * para 8 bits:
     * TMR0 = 256-(tiempo(s)/(TCY * prescaler))  
     * TMR0 = 256-(tiempo(s)/((4/FOSC) * prescaler))
     *Ejm= tiempo = 1ms      FOSC= 16MHz    prescaler=32
     *  
     * TMR0 = 256-(1x10^-3/((4/16x10^6)* 32 ))  
     * TMR0 = 131
    */
    TMR0 = 131; //carga para 1ms
    T0CONbits.TMR0ON = 1; //habilitar TMR0
}

void Config_TMR0_16bits_Temporizador(void){
    T0CONbits.TMR0ON = 0; //detener TMR0
    T0CONbits.T08BIT = 0; //TMR0 modo 16 bits
    T0CONbits.T0CS = 0; //modo temporizador, oscilador interno FOSC/4
    //T0CONbits.T0SE = 0; //imcremento por flanco de subida (solo para modo contador)
    
    /*
     * Formula de TMR0:
     * para 16 bits:
     * TMR0 = 65536-(tiempo(s)/(TCY * prescaler))  
     * TMR0 = 65536-(tiempo(s)/((4/FOSC) * prescaler))
     *Ejm= tiempo = 100ms      FOSC= 16MHz    prescaler=128
     *  
     * TMR0 = 65536-(100x10^-3/((4/16x10^6)* 128 ))  
     * TMR0 = 62411
    */
    
    T0CONbits.PSA = 0; //prescaler activado
    T0CONbits.T0PS = 0b110; //prescaler 1:128 (si esta desactivado el prescaler no es necesario configurar)
    TMR0 = 62411; //carga para 1ms
    T0CONbits.TMR0ON = 1; //habilitar TMR0
}