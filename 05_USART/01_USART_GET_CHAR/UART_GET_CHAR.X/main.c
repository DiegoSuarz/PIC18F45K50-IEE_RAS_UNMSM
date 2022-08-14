/*
 * File:   main.c
 * Author: dg_su
 *
 * Created on 13 de agosto de 2022, 08:12 PM
 */



#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config_fuses.h"
#include "USART.h"



#define _XTAL_FREQ 16000000UL


char dato_rx;

void Config_Clock(void);
void Config_Port(void);

char getChar_USART(void);

void main(void) {
    Config_Clock();
    Config_Port();
    USART_init(9600);
    
    while(1){
        dato_rx = getChar_USART();
        USART_printChar('\r');
        USART_printChar(dato_rx);
        USART_printChar('\r');
        
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

char getChar_USART(void){
    while(PIR1bits.RCIF==0);//Esperar hasta que se reciba 1 dato por UART
    //Evaluar el desbordamiento
    if(RCSTAbits.OERR==1){
        RCSTAbits.CREN=0;//Deshabilito la receocion continua
        NOP(); //Realizo un retardo de 1 ciclo de instruccion
        RCSTAbits.CREN=1;//Habilito la recepcion continua
    }
    return (RCREG);//Devuelvo el caracter recibo en el registro RCREG
}
