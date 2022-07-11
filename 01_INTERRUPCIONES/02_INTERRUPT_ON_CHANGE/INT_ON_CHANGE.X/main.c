/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 8 de julio de 2022, 03:20 PM
 */


#include <xc.h>
#include "config_fuses.h"

void Config_Clock(void);
void Config_Port(void);
void Config_Interrupt_on_Change(void);

void main(void) {
    Config_Clock();
    Config_Port();
    Config_Interrupt_on_Change();
    
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
    LATDbits.LATD7=0;
      
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital(0=digital, 1= analogico)
    ANSELDbits.ANSD7 = 0;
        
    //Establecemos como entrada o salida (0=salida, 1= entrada)
    TRISDbits.TRISD7=0;
  
    
    /*Establemos pull up internas*/
    WPUBbits.WPUB7=1; //rb7 con pull up activa
    WPUBbits.WPUB6=1; //r6 con pull up activa
    INTCON2bits.NOT_RBPU=0; //habilitar  las pull up del puerto b
       
}

void Config_Interrupt_on_Change(void){
    INTCONbits.IOCIF = 0; //borrar la bandera de interrupcion por cambio de estado
    INTCONbits.GIE = 1; //habilitamos las interrupciones globales
    
    //habilitando pines IOC individuales del puerto B (rb7 y rb6)
    IOCBbits.IOCB7 = 1;
    IOCBbits.IOCB6 = 1;

    INTCONbits.IOCIE = 1; //habilitar la interrpcion por cambio de estado
}

void __interrupt() ISR_IOC(){
    if(INTCONbits.IOCIF){
        LATDbits.LATD7=~(LATDbits.LATD7);
        LATB=PORTB;
        INTCONbits.IOCIF=0;
    }
}