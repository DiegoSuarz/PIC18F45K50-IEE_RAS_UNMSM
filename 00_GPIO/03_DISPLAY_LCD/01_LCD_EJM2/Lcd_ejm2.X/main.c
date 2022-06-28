/*
 * File:   main.c
 * Author: Diego
 *
 * Created on 27 de junio de 2022, 11:44 AM
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h> //para imprimir variables en la lcd sprintf()
#include "config_fuses.h"
#include "lcd.h"

#define _XTAL_FREQ 16000000UL



void ejemplo1(void);
void ejemplo2(void);
void ejemplo3(void);
void ejemplo4(void);
void ejemplo5(void);

void Config_Clock(void);
void Config_Port(void);

void main(void) {
    Config_Clock();
    Config_Port();
    Lcd_Init();
    Lcd_Clear();
    
    while(1){
//        ejemplo1();
//        ejemplo2();
//        ejemplo3();
//        ejemplo4();
        ejemplo5();
        
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
    ANSELAbits.ANSA0=0;
    ANSELBbits.ANSB1=0;
    
    //Establecemos como entrada o salida
    TRISD=0b00000000;
    
    TRISAbits.TRISA0=1;
    TRISBbits.TRISB1=1;
    
    //Configuramos los pull-ups internas
    WPUBbits.WPUB1=1; //rb1 con pull up activa
    INTCON2bits.NOT_RBPU=0; //habilitar  las pull up del puerto b
}

void ejemplo1(void){
    //desplazamiento de la pantalla LCD;
    for(uint8_t i=1;i<17;i++){
        Lcd_Set_Cursor(1,i);
        Lcd_Write_String("RAS UNMSM");
        __delay_ms(200);
        Lcd_Clear();
    }
    for(uint8_t i=17 ;i>0;i--){
        Lcd_Set_Cursor(1,i);
        Lcd_Write_String("RAS UNMSM");
        __delay_ms(200);
        Lcd_Clear();
    }
}

void ejemplo2(void){
    uint8_t numero=235;
    float temp1= 22.5;
    float pi=3.1416;
    uint8_t num_primo=5;
    
    
    char str_mensaje[]={"hola"};
    char str_numero[4];
    char str_temp1[4];
    char str_pi[7];
    char str_num_primo[2];
    //sprintf(cadena donde se guarda la conversion; tipo de dato o formato que se va a convertir; variable a convertir);
    sprintf(str_numero,"n:04%d",numero);
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String(str_numero);
    
    sprintf(str_numero,"n:%u",numero);
    Lcd_Set_Cursor(1,7);
    Lcd_Write_String(str_numero);
    
//    Lcd_Set_Cursor(1,13);
//    Lcd_Write_String(str_mensaje);
    
    sprintf(str_temp1,"t:%02.1f",temp1);
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String(str_temp1);
    
    sprintf(str_pi,"pi:%.4f",pi);
    Lcd_Set_Cursor(2,8);
    Lcd_Write_String(str_pi);
    
    sprintf(str_num_primo,"p:02%d",num_primo);
    Lcd_Set_Cursor(1,13);
    Lcd_Write_String(str_num_primo);
}

void ejemplo3(void){
    
    uint8_t numero=235;
    float temp1= 22.5;
    float pi=3.1416;
    uint8_t num_primo=5;
        
    char str_dato[17];
    char str_dato2[17];
    
    sprintf(str_dato,"n=%04d pi=%2.1f",numero, pi);
    sprintf(str_dato2,"n=%04d T=%3.1f",num_primo, temp1);
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String(str_dato);
     Lcd_Set_Cursor(2,1);
    Lcd_Write_String(str_dato2);
}

void ejemplo4(void){
    
    uint8_t contador=0;
    char str_contador[10]; 
    for(uint8_t i=0;i<200;i++){
        sprintf(str_contador,"Cont:%03d",i);
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String(str_contador);
        __delay_ms(100);
    }
}

void ejemplo5(void){
    uint8_t contador=0;
    char str_contador[17];  
    while(1){
        
        if(!PORTAbits.RA0){
            while(!PORTAbits.RA0);
            contador++;
            if(contador==15){
                contador=0;
            }
        }

        if(!PORTBbits.RB1){
            while(!PORTBbits.RB1);
            contador--;
            if(contador==255){
                contador=0;
            }
        }

        sprintf(str_contador,"Cont:%03d",contador);
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String(str_contador);
    }        
}