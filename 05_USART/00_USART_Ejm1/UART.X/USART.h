#ifndef USART_H
#define	USART_H

#include <xc.h>
#include <stdint.h>
#include <stdio.h>

#define _XTAL_FREQ  16000000

void USART_init(uint32_t baudios);
void USART_printChar(char caracter);
void USART_printString(char *Ptr_string);
char getChar_USART(void);
#endif	/* USART_H */

