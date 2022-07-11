/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 10 de julio de 2022, 06:15 PM
 */

#include <xc.h>
#include <stdint.h>
#include "config_fuses.h"
#define _XTAL_FREQ 16000000UL

uint16_t cuenta1=0;
uint16_t cuenta2=0;
void Config_Clock(void);
void Config_Port(void);

void Config_TMR1_TMR3_Counter(void);

void main(void) {
    
    Config_Clock();
    Config_Port();
    
    Config_TMR1_TMR3_Counter();

    while(1){
        cuenta1=TMR1;
        cuenta2=TMR3;
        LATD=(uint8_t)cuenta1;
        LATA=(uint8_t)cuenta2;
    }
    return;
}

void Config_Clock(void){
    OSCCONbits.IRCF=0b111; //trabajar a 16Mhz Clock interno
    OSCCONbits.SCS=0b10; //Seleccionar el bloque de oscilador interno
}

void Config_Port(void){
    //limpiar los puertos de salida
    LATD = 0x00;
    LATA = 0x00;
       
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital
    ANSELD = 0x00;
    ANSELA = 0x00;
    
    //Establecemos como entrada (1) o salida(0)
    TRISD = 0x00;
    TRISA = 0x00;
}

void Config_TMR1_TMR3_Counter(void){
    
    //TIMER1
    T1CONbits.TMR1ON = 0; //deshabilitar TMR0
    T1CONbits.TMR1CS = 0b10; //modo contador, fuente pulsos externo
    T1CONbits.SOSCEN = 0; //deshabilitamos el oscilador secundario
    T1CONbits.T1SYNC =0; //deshabilitamos la sincronizacion del temporizador secundario
    T1CONbits.RD16 = 1; //TMR1 modo 16bits
    T1CONbits.T1CKPS = 0b00; //prescaler de 1:1 (deshabilitado)
    T1CONbits.TMR1ON = 1; //habilitar TMR0
    
    //TIMER3 (para el pin externo T3CK1 se debe configurar los fuses )
    T3CONbits.TMR3ON = 0; //deshabilitar TMR0
    T3CONbits.TMR3CS = 0b10; //modo contador, fuente pulsos externo
    T3CONbits.SOSCEN = 0; //deshabilitamos el oscilador secundario
    T3CONbits.T3SYNC =0; //deshabilitamos la sincronizacion del temporizador secundario
    T3CONbits.RD16 = 1; //TMR1 modo 16bits
    T3CONbits.T3CKPS = 0b00; //prescaler de 1:1 (deshabilitado)
    T3CONbits.TMR3ON = 1; //habilitar TMR0
    
}