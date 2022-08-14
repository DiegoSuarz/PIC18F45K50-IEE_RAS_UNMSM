#include "USART.h"

void USART_init(uint32_t baudios){
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
void USART_printChar(char caracter){
    TXREG=caracter;
    while(TXSTAbits.TRMT==0);//Esperar hasta que se complete la trasmision
}
void USART_printString(char *Ptr_string){
    while(*Ptr_string!='\0'){
        USART_printChar(*Ptr_string);
        Ptr_string++;
    }
}
//Funcion Adicional para enciar datos por uSART con la funcion printf() - stdio.h
void putch(char c){// la funcipon putch me va a dirigir los datos a printf()
    TXREG = c;
    while(TXSTAbits.TRMT==0);
}

char getChar_USART(void){
    while(PIR1bits.RCIF==0);//Esperar hasta que se reciba 1 dato por UART
    //Evaluar el desbordamiento
    if(RCSTAbits.OERR==1){
        RCSTAbits.CREN=0;//Deshabilito la receocion continua
        NOP(); //Realizo un retardo de 1 ciclo de instruccion
        RCSTAbits.CREN=1;//Habilito la recepcion continua
    }
    return (RCREG);//Devuelvo el caracter recibo en el registro RCREG
}