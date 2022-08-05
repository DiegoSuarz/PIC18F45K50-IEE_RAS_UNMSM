/*
 * File:   main.c
 * Author: dg_su
 *
 * Created on 4 de agosto de 2022, 08:41 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config_fuses.h"

#define _XTAL_FREQ 16000000UL

void Config_Clock(void);
void Config_Port(void);
void Config_CCP_Interrupt(void);

void Config_CCP_PWM(void);
void change_dutyCycle(void);

void main(void) {
    Config_Clock();
    Config_Port();
    Config_CCP_PWM();
    while(1){
       change_dutyCycle();
        
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
    
       
    //Establecemos como entrada (1) o salida(0)
   
  
}

void Config_CCP_PWM(void){
     //Configuración de pines ccp1
    ANSELCbits.ANSC2 = 0; //pin PC2 digital
    TRISCbits.TRISC2 = 0; //pin RC2 como salida 
       
    //Configuramos CCP1 en modo PWM
    CCP1CONbits.CCP1M = 0b1100; //modo pwm
    
    //cargamos el valor del registro PR2, para el periodo total del PWM
    /*
     PR2=(FOSC/(Fpwm x 4 x TRM2 pree)-1)
     Para la frecuencia del PWM a 10 KHz (Periodo de 100us)-> Fpwm = 10x10^3 , preescaler = 4
     PR2 = (16x10^6/(10x10^3 x 4 x 4) -1) = 99
    */
    PR2 = 99; //para un periodo de 1ms -> 10Khz
    
    /*
    Para un duty cycle del 20% 
     CCPR1L:CCP1CON<5:4> = 4(PR2+1)x(Duty cycle)
     CCPR1L:CCP1CON<5:4> = 4(99+1)x(20/100) = 80 
     80 -> bin:  0001010000(10 bits) -> CCPR1L = 00010100, :CCP1CON<5:4> = 00
    */
    CCP1CONbits.DC1B = 0b00; //bits Msb del duty cycle
    CCPR1L = 0b00010100; // 8 bits LSM del duty cycle
        
    //configuración de TMR2 con preescaler =4:
    T2CONbits.T2CKPS = 0b01; //prescaler 4
    T2CONbits.TMR2ON = 0; //TMR2 apagado
    TMR2 = 0;
    T2CONbits.TMR2ON=1; //encender TMR2
    
    
    
}

void change_dutyCycle(void){
    for(uint16_t dutyCycle=0;dutyCycle<1000;dutyCycle++){
        CCPR1L=dutyCycle>>2;
        CCP1CON |= ((dutyCycle & 0x0003)<<4); 
        __delay_ms(10);
    }
}