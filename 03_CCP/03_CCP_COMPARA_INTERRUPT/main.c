
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config_fuses.h"

#define _XTAL_FREQ 16000000UL
uint8_t flag = 1;

void Config_Clock(void);
void Config_Port(void);
void Config_CCP_Interrupt(void);

void Config_CCP_Compara(void);

void main(void) {
    Config_Clock();
    Config_Port();
    Config_CCP_Compara();
    Config_CCP_Interrupt();
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
    
       
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital
    
       
    //Establecemos como entrada (1) o salida(0)
   
  
}

void Config_CCP_Interrupt(void){
    INTCONbits.GIE=1;
    INTCONbits.PEIE=1;
    PIE1bits.CCP1IE=1;
    PIR1bits.CCP1IF=0;
}

void Config_CCP_Compara(void){
    //Configuración de pines ccp1
    ANSELBbits.ANSB2 = 0; //pin PB2 digital
    TRISCbits.TRISC2 = 0; //pin RC2 como salida 

    //Configuración CCP modo Compara
    CCP2CONbits.DC2B = 0b00; // solo funciona para pwm duty cycle
    CCP1CONbits.CCP1M = 0b0010; //modo captura salida toggle 
    CCPTMRSbits.C1TSEL = 0; //CCP trabaja con TMR1
    
    //Configuracion TMR
    T1CONbits.TMR1CS = 0b00; //clock interno FOSC/4
    T1CONbits.T1CKPS = 0b00; //prescaler 1:1
    T1CONbits.SOSCEN = 0; //oscilador secundario desactivado
    T1CONbits.T1SYNC = 0; //sincronizacion del oscilador externo desactivado
    T1CONbits.RD16 = 1; //TMR1 modo 16bits
    T1CONbits.TMR1ON = 0; //TMR1 deshabilitado
    
    CCPR1=(uint16_t)((1.0/1000)*(_XTAL_FREQ/4)); //toggle a 1khz periodo*Ttimer1 = (1/frec)*Ttimer

    TMR1 = 0; //Caragamos el valor del TMR
    
    //Interrupciones CCP1
    PIR1bits.CCP1IF = 0; //limpiamos bandera
    T1CONbits.TMR1ON = 1; //TMR1 habilitado
  
}

void __interrupt() ISR(){
    
    if(PIR1bits.CCP1IF){
        if(flag==1){
            PIR1bits.CCP1IF=0;
            TMR1=0;
            CCP1CONbits.CCP1M = 0b1000;
            CCPR1=(uint16_t)(((1.0/1000)*(80/100.0))*(_XTAL_FREQ/4)); //80% en alto
            flag++;
        }
        else{
            PIR1bits.CCP1IF=0;
            TMR1=0;
            CCP1CONbits.CCP1M = 0b1001;
            CCPR1=(uint16_t)(((1.0/1000)*(20/100.0))*(_XTAL_FREQ/4)); //20% en bajo
            flag--;
        }
        
    }
    
    
    
}