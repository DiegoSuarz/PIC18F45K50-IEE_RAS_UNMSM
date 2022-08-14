/*
 * File:   main.c
 * Author: dg_su
 *
 * Created on 13 de agosto de 2022, 08:29 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config_fuses.h"
#include "USART.h"

#define _XTAL_FREQ 16000000UL

#define LAMP1 LATCbits.LATC0
#define LAMP2 LATCbits.LATC1
#define LAMP3 LATCbits.LATC2

#define APAGADO     0
#define ENCENDIDO   1

char dato_rx;

void Config_Clock(void);
void Config_Port(void);

char getChar_USART(void);

void main(void) {
    Config_Clock();
    Config_Port();
    USART_init(9600);
    
    while(1){
        dato_rx = USART_getChar();
        if(dato_rx == 'q'){
            USART_printString("\rLampara 1 encendida\r");
            LAMP1 = ENCENDIDO;
            
        }
        else if(dato_rx == 'w'){
            USART_printString("\rLampara 2 encendida\r");
            LAMP2 = ENCENDIDO;
        }
        else if(dato_rx == 'e'){
            USART_printString("\rLampara 3 encendida\r");
            LAMP3 = ENCENDIDO;
        }
        else if(dato_rx == 'a'){
            USART_printString("\rLampara 1 apagado\r");
            LAMP1 = APAGADO;
        }
        else if(dato_rx == 's'){
            USART_printString("\rLampara 2 apagado\r");
            LAMP2 = APAGADO;
        }
        else if(dato_rx == 'd'){
            USART_printString("\rLampara 3 apagado\r");
            LAMP3 = APAGADO;
        }
        else if(dato_rx == 'r'){
            USART_printString("\rTodas las lamparas encendidas\r");
            LAMP1 = ENCENDIDO;
            LAMP2 = ENCENDIDO;
            LAMP3 = ENCENDIDO;
        }
        else if(dato_rx == 'f'){
            USART_printString("\rTodas las lamparas apagadas\r");
            LAMP1 = APAGADO;
            LAMP2 = APAGADO;
            LAMP3 = APAGADO;
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
    LATCbits.LATC0;
    LATCbits.LATC1;
    LATCbits.LATC2;
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital
          
    //Establecemos como entrada (1) o salida(0)
    TRISCbits.TRISC0=0;
    TRISCbits.TRISC1=0;
    TRISCbits.TRISC2=0;
  
}