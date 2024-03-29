/*
 * File:   main.c
 * Author: dg_su
 *
 * Created on 13 de agosto de 2022, 06:56 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config_fuses.h"

#define _XTAL_FREQ  16000000
void config_port();
void config_clock();

void USART_Init(uint32_t baudios);
void printChar_USART(char caracter);
void printString_USART(char *Ptr_string);

void MSJCharacter(void);
void MsjString(void);

int main(){
    config_clock();
    USART_Init(9600);
    
    while(1){
        
        
        MsjString();
        __delay_ms(200);
         
        MSJCharacter();
             
    }
    return 0;
}
void config_port(){
    
}
void config_clock(){
    OSCCONbits.IRCF=0b111;
    OSCCONbits.SCS=0b10;
}
void USART_Init(uint32_t baudios){
    //Variable n para la generacion de baudios
    uint16_t n=0;
    //Pines de Usart
    ANSELCbits.ANSC6=0;
    ANSELCbits.ANSC7=0;
    TRISCbits.TRISC6=0;//Tx - RC6
    TRISCbits.TRISC7=1;//RX - Rc7
    //Configuracion de registros USART
    TXSTA=0B00100100;
    RCSTA=0B10010000;
    BAUDCON=0B00001000;
    //Generacion de baudios 
    n=(uint16_t)(((_XTAL_FREQ/baudios)/4)-1);
    //Cargar con el valor de n a los registros SPBGH Y SPBRG
    SPBRG=n;
    SPBRGH=(n>>8);
}
void printChar_USART(char caracter){
    TXREG=caracter;
    while(TXSTAbits.TRMT==0);//Esperar hasta que se complete la trasmision
}
void printString_USART(char *Ptr_string){
    while(*Ptr_string!='\0'){
        printChar_USART(*Ptr_string);
        Ptr_string++;
    }
}
//Funcion Adicional para enciar datos por uSART con la funcion printf() - stdio.h
void putch(char c){// la funcion putch me va a dirigir los datos a printf()
    TXREG = c;
    while(TXSTAbits.TRMT==0);
}

void MSJCharacter(void){
    //Enviamos caracteres
    printChar_USART('P');
    printChar_USART('I');
    printChar_USART('C');
    printChar_USART('1');
    printChar_USART('8');
    printChar_USART('F');
    printChar_USART('4');
    printChar_USART('5');
    printChar_USART('K');
    printChar_USART('5');
    printChar_USART('0');
    printChar_USART('\r');
}

void MsjString(void){
    //enviamos cadenas
    //printString_USART("Curso PIC RAS UNMSM\r");
    
    //enviamos cadenas con printf
    printf("Curso PIC RAS UNMSM\r");
    
}