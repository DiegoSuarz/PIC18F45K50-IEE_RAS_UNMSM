
#ifndef XC_ADC_libreria_H
#define	XC_ADC_libreria_H

#include <xc.h>

#define VREF    5.0
#define RESOLUCION 1024

typedef enum{
    _ADC_0 = 0b00000,
    _ADC_1,
    _ADC_2,
    _ADC_3,
    _ADC_4,
    _ADC_5,
    _ADC_6,
    _ADC_7,
    _ADC_8,
    _ADC_9,
    _ADC_10,
    _ADC_11,
    _ADC_12,
    _ADC_13,
    _ADC_14,
    _ADC_15,
    _ADC_16,
    _ADC_17,
    _ADC_18,
    _ADC_19,
    _ADC_Temperature_Diode = 0b11100,
    _ADC_CTMU, 
    _ADC_DAC,
    _ADC_FVR_BUF2
}ChanneL_ADC;

void ADC_Init(void);
uint16_t ADC_Read(ChanneL_ADC channel);

#endif	/* XC_ADC_libreria_H */

