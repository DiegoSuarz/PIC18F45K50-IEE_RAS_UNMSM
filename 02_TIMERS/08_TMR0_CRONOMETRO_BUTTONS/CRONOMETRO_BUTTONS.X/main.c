/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 10 de julio de 2022, 07:37 PM
 */

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "config_fuses.h"

#define _XTAL_FREQ 16000000UL

uint8_t display_anodo[]={0xC0,0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

#define Display_Anodo_size  sizeof(display_anodo)/sizeof(display_anodo[0])

uint8_t unidades=0;
uint8_t decenas=0;
uint8_t centenas=0;
uint8_t miles=0;

uint16_t cuenta=0;

bool estado1=false;
bool estado2=false;

void Imprimir_display(uint16_t dato);

void Config_Clock(void);
void Config_Port(void);
void Config_TMRS_Interrupt(void);
void Config_TMR0_Temp(void);
void Config_TMR0_INT0_INT1_Interrupt(void);

void main(void) {
    
    Config_Clock();
    Config_Port();
    
    Config_TMR0_INT0_INT1_Interrupt();
    Config_TMR0_Temp();
    
    
    while(1){
        
        if(estado1){ //iniciar cuenta / stop la cuenta
            T0CONbits.TMR0ON = ~(T0CONbits.TMR0ON); //habilitar TMR0 T0CON ^= (1<<7); 
            TMR0=62411;
            estado1=false;
        }
        if(estado2){  //reset
            cuenta=0;
            estado2=false;
        }
        Imprimir_display(cuenta);
        
    }
    return;
}

void __interrupt() ISR_TMR0(){
    if(INTCONbits.TMR0IF){
        TMR0 = 62411; //carga para 100ms para 16 bits
        cuenta++;
    INTCONbits.TMR0IF=0;
        
    }
    if(INTCONbits.INT0F){
        estado1=true;
        INTCONbits.INT0F=0;
    }
    
    if(INTCON3bits.INT1F){
        estado2=true;
        INTCON3bits.INT1F=0;
    }
    
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
   
    ANSELAbits.ANSA0=0;
    ANSELAbits.ANSA1=0;
    ANSELAbits.ANSA2=0;
    ANSELAbits.ANSA3=0;
    
    ANSELBbits.ANSB0=0;
    ANSELBbits.ANSB1=0;
    //Establecemos como entrada o salida
    TRISD=0b00000000;
       
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


void Config_TMR0_INT0_INT1_Interrupt(void){
    INTCONbits.GIE = 1; //habilitar interrupciones globales
    INTCONbits.PEIE = 1; //habilitar interrupciones de perifericos
    
    //TMR0
    INTCONbits.TMR0IF = 0; //limpiar bandera de interrupcion del TMR0
    INTCONbits.TMR0IE = 1; //habilitar interrupcion por desbordamiento del TMR0
         
    //INT0
    INTCONbits.INT0F = 0; //limpiar bandera de INT0
    INTCONbits.INT0E = 1; //habilitar interrupciones INT0
    INTCON2bits.INTEDG0 = 0; //flanco de bajada
    
    //INT1
    INTCON3bits.INT1F = 0; //limpiar bandera de INT1
    INTCON3bits.INT1E = 1; //habilitar interrupciones INT1
    INTCON2bits.INTEDG1 = 0; //flanco de bajada
    
}

void Config_TMR0_Temp(void){
    //TIMER0
    T0CONbits.TMR0ON = 0; //detener TMR0
    T0CONbits.T08BIT = 0; //TMR0 modo 16 bits
    T0CONbits.T0CS = 0; //modo temporizador, oscilador interno FOSC/4
    //T0CONbits.T0SE = 0; //imcremento por flanco de subida (solo para modo contador)
        
    /*Para 100ms, pree: 128
     * TMR0 = 65536-(tiempo(s)/((4/FOSC) * prescaler)) 
     * TMR0 = 65536-(100x10^-3/((4/16x10^6)*128 ))  
     * TMR0 = 62411
    */
    T0CONbits.PSA = 0; //prescaler desactivado
    T0CONbits.T0PS = 0b110; //prescaler 1:128 (si esta desactivado el prescaler no es necesario configurar)
    TMR0 = 62411; //carga para 100ms
    T0CONbits.TMR0ON = 0; //deshabilitar TMR0
        
}

