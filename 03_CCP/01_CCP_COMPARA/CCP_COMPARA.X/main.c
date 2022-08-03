/*
 * File:   main.c
 * Author: dg_su
 *
 * Created on 2 de agosto de 2022, 04:21 PM
 */

//CCP COMPARACIÖN TMR1
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config_fuses.h"

#define _XTAL_FREQ 16000000UL

void Config_Clock(void);
void Config_Port(void);

void Config_CCP_Compara(void);

void main(void) {
    Config_Clock();
    Config_Port();
    Config_CCP_Compara();
    while(1){
        while(!PIR1bits.CCP1IF);
        PIR1bits.CCP1IF=0;
        TMR1=0;
        
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
    
       
    //Establecemos como entrada (1) o salida(0)
   
  
}

void Config_CCP_Compara(void){
    //Configuración de pines ccp
    ANSELBbits.ANSB2 = 0; //pin PB2 digital
    TRISCbits.TRISC2 = 0; //pin RC2 como salida 

    //Configuración CCP modo Compara
    CCP2CONbits.DC2B = 0b00; // solo funciona para pwm duty cycle
    CCP1CONbits.CCP1M = 0b0010; //modo captura salida toggle 
    CCPTMRSbits.C1TSEL = 0; //CCP trabaja con TMR1
    
    //Configuracion TMR
    T1CONbits.TMR1CS = 0b00; //clock interno FOSC/4
    T1CONbits.T1CKPS = 0b00; //prescaler 1:1
    T1CONbits.SOSCEN = 0; //oscilador secundario desactivado
    T1CONbits.T1SYNC = 0; //sincronizacion del oscilador externo desactivado
    T1CONbits.RD16 = 1; //TMR1 modo 16bits
    T1CONbits.TMR1ON = 0; //TMR1 deshabilitado
    
    /*
     Para generar una señal cuadrada de 5khz con un duty cycle de 50%
     frecuencia = 5khz
     periodo = 200us  -> 100us en algo, 100us en bajo
     f timer1 = FOSC/4xPree = (16x10^6/4x1) = 4x10^6
     CCPRx = tiempoDeseado x f timer1
     CCPRx = 100x10^-6 x 4x10^6
     CCPR = 400
     
    */
    CCPR1 = 400; //cargamos el valor del registro CCPR1
    TMR1 = 0; //Caragamos el valor del TMR
    
    //Interrupciones CCP1
    PIR1bits.CCP1IF = 0; //limpiamos bandera
    T1CONbits.TMR1ON = 1; //TMR1 habilitado
  
}
