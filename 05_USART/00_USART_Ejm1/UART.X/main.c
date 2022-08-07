/*
 * File:   main.c
 * Author: dg_su
 *
 * Created on 6 de agosto de 2022, 02:34 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config_fuses.h"

#define _XTAL_FREQ 16000000UL

void Config_Clock(void);
void Config_Port(void);

void Config_USART(uint32_t baudios);
void printChar_USART(char caracter);
void printString_USART(char *Ptr_string);

void main(void) {
    Config_Clock();
    Config_Port();
    Config_USART(9600);
    
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
    
       
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital
    
       
    //Establecemos como entrada (1) o salida(0)
   
  
}
void Config_USART(uint32_t baudios){
    //Variable n para la generacion de baudios
    uint16_t n=0;
    //Pines de Usart
    ANSELCbits.ANSC6=0;
    ANSELCbits.ANSC7=0;
    TRISCbits.TRISC6=0;//Tx - RC6
    TRISCbits.TRISC7=1;//RX - Rc7
    //Configuracion de registros USART
    TXSTA=0B00100100;
    RCSTA=0B10010000;
    BAUDCON=0B00001000;
    //Generacion de baudios 
    n=(uint16_t)(((_XTAL_FREQ/baudios)/4)-1);
    //Cargar con el valor de n a los registros SPBGH Y 
    SPBRG =n;
    SPBRGH = (n>>8);

}

void printChar_USART(char caracter){
    TXREG=caracter;
    while(TXSTAbits.TRMT==0);//Esperar hasta que se complete la trasmision
}

void printString_USART(char *Ptr_string){
    while(*Ptr_string!='\0'){
        printChar_USART(*Ptr_string);
        Ptr_string++;
    }
}
