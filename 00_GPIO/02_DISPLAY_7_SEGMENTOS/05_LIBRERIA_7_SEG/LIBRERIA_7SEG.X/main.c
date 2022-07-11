/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 30 de junio de 2022, 06:24 PM
 */



#include <stdint.h>
#include <stdbool.h>

#include "display_7seg.h"


#define _XTAL_FREQ 16000000UL

char mensaje[]={'h','o','l','a'};

void ejemplo1(void);
void ejemplo2(void);

void Config_Clock(void);
void Config_Port(void);

void main(void) {
    Config_Clock();
    Config_Port();
    while(1){
//        ejemplo1();
        ejemplo2();
    }
    
    return;
}
void Config_Clock(void){
    OSCCONbits.IRCF=0b111; //trabajar a 16Mhz Clock interno
    OSCCONbits.SCS=0b10; //Seleccionar el bloque de oscilador interno
}

void Config_Port(void){
    //limpiar los puertos de salida
    LATD= 0b00000000;
    LATB= 0b00000000;
    
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital
    ANSELD=0b00000000;
    ANSELB=0b00000000;
    
    ANSELAbits.ANSA0=0;
    ANSELAbits.ANSA1=0;
    
    //Establecemos como entrada o salida
    TRISD=0b00000000;
    TRISB=0b00000000;
    
    TRISAbits.TRISA0=1; //entrada
    TRISAbits.TRISA1=1; //entrada
        
}

void ejemplo1(void){
    LATD=Display_anodo('h');
        __delay_ms(500);
        LATD=Display_anodo('o');
        __delay_ms(500);
        LATD=Display_anodo('l');
        __delay_ms(500);
        LATD=Display_anodo('a');
        __delay_ms(500);
        
        LATB=Display_catodo('m');
        __delay_ms(500);
        LATB=Display_catodo('u');
        __delay_ms(500);
        LATB=Display_catodo('n');
        __delay_ms(500);
        LATB=Display_catodo('d');
        __delay_ms(500);
        LATB=Display_catodo('o');
        __delay_ms(500);
}

void ejemplo2(void){
    for(uint8_t i=0;i<sizeof(mensaje);i++){
        LATB=(uint8_t)Display_catodo(mensaje[i]);
        
        __delay_ms(500);
    }
}