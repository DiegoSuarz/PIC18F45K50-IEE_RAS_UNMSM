/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 20 de julio de 2022, 04:52 PM
 */

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config_fuses.h"
#include "lcd.h"
#define _XTAL_FREQ 16000000UL

float distancia; 
char str_buffer[16]=" ";

void InitUltrasonic(void);
float ReadUltrasonic(void);

void Config_Clock(void);
void Config_Port(void);

void main(void) {
    
    Config_Clock();
    Config_Port();
    
    Lcd_Init();
    Lcd_Clear();
    InitUltrasonic();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("Distancia: ");
    while(1){
        distancia=ReadUltrasonic();
        sprintf(str_buffer,"%.2f cm",distancia);
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(str_buffer);
        __delay_ms(10);
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
   
    //Establecemos como entrada o salida
    TRISD=0b00000000;
}

void InitUltrasonic(void){
    TRISBbits.TRISB6 =1;
    TRISBbits.TRISB7 =0;
    LATBbits.LATB7 =0;
    T0CONbits.TMR0ON=0; // deshabilitar el TMR0
    T0CONbits.T08BIT=0; // TMR0 a 16bits
    T0CONbits.T0CS=0; // FOSC/4
    T0CONbits.PSA=1; // Presscaler deshabilitado
    INTCONbits.TMR0IF=0; //Limpiar bandera del TMR0
    TMR0=0; //cargamos el valor inicial del TMR0
}

float ReadUltrasonic(void){
    float tiempo=0.0;
    LATBbits.LATB7=1;
    __delay_us(10);
    LATBbits.LATB7=0;
    while(!PORTBbits.RB6); //esperamos que el sensor nos envie la data
    TMR0=0;
    T0CONbits.TMR0ON=1; //habilitar TMR0, empieza a contar
    while(PORTBbits.RB6);
    T0CONbits.TMR0ON=0; //deshabilitar TMR0, fin de la data
    tiempo=TMR0;
    /*
    para calcular el tiempo se usa MRU d=vxt
    como solo vamos a considerar el tiempo en el cual
    la onda ultrasonica sea emitida se debe dividir entre 2
    d=(vxt)/2  d=distancia del objeto detectado v=velocidad del sonido t=tiempo del timer(Ttimer)
     la velocidad del sonida v= 343m/s = 34300cm/s
     d(cm)=(34300cm/s x t)/2
     d(cm)=171250cm/s x t
     * 
     * Ftimer=FOSC/4=> Ttimer=16Mhz/4= 4Mhz
     * Ttimer= 1/4Mhz => Ttimer=0.25us  tiempo(t) = TMR0xTtimer => t=TMR0x0.25us
     *  d(cm)=171250cm/s x Ttimer => d(cm)=171250cm/sxTMR0x0.25us
       d(cm)=0.0042875xTMR0   => TMR0/233.236 ->TMR0/233
    */
    return ((float)tiempo/233.23);
}
