#include <xc.h>
#include "ADC_Libreria.h"

void ADC_Init(void){
    
    ADCON1bits.PVCFG = 0b00; // Voltaje positivo de referencia interno AVDD
    ADCON1bits.NVCFG = 0b00; // voltaje negativo de referencia interno AVss
    ADCON2bits.ADFM = 1; //Justificado a la derecha
    ADCON2bits.ACQT = 0b100;  //Tiempo de adqusición de 8 Tad (8us)
    ADCON2bits.ADCS = 0b101; //Seleccionar Fosc/16 = 16Mhz/16 = 1Mhz, tad = 1us
    ADCON0bits.ADON = 1; //habiiltar el modulo ADC
    
}   

uint16_t ADC_Read(ChanneL_ADC channel){
   
    ADCON0bits.CHS =channel; //Seleccionamos AN0 para su conversion
    ADCON0bits.GO_DONE=1; //Inicia la conversion ADC
    while(ADCON0bits.GO_DONE); //Esperar que terminade de convertir el voltaje analogico GO_DONE=0
    return (uint16_t)((ADRESH<<8)+(ADRESL));
}         

