/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 27 de junio de 2022, 03:27 PM
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h> //para imprimir variables en la lcd sprintf()
#include "config_fuses.h"
#include "lcd.h"

#define _XTAL_FREQ 16000000UL

void Config_Clock(void);
void Config_Port(void);

void main(void) {
    Config_Clock();
    Config_Port();
    
    
    while(1){
        if(!PORTBbits.RB0){
            while(!PORTBbits.RB0)LATDbits.LATD0=1;
            
            
        }else{
            LATDbits.LATD0=0;
        }
        
        if(!PORTBbits.RB1){
            while(!PORTBbits.RB1)LATDbits.LATD1=1;
            
        }else{
            LATDbits.LATD1=0;
        }
        
        if(!PORTBbits.RB2){
            while(!PORTBbits.RB2)LATDbits.LATD2=1;
            
        }else{
            LATDbits.LATD2=0;
        }
        
        if(!PORTBbits.RB3){
            while(!PORTBbits.RB3)LATDbits.LATD3=1;
            
        }else{
            LATDbits.LATD3=0;
        }
        
        if(!PORTBbits.RB4){
            while(!PORTBbits.RB4)LATDbits.LATD4=1;
            
        }else{
            LATDbits.LATD4=0;
        }
        
        if(!PORTBbits.RB5){
            while(!PORTBbits.RB5)LATDbits.LATD5=1;
            
        }else{
            LATDbits.LATD5=0;
        }
        
        if(!PORTBbits.RB6){
            while(!PORTBbits.RB6)LATDbits.LATD6=1;
            
        }else{
            LATDbits.LATD6=0;
        }
        
        if(!PORTBbits.RB7){
            while(!PORTBbits.RB7)LATDbits.LATD7=1;
            
        }else{
            LATDbits.LATD7=0;
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
    LATD= 0b00000000;
     
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital
    ANSELD=0b00000000;
    ANSELB=0b00000000;
    
    //Establecemos como entrada o salida
    TRISD=0b00000000;
    
    TRISB = 0xFF; //todo el puerto B como salida
    
    //Configuramos los pull-ups internas
    WPUB = 0x0F; //solo el nibble low tiene activas pull ups internas, el nibble high tiene pull ups externas
    //WPUBbits.WPUB3=1; //rb3 con pull up activa
    //WPUBbits.WPUB4=1; //rb4 con pull up activa
    
    INTCON2bits.NOT_RBPU=0; //habilitar  las pull up del puerto b
}
