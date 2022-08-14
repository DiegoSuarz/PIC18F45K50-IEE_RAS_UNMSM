/*
 * File:   main.c
 * Author: dg_su
 *
 * Created on 13 de agosto de 2022, 09:36 PM
 */

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config_fuses.h"
#include "USART.h"
#include "ADC_Libreria.h"
#include "lcd.h"
#define _XTAL_FREQ  16000000

#define L1  LATCbits.LATC0
#define L2  LATCbits.LATC1
#define L3  LATCbits.LATC2
#define APAGADO     0
#define ENCENDIDO   1

float temp_lm35=0.0,temp_mcp=0.0,v1=0.0; 
char Str_lm35[8],Str_mcp[8],Str_v1[8];
char lcd_Buffer[20];
char dato_Rx;

void config_port();
void config_clock();
void config_interrup_usart();
void config_interrup_usart_timer0();
void Init_Cargas();
void Enviar_Data_UART();
void config_TMR0_16bits_Temporizador();
int main(){
    config_clock();
    config_port();
    USART_init(9600);
    ADC_Init();
    Lcd_Init();
    Lcd_Clear();
    Init_Cargas();
    config_interrup_usart_timer0();
    config_TMR0_16bits_Temporizador();
    while(1){
        temp_lm35=((ADC_Read(_ADC_0)*(VREF/RESOLUCION))*100);
        temp_mcp=(((ADC_Read(_ADC_1)*(VREF/RESOLUCION))-0.5)*100);
        v1=(ADC_Read(_ADC_2)*(VREF/RESOLUCION));
        
        sprintf(Str_lm35,"%.2f",temp_lm35);
        sprintf(Str_mcp,"%.2f",temp_mcp);
        sprintf(Str_v1,"%.2f",v1);
        
 
    }
    return 0;
}
void __interrupt(high_priority) ISR_H(){
    if(INTCONbits.TMR0IF==1){
        INTCONbits.TMR0IF=0;
        TMR0H=0x85;// TMR0=34286
        TMR0L=0xEE;
        Enviar_Data_UART();
    }
}
void __interrupt(low_priority) ISR_L(){
    if(PIR1bits.RCIF==1){
        PIR1bits.RCIF=0;
        dato_Rx=RCREG;
        
        //APLICACION//////////////////////////////////////////////////
        if(dato_Rx == 'q'){
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("L1:ON ");
            L1=ENCENDIDO;
        }
        else if(dato_Rx == 'w'){
            Lcd_Set_Cursor(1,8);
            Lcd_Write_String("L2:ON ");
            L2=ENCENDIDO;
        }
        else if(dato_Rx == 'e'){
            Lcd_Set_Cursor(2,5);
            Lcd_Write_String("L3:ON ");
            L3=ENCENDIDO;
        }
        else if(dato_Rx == 'a'){
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("L1:OFF");
            L1=APAGADO;
        }
        else if(dato_Rx == 's'){
            Lcd_Set_Cursor(1,8);
            Lcd_Write_String("L2:OFF");
            L2=APAGADO;
        }
        else if(dato_Rx == 'd'){
            Lcd_Set_Cursor(2,5);
            Lcd_Write_String("L3:OFF");
            L3=APAGADO;
        }
        else if(dato_Rx == 'r'){
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("L1:ON ");
            Lcd_Set_Cursor(1,8);
            Lcd_Write_String("L2:ON ");
            Lcd_Set_Cursor(2,5);
            Lcd_Write_String("L3:ON ");
            L1=ENCENDIDO;
            L2=ENCENDIDO;
            L3=ENCENDIDO;
        }
        else if(dato_Rx == 'f'){
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("L1:OFF");
            Lcd_Set_Cursor(1,8);
            Lcd_Write_String("L2:OFF");
            Lcd_Set_Cursor(2,5);
            Lcd_Write_String("L3:OFF");
            L1=APAGADO;
            L2=APAGADO;
            L3=APAGADO;
        }
        //////////////////////////////////////////////////////////
    }
    
}
void config_interrup_usart_timer0(){
    //Asignacion de prioridades
    RCONbits.IPEN=1;//Habilito las interrupciones por prioridades
    // Habilitamos interrupciones de alta y baja prioridad
    INTCONbits.GIEH=1;//Habilito las interrupciones de ALTA PRIORIDAD
    INTCONbits.GIEL=1;//Habilito las interrupciones de BAJA PRIORIDAD
    //ASIGNAMOS PRIORIDADES A LOS PERIFERICOS
    INTCON2bits.TMR0IP=1;//Interrupcion del TIMER 0 con ALTA PRIORIDAD
    IPR1bits.RCIP=0; //Interrupcion de RX con BAJA PRIORIDAD
    //HABILITAMOS LAS INTERRUPCIONES DE LOS PERIFERICOS A USAR Y LIMPIAMOS SUS FLAG
    PIE1bits.RC1IE=1;//Habilito la interrupción por recepción en RX
    PIR1bits.RCIF=0;//Limpiamos la bandera de RX
    INTCONbits.TMR0IE=1;//Habilito la interrupción del timer 0
    INTCONbits.TMR0IF=0;//Limpiamos la bandera del timer 0
}
void Init_Cargas(){
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("L1:OFF");
     Lcd_Set_Cursor(1,8);
    Lcd_Write_String("L2:OFF");
     Lcd_Set_Cursor(2,5);
    Lcd_Write_String("L3:OFF");
}
void Enviar_Data_UART(){
    //aplicacion
    USART_printString("\r\r\rSistema de adquisición de datos: ");
    USART_printString("\rTemp LM35: ");
    USART_printString(Str_lm35);
    USART_printString("\rTemp MCP: ");
    USART_printString(Str_mcp);
    USART_printString("\rVoltaje: ");
    USART_printString(Str_v1);
}
void config_TMR0_16bits_Temporizador(){
    T0CONbits.TMR0ON=0;
    T0CONbits.T08BIT=0;
    T0CONbits.T0CS=0;
    T0CONbits.PSA=0;
    T0CONbits.T0PS=0b100;//Prescaler de 1:4
    TMR0H=0x85;// TMR0=34286
    TMR0L=0xEE;
    T0CONbits.TMR0ON=1;
}
void config_port(){
    /*LIMPIMOS LOS PUERTOS*/
    L1=0;
    L2=0;
    L3=0;
    /*PINES COMO DIGITALES Y ANALOGICOS*/
    ANSELCbits.ANSC2=0;
    ANSELAbits.ANSA0=1;
    ANSELAbits.ANSA1=1;
    ANSELAbits.ANSA2=1;
    ANSELD=0X00;
    /*PINES COMO ENTYRADAS Y SALIDAS*/
    TRISCbits.TRISC0=0;
    TRISCbits.TRISC1=0;
    TRISCbits.TRISC2=0;
    
    TRISAbits.TRISA0=1;
    TRISAbits.TRISA1=1;
    TRISAbits.TRISA1=1;
    
    TRISD=0X00;
}
void config_clock(){
    OSCCONbits.IRCF=0b111;
    OSCCONbits.SCS=0b10;
}
