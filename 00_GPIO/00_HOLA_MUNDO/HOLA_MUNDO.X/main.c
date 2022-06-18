
#include <xc.h>
#include <math.h>
#include "config_fuses.h"
#include <stdint.h>

#define _XTAL_FREQ  16000000

void ejemplo1(void);
void ejemplo2(void);
void ejemplo3(void);
void ejemplo4(void);
void ejemplo5(void);

//variables globales:
int8_t j=0;
int main(void) {
    //Formato binario
    OSCCON = 0b01110010; //Seleccionamos frecuencia interna de 16Mhz del oscilador interno
    LATD = 0b00000000; //Limpiamos el puerto D
    SLRCON = 0b00000000; //velocidad Starndart del puerto D
    ANSELD = 0b00000000; //Todos los pines del puerto D como digitales
    TRISD = 0b00000000; //Establecemos el puerto D como salida digital
    
//    //Formato hexadecimal
//    LATD = 0x00; //Limpiamos el puerto D
//    SLRCON = 0x00; //velocidad Starndart del puerto D
//    ANSELD = 0x00; //Todos los pines del puerto D como digitales
//    TRISD = 0x00; //Establecemos el puerto D como salida digital
    
//    //Formato decimal
//    LATD = 0; //Limpiamos el puerto D
//    SLRCON = 0; //velocidad Starndart del puerto D
//    ANSELD = 0; //Todos los pines del puerto D como digitales
//    TRISD = 0; //Establecemos el puerto D como salida digital

//Configurar bit a bit usando estructuras al pin B0
//    LATBbits.LATB0 = 0;
//    LATBbits.LATB1 = 0;
//    SLRCONbits.SLRB = 0;
//    ANSELBbits.ANSB0 = 0;
//    ANSELBbits.ANSB1 = 0;
//    TRISBbits.TRISB0 = 0;
//    TRISBbits.TRISB1 = 0;

//Configurar el pin B0 y B1 a nivel de bits
    LATB&= ~(1<<0)&~(1<<1);
    SLRCON&=~(1<<2);
    ANSELB&=~(1<<0);
    ANSELB&=~(1<<1);
    TRISB&=~((1<<0)|(1<<1));
    
    while(1){
       ejemplo5();
    }
    return 0;
}

void ejemplo1(void)
{
    LATD= 0xFF;
    __delay_ms(500);
    LATD = 0b00110011;
    __delay_ms(500);
    LATB |= (1<<0)|(1<<1);
    __delay_ms(500);
    LATB &= ~((1<<0)|(1<<1));
    LATD = 0;
     __delay_ms(500);
}

void ejemplo2(void)
{
    LATDbits.LATD0 = 1;
    __delay_ms(200);
    LATDbits.LATD1 = 1;
    __delay_ms(200);
    LATDbits.LATD2 = 1;
    __delay_ms(200);
    LATDbits.LATD3 = 1;
    __delay_ms(200);
    LATDbits.LATD4 = 1;
    __delay_ms(200);
    LATDbits.LATD5 = 1;
    __delay_ms(200);
    LATDbits.LATD6 = 1;
    __delay_ms(200);
    LATDbits.LATD7 = 1;
    __delay_ms(200);
    
    //ciclo descendente
    LATDbits.LATD7 = 0;
    __delay_ms(200);
    LATDbits.LATD6 = 0;
    __delay_ms(200);
    LATDbits.LATD5 = 0;
    __delay_ms(200);
    LATDbits.LATD4 = 0;
    __delay_ms(200);
    LATDbits.LATD3 = 0;
    __delay_ms(200);
    LATDbits.LATD2 = 0;
    __delay_ms(200);
    LATDbits.LATD1 = 0;
    __delay_ms(200);
    LATDbits.LATD0 = 0;
    __delay_ms(200);
}

void ejemplo3(void)
{
    for(int i=0;i<=8;i++){
        LATD=(uint8_t) pow(2,i)-1;
        __delay_ms(200);
    }
    
    for(int i=7;i>0;i--){
        LATD= (uint8_t) pow(2,i)-1;
        __delay_ms(200);
    }
        
}

void ejemplo4(void)
{
    while(j<8){
        LATD|=(1<<j);
        __delay_ms(300);
        j++;
    }
    j--;
    while(j>=0){
        LATD&=~(1<<j);
        __delay_ms(300);
        j--;
    }
}

void ejemplo5(void)
{
    LATD=0b00000001;
    while(j<7){
        __delay_ms(200);
        LATD =(uint8_t)(LATD<<1);
        j++;
    }
    j--;
    while(j>=0){
        __delay_ms(200);
        LATD = (uint8_t)(LATD>>1);
        j--;
    }
    j=0;
}