/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 17 de junio de 2022, 09:13 PM
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
uint8_t centenas=0;
uint8_t miles=0;

uint16_t cuenta=0;

void Imprimir_display(uint16_t dato);

void Config_Clock(void);
void Config_Port(void);

void main(void) {
    Config_Clock();
    Config_Port();
    while(1){
        if(PORTBbits.RB0==0){
            while(PORTBbits.RB0==0)Imprimir_display(cuenta);
            cuenta++;
            if(cuenta==10000)cuenta=0;
        }
        if(PORTBbits.RB1==1){
            while(PORTBbits.RB1==1)Imprimir_display(cuenta);
            cuenta--;
            if(cuenta==65535)cuenta=0;
        }
        Imprimir_display(cuenta);
        
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
    LATA= 0b00000000;   
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital
    ANSELD=0b00000000;
   
    ANSELBbits.ANSB0=0;
    ANSELBbits.ANSB1=0;
    
    ANSELAbits.ANSA0=0;
    ANSELAbits.ANSA1=0;
    ANSELAbits.ANSA2=0;
    ANSELAbits.ANSA3=0;
    
    //transistores de switcheo
    ANSELEbits.ANSE0=0;
    ANSELEbits.ANSE1=0;
    
    //Establecemos como entrada o salida
    TRISD=0b00000000;
       
    //botones
    TRISBbits.TRISB0=1;
    TRISBbits.TRISB0=1;
    
    //transistores
    TRISAbits.TRISA0=0; //salida
    TRISAbits.TRISA1=0; //salida
    TRISAbits.TRISA2=0; //salida
    TRISAbits.TRISA3=0; //salida
           
}

void Imprimir_display(uint16_t dato)
{
    unidades=dato%10;
    decenas=(dato/10)%10;
    centenas=(dato/100)%10;
    miles=(dato/1000);
    
    //para visualizar unidades:
    LATD=display_anodo[unidades];
    LATAbits.LATA3=0; //habilitar el transistor para visualizar unidades
    __delay_ms(2);
    LATAbits.LATA3=1; //deshabilitar el transistor 

    //para visualizar decenas:
    LATD=display_anodo[decenas];
    LATAbits.LATA2=0; //habilitar el transistor para visualizar decenas
    __delay_ms(2);
    LATAbits.LATA2=1; //deshabilitar el transistor 
    
    //para visualizar centenas:
    LATD=display_anodo[centenas];
    LATAbits.LATA1=0; //habilitar el transistor para visualizar centenas
    __delay_ms(2);
    LATAbits.LATA1=1; //deshabilitar el transistor 

    //para visualizar miles:
    LATD=display_anodo[miles];
    LATAbits.LATA0=0; //habilitar el transistor para visualizar miles
    __delay_ms(2);
    LATAbits.LATA0=1; //deshabilitar el transistor 
    
}