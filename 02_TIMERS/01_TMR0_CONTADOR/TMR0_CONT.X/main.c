/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 10 de julio de 2022, 12:16 PM
 */

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config_fuses.h"
#include "lcd.h"

#define _XTAL_FREQ 16000000UL

uint8_t str_contador[16];
uint16_t contador=0;

void Config_Clock(void);
void Config_Port(void);
void Config_TMR0_Interrupt(void);

void Config_TMR0_Counter_8bits(void);
void Config_TMR0_Counter_8bits_prescaler(void);
void Config_TMR0_Counter_16bits_prescaler(void);

void main(void) {
    
    Config_Clock();
    Config_Port();
    Lcd_Init();
    Lcd_Clear();
    Config_TMR0_Interrupt();        
    
//    Config_TMR0_Counter_8bits();
//    Config_TMR0_Counter_8bits_prescaler();
    Config_TMR0_Counter_16bits_prescaler();
    
    while(1){
        contador=TMR0;
        sprintf(str_contador,"TMR0: %05u",contador);
        Lcd_Set_Cursor(1,4);
        Lcd_Write_String(str_contador);
        LATB=(uint8_t)contador;
        __delay_ms(50);
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
    LATCbits.LATC0 = 0;
    
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital
    ANSELD=0b00000000;
    ANSELB=0b00000000;   
    
    //Establecemos como entrada o salida
    TRISD=0b00000000;
    TRISB=0b00000000;
    TRISCbits.TRISC0 = 0;
}

void Config_TMR0_Interrupt(void){
    INTCONbits.GIE = 1; //habilitar interrupciones globales
    INTCONbits.PEIE = 1; //habilitar interrupciones de perifericos
    
    INTCONbits.TMR0IF = 0; //limpiar bandera de interrupcion del TMR0
    INTCONbits.TMR0IE = 1; //habilitar interrupcion por desbordamiento del TMR0
        
}

void __interrupt()ISR(){
    if(INTCONbits.TMR0IF){
        INTCONbits.TMR0IF =0;
        LATC ^= (1<<0); //LATCbits.LATC0 = !(LATCbits.LATC0);
        //para timer de 8 bits
//        TMR0 = 250;          //ocurre la interrupcion y vuelve a cargar con el valor de 250
    
        //para timer de 16 bits
        TMR0 = 65530;          //ocurre la interrupcion y vuelve a cargar con el valor de 65530
    }
    
}

void Config_TMR0_Counter_8bits(void){
    T0CONbits.TMR0ON = 1; //habilitar TMR0
    T0CONbits.T08BIT = 1; //TMR0 modo 8 bits
    T0CONbits.T0CS = 1; //modo contador, transicion del pin externo T0CKI
    T0CONbits.T0SE = 0; //imcremento por flanco de subida
    T0CONbits.PSA = 1; //prescaler desactivado
    //T0CONbits.T0PS = 0b111; //prescaler 1:256 (si esta desactivado el prescaler no es necesario configurar)
    
}

void Config_TMR0_Counter_8bits_prescaler(void){
    T0CONbits.TMR0ON = 1; //habilitar TMR0
    T0CONbits.T08BIT = 1; //TMR0 modo 8 bits
    T0CONbits.T0CS = 1; //modo contador, transicion del pin externo T0CKI
    T0CONbits.T0SE = 0; //imcremento por flanco de subida
    T0CONbits.PSA = 0; //prescaler activado
    T0CONbits.T0PS = 0b000; //prescaler 1:2 
    
    TMR0=250;  //para timer de 8 bits
    
}

void Config_TMR0_Counter_16bits_prescaler(void){
    T0CONbits.TMR0ON = 1; //habilitar TMR0
    T0CONbits.T08BIT = 0; //TMR0 modo 16 bits
    T0CONbits.T0CS = 1; //modo contador, transicion del pin externo T0CKI
    T0CONbits.T0SE = 0; //imcremento por flanco de subida
    T0CONbits.PSA = 0; //prescaler activado
    T0CONbits.T0PS = 0b000; //prescaler 1:2 
   
    TMR0=65530;  //para timer de 16 bits
}