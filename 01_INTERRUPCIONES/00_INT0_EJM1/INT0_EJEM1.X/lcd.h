#ifndef LCD_H
#define	LCD_H
    #include <xc.h>
    #define _XTAL_FREQ  16000000UL
    #define RS LATDbits.LATD2  //LATAbits.LATA5  
    #define EN LATDbits.LATD3  //LATAbits.LATA3   
    #define D4 LATDbits.LATD4
    #define D5 LATDbits.LATD5
    #define D6 LATDbits.LATD6
    #define D7 LATDbits.LATD7

    void Lcd_Port(unsigned char a);
    void Lcd_Cmd(unsigned char a);
    void Lcd_Clear( void );
    void Lcd_Set_Cursor(unsigned char a, unsigned char b);
    void Lcd_Init( void );
    void Lcd_Write_Char(unsigned char a);
    void Lcd_Write_String(unsigned char *a);
    void Lcd_Shift_Right( void );
    void Lcd_Shift_Left( void );

#endif	/* LCD_H */

