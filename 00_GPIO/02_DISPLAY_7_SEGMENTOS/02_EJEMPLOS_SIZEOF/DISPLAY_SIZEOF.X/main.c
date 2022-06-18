/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 17 de junio de 2022, 06:44 PM
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "config_fuses.h"
#define _XTAL_FREQ 16000000UL

#define Digit_0 0xC0
#define Digit_1 0xF9
#define Digit_2 0xA4
#define Digit_3 0xB0
#define Digit_4 0x99
#define Digit_5 0x92
#define Digit_6 0x82
#define Digit_7 0xF8
#define Digit_8 0x80
#define Digit_9 0x90

uint8_t display_anodo[]={Digit_0,Digit_1,Digit_2,Digit_3,Digit_4,Digit_5,Digit_6,Digit_7,Digit_8,Digit_9};
//uint8_t display_anodo[]={0xC0,0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
uint8_t display_catodo[]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};

#define Display_Anodo_size  sizeof(display_anodo)/sizeof(display_anodo[0])
void Imprimir_display(uint8_t array[], uint8_t array_size);

void Config_Clock(void);
void Config_Port(void);

void main(void) {
    Config_Clock();
    Config_Port();
    while(1){
        Imprimir_display(display_anodo,Display_Anodo_size);
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
    //TRISB=0b00000000;
    
    TRISAbits.TRISA0=1; //entrada
    TRISAbits.TRISA1=1; //entrada
        
}

void Imprimir_display(uint8_t array[], uint8_t array_size)
{
    for (uint8_t i=0;i<array_size;i++){
        LATD=array[i];
        __delay_ms(500);
    }
}