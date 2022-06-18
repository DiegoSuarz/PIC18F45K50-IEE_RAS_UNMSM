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

uint8_t display_anodo[]={0xC0,0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
uint8_t display_catodo[]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};

#define Display_Anodo_size  sizeof(display_anodo)/sizeof(display_anodo[0])

uint8_t unidades=0;
uint8_t decenas=0;
uint8_t cuenta=0;

void Imprimir_display(uint8_t array[], uint8_t array_size);

void Config_Clock(void);
void Config_Port(void);

void main(void) {
    Config_Clock();
    Config_Port();
    while(1){
        unidades=cuenta%10;
        decenas=cuenta/10;
        for(uint8_t i=0;i<250;i++){  //4ms x 250 = 1s  el display se visualizara cada segundo
            //para visualizar unidades:
            LATD=display_anodo[unidades];
            LATEbits.LATE1=0; //habilitar el transistor para visualizar unidades
            __delay_ms(2);
            LATEbits.LATE1=1; //deshabilitar el transistor 
            
            //para visualizar decenas:
            LATD=display_anodo[decenas];
            LATEbits.LATE0=0; //habilitar el transistor para visualizar decenas
            __delay_ms(2);
            LATEbits.LATE0=1; //deshabilitar el transistor 
        }
        cuenta++;
        if(cuenta==100)cuenta=0;
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
       
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital
    ANSELD=0b00000000;
   
    ANSELAbits.ANSA0=0;
    ANSELAbits.ANSA1=0;
    
    //transistores de switcheo
    ANSELEbits.ANSE0=0;
    ANSELEbits.ANSE1=0;
    
    //Establecemos como entrada o salida
    TRISD=0b00000000;
       
    //transistores de switcheo
    TRISEbits.TRISE0=0; //salida
    TRISEbits.TRISE1=0; //salida
        
}

void Imprimir_display(uint8_t array[], uint8_t array_size)
{
    for (uint8_t i=0;i<array_size;i++){
        LATD=array[i];
        __delay_ms(500);
    }
}