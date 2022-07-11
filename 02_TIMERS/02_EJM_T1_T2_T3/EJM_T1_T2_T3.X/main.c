/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 10 de julio de 2022, 03:58 PM
 */

#include <xc.h>
#include <stdint.h>
#include "config_fuses.h"
#define _XTAL_FREQ 16000000UL

void Config_Clock(void);
void Config_Port(void);
void Config_TMRS_Interrupt(void);


void Config_TMR0_TMR1_TMR3(void);

void main(void) {
    
    Config_Clock();
    Config_Port();
    Config_TMRS_Interrupt();
    Config_TMR0_TMR1_TMR3();

    while(1){
       
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
    LATE &= ~(1<<0);
    LATE &= ~(1<<1);
    LATE &= ~(1<<2);
   
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital
    ANSELE &= ~(1<<0);
    ANSELE &= ~(1<<1);
    ANSELE &= ~(1<<2);
    ANSELD = 0x00;
    ANSELA = 0x00;
    
    //Establecemos como entrada (1) o salida(0)
    TRISE &= ~(1<<0);
    TRISE &= ~(1<<1);
    TRISE &= ~(1<<2);
    TRISD = 0x00;
    TRISA = 0x00;
}

void Config_TMRS_Interrupt(void){
    INTCONbits.GIE = 1; //habilitar interrupciones globales
    INTCONbits.PEIE = 1; //habilitar interrupciones de perifericos
    
    //TMR0
    INTCONbits.TMR0IF = 0; //limpiar bandera de interrupcion del TMR0
    INTCONbits.TMR0IE = 1; //habilitar interrupcion por desbordamiento del TMR0
        
    //TMR1
    PIR1bits.TMR1IF = 0; //limpiar bandera de interrupcion del TMR1
    PIE1bits.TMR1IE = 1; //habilitar interrupcion por desbordamiento del TMR1
   
    //TMR3
    PIR2bits.TMR3IF = 0; //limpiar bandera de interrupcion del TMR3
    PIE2bits.TMR3IE = 1; //habilitar interrupcion por desbordamiento del TMR3
   
    
}

void __interrupt() ISR_TMRS(){
    if(INTCONbits.TMR0IF){
        INTCONbits.TMR0IF=0;
        LATE^=(1<<0);
        TMR0 = 53036;  //carga para 200ms para 16 bits
    }
    
    if(PIR1bits.TMR1IF){
        PIR1bits.TMR1IF=0;
        LATE^=(1<<1);
        TMR1 = 15536;  //carga para 100ms para 16 bits
        LATD++;
    }
       
    if(PIR2bits.TMR3IF){
        PIR2bits.TMR3IF =0;
        LATE^=(1<<2);
        LATA++;
        TMR3 = 40536;  //carga para 50ms para 16 bits
    }
}

void Config_TMR0_TMR1_TMR3(void){
    //TIMER0
    T0CONbits.TMR0ON = 0; //detener TMR0
    T0CONbits.T08BIT = 0; //TMR0 modo 16 bits
    T0CONbits.T0CS = 0; //modo temporizador, oscilador interno FOSC/4
    //T0CONbits.T0SE = 0; //imcremento por flanco de subida (solo para modo contador)
        
    /*Para 200ms, pree: 64
     * TMR0 = 65536-(tiempo(s)/((4/FOSC) * prescaler)) 
     * TMR0 = 65536-(200x10^-3/((4/16x10^6)*64 ))  
     * TMR0 = 53036
    */
    T0CONbits.PSA = 0; //prescaler desactivado
    T0CONbits.T0PS = 0b101; //prescaler 1:64 (si esta desactivado el prescaler no es necesario configurar)
    TMR0 = 53036; //carga para 200ms
    T0CONbits.TMR0ON = 1; //habilitar TMR0
    
    //TIMER1
    T1CONbits.TMR1ON = 0; //detener TMR1
    T1CONbits.TMR1CS = 0b00; //modo temporizador, fuente FOSC/4
    T1CONbits.SOSCEN = 0; //deshabilitamos el oscilador secundario
    T1CONbits.T1SYNC =0; //deshabilitamos la sincronizacion del temporizador secundario
    T1CONbits.RD16 = 0; //habilitar la escritura/lectura del registro TMR1
    /*Para 100ms, pree: 64
     * TMR0 = 65536-(tiempo(s)/((4/FOSC) * prescaler)) 
     * TMR0 = 65536-(100x10^-3/((4/16x10^6)*64 ))  
     * TMR0 = 15336
    */
    T1CONbits.T1CKPS = 0b11; //prescaler de 1:8
    TMR1 = 15336;
    T1CONbits.TMR1ON = 1; //habilitar TMR0
    
    
    //TIMER3
    T3CONbits.TMR3ON = 0; //detener TMR3
    T3CONbits.TMR3CS = 0b00; //modo temporizador, fuente FOSC/4
    T3CONbits.SOSCEN = 0; //deshabilitamos el oscilador secundario
    T3CONbits.T3SYNC =0; //deshabilitamos la sincronizacion del temporizador secundario
    T3CONbits.RD16 = 0; //habilitar la escritura/lectura del registro TMR1
    /*Para 100ms, pree: 64
     * TMR0 = 65536-(tiempo(s)/((4/FOSC) * prescaler)) 
     * TMR0 = 65536-(50x10^-3/((4/16x10^6)*64 ))  
     * TMR0 = 40536
    */
    T3CONbits.T3CKPS = 0b11; //prescaler de 1:8
    TMR3 = 40536;
    T3CONbits.TMR3ON = 1; //habilitar TMR0
    
}