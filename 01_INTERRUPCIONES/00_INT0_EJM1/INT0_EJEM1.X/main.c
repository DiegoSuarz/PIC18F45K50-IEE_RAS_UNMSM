/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 27 de junio de 2022, 04:55 PM
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "config_fuses.h"
#include "lcd.h"

#define _XTAL_FREQ 16000000UL

#define PASE_AUTOS      0b00001100
#define AMBAR           0b00010010
#define PASE_PERSONAS   0b00100001

void Config_Clock(void);
void Config_Port(void);
void Config_Interrupt(void);

uint8_t autos,personas;
char str_autos[16];
char str_personas[16];
void main(void) {
    Config_Clock();
    Config_Port();
    Config_Interrupt();
    Lcd_Init();
    Lcd_Clear();
    sprintf(str_autos,"Autos:%03d",autos);
    sprintf(str_personas,"Personas:%03d",personas);
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String(str_autos);
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String(str_personas);
    while(1){
        LATA = PASE_AUTOS;
        __delay_ms(3000);
        
        LATA = AMBAR;
        __delay_ms(3000);
        
         LATA = PASE_PERSONAS;
        __delay_ms(3000);
        
         LATA = AMBAR;
        __delay_ms(3000);
    }
    return;
}

void Config_Clock(void){
    OSCCONbits.IRCF=0b111; //trabajar a 16Mhz Clock interno
    OSCCONbits.SCS=0b10; //Seleccionar el bloque de oscilador interno
}

void Config_Port(void){
    
    //limpiar los puertos de salida
    LATA= 0b00000000;
    LATD= 0x00; 
    LATEbits.LATE0=0;
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital(0=digital, 1= analogico)
    ANSELAbits.ANSA0=0;
    ANSELAbits.ANSA1=0;
    ANSELAbits.ANSA2=0;
    ANSELAbits.ANSA5=0;
    
    ANSELBbits.ANSB0=0;
    ANSELBbits.ANSB1=0;
    ANSELBbits.ANSB2=0;
    
    ANSELEbits.ANSE0=0;
    
    ANSELD = 0x00;
    
    //Establecemos como entrada o salida
    TRISA=0x00;
    TRISD=0x00;
    TRISEbits.RE0=0;
    
    TRISBbits.RB0=1;
    TRISBbits.RB1=1;
    TRISBbits.RB2=1;
    
    /*Establemos pull up internas*/
    WPUBbits.WPUB0=1; //rb0 con pull up activa
    WPUBbits.WPUB1=1; //rb1 con pull up activa
    INTCON2bits.NOT_RBPU=0; //habilitar  las pull up del puerto b
       
}

void Config_Interrupt(void){
    
    INTCONbits.GIE=1; //habilitar las interrupciones globales
    INTCONbits.PEIE=1; //deshabilitar las interrupciones por perifericos
    
    /*Configuración Interrupcion externa 0 INT0*/
    INTCONbits.INT0E=1; //habilitar interrupcion externa 0
    INTCONbits.INT0F=0; //limpiar bandera de interrupcion INT0
    INTCON2bits.INTEDG0=0; //configurar INT0 por flanco de bajada (1 =  flanco de subida)
    
    /*Configuración Interrupcion externa 1 INT1*/
    INTCON3bits.INT1E=1; //habilitar interrupcion externa 1
    INTCON3bits.INT1F=0; //limpiar bandera de interrupcion INT1
    INTCON2bits.INTEDG1=0; //configurar INT1 por flanco de bajada 
    
    /*Configuración Interrupcion externa 2 INT2*/
    INTCON3bits.INT2E=1; //habilitar interrupcion externa 2
    INTCON3bits.INT2F=0; //limpiar bandera de interrupcion INT2
    INTCON2bits.INTEDG2=0; //configurar INT2 por flanco de bajada 
 }

void __interrupt()ISR_INT0(){
    
    if(INTCONbits.INT0F){
        autos++;
        INTCONbits.INT0F=0; //limpiar bandera de interrupcion
    }
    if(INTCON3bits.INT1F == 1){
        personas++;
        INTCON3bits.INT1F=0;
    }
    
    if(INTCON3bits.INT2F){
        LATE^=(1<<0);
        INTCON3bits.INT2F=0;
    }
    
    sprintf(str_autos,"Autos:%03d",autos);
    sprintf(str_personas,"Personas:%03d",personas);
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String(str_autos);
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String(str_personas);
    
}