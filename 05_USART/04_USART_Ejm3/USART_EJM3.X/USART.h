#ifndef USART_H
#define	USART_H

#include <xc.h>
#include <stdint.h>
#include <stdio.h>

#define _XTAL_FREQ  16000000UL

void USART_init(uint32_t baudios);
void USART_printChar(char caracter);
void USART_printString(char *Ptr_string);
char USART_getChar(void);
#endif	/* USART_H */

