/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 14 de junio de 2022, 05:40 PM
 */
/*
 Z = !7 --> z=(7!=0)=1 (verdadero)
 Z = !0 --> Z=(0!=0)=0 (falso)

 */


#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "config_fuses.h"

#define _XTAL_FREQ 16000000UL
uint8_t contador=0;
uint8_t i=0;
bool estado=false;

void Config_Clock(void);
void Config_Port(void);

bool digitalRead(uint8_t pin, uint8_t registro);
bool digitalRead1(uint8_t pin, uint8_t registro);

void ejemplo1(void);
void ejemplo2(void);
void ejemplo3(void);
void ejemplo4(void);
void ejemplo5(void);
void ejemplo6_funcionArduino(void);

void main(void) {
    Config_Clock();
    Config_Port();
    while(1)
    {
//       ejemplo1();
//       ejemplo2();
//       ejemplo3();
//       ejemplo4();
//       ejemplo5();
       ejemplo6_funcionArduino();
    }
    
    return;
}

void Config_Clock(void){
    OSCCON=0b01110010;
}

void Config_Port(void){
    //limpiar los puertos de salida
    LATD= 0b00000000;
    LATCbits.LATC0=0;
    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital
    ANSELD=0b00000000;
    ANSELAbits.ANSA0=0;
    ANSELAbits.ANSA1=0;
    
    //Establecemos como entrada o salida
    TRISD=0b00000000;
    TRISAbits.TRISA0=1; //entrada
    TRISAbits.TRISA1=1; //entrada
    TRISCbits.TRISC0=0; //salida, no esta asociado a un pin analogico
    
}

void ejemplo1(void)
{
    if(PORTAbits.RA0==1)
    {
        LATD=0xFF;
    }else{
        LATD=0x00;
    }
}

void ejemplo2(void)
{
    while(PORTAbits.RA0==1)
    {
        LATD=0xFF;
    }
    LATD=0x00;
}

void ejemplo3(void)
{
    if(PORTAbits.RA0==1)
    {
        while(PORTAbits.RA0==1); //antirebote
        __delay_ms(20); //antirebote
        contador++;
        if(contador==10)contador=0;
        LATD = contador;
    }
    
    if(PORTAbits.RA1==0)
    {
        while(PORTAbits.RA1==0); //antirebote
        __delay_ms(20); //antirebote
        contador--;
        if(contador==255)contador=0;
        LATD = contador;
        
    }
}

void ejemplo4(void)
{
    //usando mascaras
    //PULL DOWN
    if(PORTA & (1<<0))  //tambien  if((PORTA & (1<<0))==1)
    {
        while(PORTA & (1<<0)); //antirebote
        __delay_ms(20); //antirebote
        contador++;
        if(contador==10)contador=0;
        LATD = contador;
    }
    
    //PULL UP
    if(!(PORTA & (1<<1))) //tambien:  if((PORTA & (1<<1))==0)
    {
        while(!(PORTA & (1<<1))); //antirebote
        __delay_ms(20); //antirebote
        contador--;
        if(contador==255)contador=0;
        LATD = contador;
        
    }
}

void ejemplo5(void)
{
    if(PORTAbits.RA0 ==1)
    {
        while(PORTAbits.RA0 ==1)LATCbits.LATC0=1;
        contador++;
        LATCbits.LATC0=0;
    }
    
    if(PORTAbits.RA1==0)
    {
        while(PORTAbits.RA1 ==0);
        estado=true;
    }
    
    while(estado){
        
        while(i<contador){
            LATD=0xFF;
            __delay_ms(300);
            LATD=0x00;
            __delay_ms(300);
            i++;
        }
        estado=false;
        i=0;
        contador=0;
    }
}

void ejemplo6_funcionArduino(void)
{
    if(digitalRead1(0,PORTA)==1){ //pull down
        while(digitalRead(0,PORTA)==1); //antirebote
        __delay_ms(20); //antirebote
        contador++;
        if(contador==255)contador=0;
        
    }
    
    if(digitalRead1(1,PORTA)==0){ //pull up
        while(digitalRead1(1,PORTA)==0); //antirebote
        __delay_ms(20); //antirebote
        contador--;
        if(contador==255)contador=0;
        
    }
    LATD = contador;
}

bool digitalRead(uint8_t pin, uint8_t registro){
    if(registro & (1<<pin)){
        return 1;
    }else{
        return 0;
    }
}

bool digitalRead1(uint8_t pin, uint8_t registro){ 
    return registro & (1<<pin);
}
