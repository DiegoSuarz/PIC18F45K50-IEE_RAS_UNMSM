
#ifndef XC_ADC_libreria_H
#define	XC_ADC_libreria_H

#include <xc.h>

#define VREF    5.0
#define RESOLUCION 1024

typedef enum{
    ADC_0 = 0b00000,
    ADC_1,
    ADC_2,
    ADC_3,
    ADC_4,
    ADC_5,
    ADC_6,
    ADC_7,
    ADC_8,
    ADC_9,
    ADC_10,
    ADC_11,
    ADC_12,
    ADC_13,
    ADC_14,
    ADC_15,
    ADC_16,
    ADC_17,
    ADC_18,
    ADC_19,
    Temperature_Diode = 0b11100,
    CTMU, 
    DAC,
    FVR_BUF2
}ChanneL_ADC;

void ADC_Init(void);
uint16_t ADC_Read(ChanneL_ADC channel);

#endif	/* XC_ADC_libreria_H */

