#include <xc.h>
#include "config_fuses.h"
#include "LIBRERIA_LCD.h"

/******************************************************/
void enviar_dato(unsigned char cmd){
    TRIS_D4=0;
    TRIS_D5=0;
    TRIS_D6=0;
    TRIS_D7=0;
    PIN_D4=0;
    PIN_D5=0;
    PIN_D6=0;
    PIN_D7=0;
    
    PIN_E=0;
    __delay_us(100);
    
    PIN_D4=(cmd>>4)&0x01;
    PIN_D5=(cmd>>5)&0x01;
    PIN_D6=(cmd>>6)&0x01;
    PIN_D7=(cmd>>7)&0x01;
    PIN_E=1;
    __delay_us(100);
    PIN_E=0;
    
    PIN_D4=(cmd&0x0F);
    PIN_D5=((cmd&0x0F)>>1);
    PIN_D6=((cmd&0x0F)>>2);
    PIN_D7=((cmd&0x0F)>>3);
    
    PIN_E=1;
    __delay_us(100);
    PIN_E=0;
    
    TRIS_D4=1;
    TRIS_D5=1;
    TRIS_D6=1;
    TRIS_D7=1;
       
    return;
}

void Lcd_Init(void){
    TRIS_E=0;
    TRIS_RS=0;
    TRIS_D4=0;
    TRIS_D5=0;
    TRIS_D6=0;
    TRIS_D7=0;
    
    PIN_RS=0;
    __delay_ms(15);
    // secuencia de sincro
    enviar_dato(0x30);
    __delay_ms(5);
    enviar_dato(0x30);
    __delay_us(100);
    enviar_dato(0x32);
    __delay_us(100);
    
    //configurar la lcd
    __delay_us(100);
    enviar_dato(0x2B);
    __delay_us(100);
    enviar_dato(0x08);
    __delay_us(100);
    enviar_dato(0x0F);
    __delay_us(100);
    enviar_dato(0x01);
    __delay_us(100);
    enviar_dato(0x06);
    __delay_us(100);
    
    enviar_dato(0x0F);
    __delay_us(100);  
    
    Lcd_Cmd(12);
    __delay_us(100);
    return;
}

void Lcd_Cmd(unsigned char cmd){
    PIN_RS=0;
    enviar_dato(cmd);
    PIN_RS=0;
    return;
}
void Lcd_Char(char letra){
    PIN_RS=1;
    enviar_dato(letra);
    PIN_RS=0;
    return;
}

void Lcd_XY(unsigned char y, unsigned char x){
    switch(y){
        case 1: Lcd_Cmd(0x80+(x-1)); break;
        case 2: Lcd_Cmd(0xC0+(x-1)); break;
        case 3: Lcd_Cmd(0x94+(x-1)); break;
        case 4: Lcd_Cmd(0xD4+(x-1)); break;
    }
    __delay_us(100);
    PIN_RS=0;
    return;
}
void Lcd_Char_XY(unsigned char y, unsigned char x, char letra){
    Lcd_XY(y,x);
    PIN_RS=1;
    enviar_dato(letra);
    PIN_RS=0;
    return;
}
void Lcd_Out(char *text){
    PIN_RS=1;
    while(*text){
        Lcd_Char(*text);
        text++;
    }
    return;
}

void Lcd_Out_XY(unsigned char y, unsigned char x, char *text){
    Lcd_XY(y,x);
    PIN_RS=1;
    while(*text){
        Lcd_Char(*text);
        text++;
    }
    return;
}
