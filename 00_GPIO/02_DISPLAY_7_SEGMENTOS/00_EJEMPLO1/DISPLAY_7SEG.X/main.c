/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 16 de junio de 2022, 04:35 PM
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "config_fuses.h"
#define _XTAL_FREQ 16000000UL

uint8_t display_anodo[]={0xC0,0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
uint8_t display_catodo[]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};

void Ejm1_anodo(void);
void Ejm2_catodo(void);
void Ejm3_bucle(void);
void Ejm4_bucle(void);
void Ejm5_pulsadores(void);
void Ejm6_pulsadores_nivelBits(void);

void Config_Clock(void);
void Config_Port(void);

void main(void) {
    
    Config_Clock();
    Config_Port();
    
    while(1){
//       Ejm1_anodo();
//       Ejm2_catodo();
//       Ejm3_bucle();
//       Ejm4_bucle();
        Ejm5_pulsadores();
        Ejm6_pulsadores_nivelBits();
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
    
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital
    ANSELD=0b00000000;
    ANSELB=0b00000000;
    
    ANSELAbits.ANSA0=0;
    ANSELAbits.ANSA1=0;
    
    //Establecemos como entrada o salida
    TRISD=0b00000000;
    TRISB=0b00000000;
    
    TRISAbits.TRISA0=1; //entrada
    TRISAbits.TRISA1=1; //entrada
        
}

void Ejm1_anodo(void)
{
     //Display anodo comun
        LATD=0b11000000; //0
        __delay_ms(500);
        LATD=0b11111001; //1
        __delay_ms(500);
        LATD=0b10100100; //2
        __delay_ms(500);
        LATD=0b10110000; //3
        __delay_ms(500);
        LATD=0b10011001; //4
        __delay_ms(500);
        LATD=0b10010010; //5
        __delay_ms(500);
        LATD=0b10000010; //6
        __delay_ms(500);
        LATD=0b11111000; //7
        __delay_ms(500);
        LATD=0b10000000; //8
        __delay_ms(500);
        LATD=0b10010000; //9
        __delay_ms(500);
}

void Ejm2_catodo(void)
{
    //Display catodo comun
        LATB=0b00111111; //0
        __delay_ms(500);
        LATB=0b00000110; //1
        __delay_ms(500);
        LATB=0b01011011; //2
        __delay_ms(500);
        LATB=0b01001111; //3
        __delay_ms(500);
        LATB=0b01100110; //4
        __delay_ms(500);
        LATB=0b01101101; //5
        __delay_ms(500);
        LATB=0b01111101; //6
        __delay_ms(500);
        LATB=0b00000111; //7
        __delay_ms(500);
        LATB=0b01111111; //8
        __delay_ms(500);
        LATB=0b01101111; //9
        __delay_ms(500);
}

void Ejm3_bucle(void)
{
    LATD = display_anodo[0];
    __delay_ms(500);
    LATD = display_anodo[1];
    __delay_ms(500);
    LATD = display_anodo[2];
    __delay_ms(500);
    LATD = display_anodo[3];
    __delay_ms(500);
    LATD = display_anodo[4];
    __delay_ms(500);
    LATD = display_anodo[5];
    __delay_ms(500);
    LATD = display_anodo[6];
    __delay_ms(500);
    LATD = display_anodo[7];
    __delay_ms(500);
    LATD = display_anodo[8];
    __delay_ms(500);
    LATD = display_anodo[9];
    __delay_ms(500);
}

void Ejm4_bucle(void)
{
    uint8_t val=0;
    while(1){
        LATB=display_catodo[val];
        __delay_ms(500);
        val++;
        if(val==10){
            val=0;
        }
    }
}

void Ejm5_pulsadores(void)
{
    uint8_t unidades=0, decenas=0;
    
    LATB=display_catodo[0];
    LATD=display_anodo[0];
    while(1){
        
        //aumentar cuenta
        if(PORTAbits.RA0==0)//!PORTAbits.RA0 (pull up)
        {
            while(!PORTAbits.RA0);
            unidades++;
            if(unidades==10)
            {
                unidades=0;
                decenas++;
                if(decenas==10)decenas=0;
            }
        }
                
        //decrementar cuenta
        if(PORTAbits.RA1==1)//!PORTAbits.RA0 (pull down)
        {
            while(PORTAbits.RA1);
            unidades--;
            if((decenas==0) && (unidades==255))
            {
                unidades=0;
            }
            if((decenas>0) && (unidades==255))
            {
                unidades=9;
                decenas--;
            }
        }
        LATB=display_catodo[unidades];
        LATD=display_anodo[decenas];
    }
}

void Ejm6_pulsadores_nivelBits(void)
{
        
    uint8_t unidades=0, decenas=0;
    //aumentar cuenta
    if(!(PORTA & (1<<0))) //pull up) if((PORTA & (1<<0))==0)(
    {
        while(!(PORTA & (1<<0)));
        unidades++;
        if(unidades==10)
        {
            unidades=0;
            decenas++;
            if(decenas==10)decenas=0;
        }
    }

    //decrementar cuenta
    if(PORTA & (1<<1))//(pull down) if((PORTA & (1<<1))!=0), if((PORTA & (1<<1))==1)
    {
        while(PORTA & (1<<1));
        unidades--;
        if((decenas==0) && (unidades==255))
        {
            unidades=0;
        }
        if((decenas>0) && (unidades==255))
        {
            unidades=9;
            decenas--;
        }
    }
    LATB=display_catodo[unidades];
    LATD=display_anodo[decenas];
    }

