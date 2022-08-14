/*
 * File:   main.c
 * Author: dg_su
 *
 * Created on 12 de agosto de 2022, 08:41 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config_fuses.h"

uint8_t Array_DAC[]={0x10,0x13,0x15,0x18,0x1a,0x1c,0x1e,0x1f,0x1f,0x1f,
0x1e,0x1c,0x1a,0x18,0x15,0x13,0x10,0xc,0xa,0x7,
0x5,0x3,0x1,0x0,0x0,0x0,0x1,0x3,0x5,0x7,
0xa,0xc};
#define array_size (sizeof(Array_DAC)/sizeof(Array_DAC[0]))

#define _XTAL_FREQ 16000000UL

void DAC_Init(void);
void Volt_DAC(uint8_t voltaje);

void DienteSierra(void);
void SignalSquare(void);
void SignalTriangle(void);
void SignalSeno(void);

void Config_Clock(void);
void Config_Port(void);

void main(void) {
    Config_Clock();
    Config_Port();
   
   
    DAC_Init();
    while(1){
//        SignalSquare();
//        DienteSierra();
//        SignalTriangle();
        SignalSeno();
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
       
    ANSELD = 0x00; //Puerto D como digital
    
    //Establecemos como entrada (1) o salida(0)
    
}

void DAC_Init(void){
    
    //Configurar el pin de salida
    ANSELAbits.ANSA2 = 0; //pin mode digital
    //TRISAbits.TRISA2 = 0; //salida dac
    
    //Configurar DAC
    VREFCON1bits.DACEN = 1; //habilitar el DAC
    VREFCON1bits.DACLPS = 0; //Fuente de baja potencia desactivada
    VREFCON1bits.DACOE = 1; //habiiltar la salida de voltaje por el pin fisico DACOUT
    VREFCON1bits.DACPSS = 0b00; //Seleccionar la fuente de voltaje poditivo VDD para el DAC
    VREFCON1bits.DACNSS =0; //Seleccionar la fuente de voltaje negativo VSS para el DAC
}

void Volt_DAC(uint8_t voltaje){ //El DAC tiene una resolución de 15 bits (0 - 31)
    VREFCON2 = (voltaje & 0x1F);
}

void SignalSquare(void){
    Volt_DAC(31);
    __delay_ms(500);
    Volt_DAC(0);
    __delay_ms(500);
}

void DienteSierra(void){
    for(uint8_t i=0;i<32;i++){
        Volt_DAC(i);
        __delay_ms(1);
    }
    
}

void SignalTriangle(void){
      for(uint8_t i=0;i<32;i++){
        Volt_DAC(i);
        __delay_ms(1);
    }
        for(uint8_t j=30;j>0;j--){
        Volt_DAC(j);
        __delay_ms(1);
    }
}

void SignalSeno(void){
    for(uint8_t i=0;i<array_size;i++){
        Volt_DAC(Array_DAC[i]);
        __delay_ms(1);
    }
    
}