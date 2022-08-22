// LIBRERIA DE 4 BITS

// Establecer las conexiones de la LCD
#define _XTAL_FREQ  16000000UL
// Establecer las conexiones de la LCD
#define TRIS_RS TRISDbits.RD0
#define PIN_RS  LATDbits.LATD0

#define TRIS_E TRISDbits.RD1
#define PIN_E  LATDbits.LATD1

#define TRIS_D4 TRISDbits.RD4
#define PIN_D4  LATDbits.LATD4

#define TRIS_D5 TRISDbits.RD5
#define PIN_D5  LATDbits.LATD5

#define TRIS_D6 TRISDbits.RD6
#define PIN_D6  LATDbits.LATD6

#define TRIS_D7 TRISDbits.RD7
#define PIN_D7  LATDbits.LATD7

/********************************************************/
// DEFINICION DE  COMANDOS PROPUESTOS POR EL FABRICANTE //
//           CON MODELO MIKROC                          //
/********************************************************/
#define      _LCD_FIRST_ROW           128
#define      _LCD_SECOND_ROW          192
#define      _LCD_THIRD_ROW           148
#define      _LCD_FOURTH_ROW          212
#define      _LCD_CLEAR               1
#define      _LCD_RETURN_HOME         2
#define      _LCD_CURSOR_OFF          12
#define      _LCD_UNDERLINE_ON        14
#define      _LCD_BLINK_CURSOR_ON     15
#define      _LCD_MOVE_CURSOR_LEFT    16
#define      _LCD_MOVE_CURSOR_RIGHT   20
#define      _LCD_TURN_OFF            0
#define      _LCD_TURN_ON             8
#define      _LCD_SHIFT_LEFT          24
#define      _LCD_SHIFT_RIGHT         28

//definir los metodos a utilizar
void Lcd_Init(void);
void enviar_dato(unsigned char cmd);
void Lcd_Char(char letra);
void Lcd_Cmd(unsigned char cmd);
void Lcd_XY(unsigned char y, unsigned char x);
void Lcd_Char_XY(unsigned char y, unsigned char x, char letra);
void Lcd_Out(char *text);
void Lcd_Out_XY(unsigned char y, unsigned char x, char *text);

