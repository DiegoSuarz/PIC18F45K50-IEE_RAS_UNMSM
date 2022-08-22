/*
 * File:   main.c
 * Author: dg_su
 *
 * Created on 18 de agosto de 2022, 08:48 PM
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "config_fuses.h"
#include "LIBRERIA_LCD.h"
#include "ADC_Libreria.h"
#include "USART.h"


#define _XTAL_FREQ  16000000UL
#define ENCENDIDO   1
#define APAGADO 0

char lcd_buffer[20]=" ";

int16_t lectura_adc;
float LM35,temp,sensor;

uint8_t data_aux,data_seg=45,data_min=59,data_hor=1;
uint8_t data_diaS=0, data_dia=1, data_mes=1, data_year=20;
uint8_t a_min=0,a_hor=0;
uint8_t dato_rx;

uint8_t menu=0;

uint8_t config=0;

uint8_t flag,flag_a,flag_f,aStatus;
uint8_t alarmaOn;
uint8_t cont_T0=0;

void Config_Interrupt_priority(void);
void Config_TMR0(void);
 
void menu0(void);
void menu1(void);
void menu2(void);
void menu3(void);
void menu4(void);

void Config_Clock(void);

void Config_Port(void);
void visualizar_Parametros(void);


void main(void) {
    Config_Clock();
    Config_Port();
    
    Config_Interrupt_priority();
    ADC_Init();
    USART_init(9600);
    Config_TMR0();
    Lcd_Init();
    Lcd_Cmd(_LCD_CURSOR_OFF);
    Lcd_XY(1,1);
	Lcd_Out("Proyecto Final");
	Lcd_XY(2,2);
	Lcd_Out("del curso PIC");
	Lcd_XY(3,3);
	Lcd_Out("IEEE RAS UNMSM");
	
    USART_printString("Proyecto Final\r");
    USART_printString("del curso PIC\r");
	USART_printString("IEEE RAS UNMSM\r");
    __delay_ms(1000);

    while(1){
        switch(menu){
//            case 0: menu0();break;
            case 1: menu1();break;
            case 2: menu2();break;
            case 3: menu3();break;
            case 4: menu4();break;
            default :menu0();break;
		}
    }
       
    return;
}

void Config_Port(void){
        
    LATCbits.LATC0 = 0;
    //limpiar los puertos de salida
    LATD=0;

    //Establecer la velocidad de los puertos
    SLRCON=0b00000000;
    
    //Configuramos pines como analogico o digital(0=digital, 1= analogico)
    ANSELAbits.ANSA0 = 1;
    ANSELD = 0;

    ANSELBbits.ANSB1=0;
    ANSELBbits.ANSB2=0;
    ANSELBbits.ANSB3=0;
    ANSELBbits.ANSB4=0;
    
    //Establecemos como entrada o salida (0=salida, 1= entrada)
    TRISCbits.TRISC0=0;
    
    TRISAbits.TRISA0=1;
    TRISBbits.TRISB3=1;
    TRISBbits.TRISB4=1;
    
    TRISD=0;
     
}

void Config_Interrupt_priority(void){
    /*Configuracion de prioridades*/
    RCONbits.IPEN=1; //habilitar las interrupciones por prioridad (alta o baja)
    INTCONbits.GIEH = 1; //habilitar interrucpiones de alta prioridad
    INTCONbits.GIEL = 1; //habilitar interrupciones de baja prioridad
    
    /*Configuracion de interrupciones de baja y alta prioridad*/
    INTCON2bits.TMR0IP = 1; //interrupcion desbordamiento del TMR0 alta prioridad
    INTCONbits.TMR0IF = 0; //limpiar bandera de interrupcion del TMR0
    INTCONbits.TMR0IE = 1; //habilitar interrupcion por desbordamiento del TMR0
     
    /*Configuración Interrupcion externa 1 INT1*/
    INTCON3bits.INT1IP = 0; //interrupcion externa 1 como baja prioridad
    INTCON3bits.INT1E=1; //habilitar interrupcion externa 1
    INTCON3bits.INT1F=0; //limpiar bandera de interrupcion INT1
    INTCON2bits.INTEDG1=0; //configurar INT1 por flanco de bajada 
    
    /*Configuración Interrupcion externa 2 INT2*/
     INTCON3bits.INT2IP = 0; //interrupcion externa 2 como baja prioridad
    INTCON3bits.INT2E=1; //habilitar interrupcion externa 2
    INTCON3bits.INT2F=0; //limpiar bandera de interrupcion INT2
    INTCON2bits.INTEDG2=0; //configurar INT2 por flanco de bajada 
    
    /*Configuración Interrupcion por recepción UART*/
    IPR1bits.RCIP = 0; //interrupcion por recepcion de Usart de baja prioridad
    PIE1bits.RC1IE=1; //habilitamos la interrupcion por recepcion de datos uart
    PIR1bits.RCIF=0; //limpiamos la bandera de interrupcion
 }

void Config_TMR0(void){
        
    T0CONbits.TMR0ON = 0; //detener TMR0
    T0CONbits.T08BIT = 0; //TMR0 modo 16 bits
    T0CONbits.T0CS = 0; //modo temporizador, oscilador interno FOSC/4
        /*
     * Formula de TMR0:
     * para 16 bits:
     * TMR0 = 65536-(tiempo(s)/(TCY * prescaler))  
     * TMR0 = 65536-(tiempo(s)/((4/FOSC) * prescaler))
     *Ejm= tiempo = 1s      FOSC= 16MHz    prescaler=128
     *  
     * TMR0 = 65536-(1/((4/16x10^6)* 128 ))  
     * TMR0 = 34286
    */
    
    T0CONbits.PSA = 0; //prescaler activado
    T0CONbits.T0PS = 0b110; //prescaler 1:128 (si esta desactivado el prescaler no es necesario configurar)
    TMR0 = 34286; //carga para 1s
    T0CONbits.TMR0ON = 1; //habilitar TMR0
}

void menu0(void){
    if(config == 0){
        Lcd_XY(1,1);
        Lcd_Char('-');
        Lcd_Out(" Reloj             ");
    }else{
        Lcd_XY(1,1);
        Lcd_Out("  Reloj            ");
    }
    if(config == 1){
        Lcd_XY(2,1);
        Lcd_Char('-');
        Lcd_Out(" Establecer Hora   ");
    }else{
        Lcd_XY(2,1);
        Lcd_Out("  Establecer Hora  ");
    }
    if(config == 2){
        Lcd_XY(3,1);
        Lcd_Char('-');
        Lcd_Out(" Establecer Fecha  ");
    }else{
        Lcd_XY(3,1);
        Lcd_Out("  Establecer Fecha ");
    }
    if(config ==3){
        Lcd_XY(4,1);
        Lcd_Char('-');
        Lcd_Out(" Establecer Alarma ");
    }else{
        Lcd_XY(4,1);
        Lcd_Out("  Establecer Alarma");
    }
    
    
}

void menu1(void){
    Lcd_XY(1,1);
    Lcd_Out("  Reloj despertador ");
    visualizar_Parametros();


}

void menu2(void){
    while(menu==2){
        T0CONbits.TMR0ON = 0; //detener TMR0
		if(!PORTBbits.RB3){
			while(!PORTBbits.RB3);
			if(flag==0){
				data_seg++;
				if(data_seg==60)data_seg=0;
			}
			if(flag==1){
				data_min++;
				if(data_min==60)data_min=0;
			}
			if(flag==2){
				data_hor++;
				if(data_hor==23)data_hor=0;
			}
		}
		if(!PORTBbits.RB4){
			while(!PORTBbits.RB4);
			flag++;
			if(flag==3)flag=0;
		}
		
		if(flag==0){
			Lcd_XY(3,13);
			Lcd_Out("  ");
			
		}
		if(flag==1){
			Lcd_XY(3,10);
			Lcd_Out("  ");
			
		}
		if(flag==2){
			Lcd_XY(3,7);
			Lcd_Out("  ");
		}
		__delay_ms(110);
		Lcd_XY(1,1);
		Lcd_Out("Establecer hora:    ");
		Lcd_XY(2,1);
		Lcd_Out("                    ");
		Lcd_XY(3,1);
		Lcd_Out("Hora: ");
		Lcd_Char((data_hor/10)+48);
		Lcd_Char((data_hor%10)+48);
		Lcd_Char(':');
		Lcd_Char((data_min/10)+48);
		Lcd_Char((data_min%10)+48);
		Lcd_Char(':');
		Lcd_Char((data_seg/10)+48);
		Lcd_Char((data_seg%10)+48);
		Lcd_Char(' ');
		Lcd_Char(' ');
		Lcd_Char(' ');
		Lcd_Char(' ');
		Lcd_Char(' ');
		Lcd_XY(4,1);
		Lcd_Out("                    ");
		__delay_ms(110);
	}
    T0CONbits.TMR0ON = 1; //habilitar TMR0
    USART_printString("Hora Actualizada: ");
    printf("%02d:%02d:%02d\r",data_hor,data_min,data_seg);
    
	flag=0;
}

void menu3(void){
	while(menu==3){
		if(!PORTBbits.RB3){
			while(!PORTBbits.RB3);
			if(flag_f==0){
				data_dia++;
				if(data_dia==32)data_dia=1;
			}
			if(flag_f==1){
				data_mes++;
				if(data_mes==13)data_mes=1;
			}
			if(flag_f==2){
				data_year++;
				if(data_year==100)data_year=20;
			}
			if(flag_f==3){
				data_diaS++;
				if(data_diaS==7)data_diaS=0;
			}
		}
		if(!PORTBbits.RB4){
			while(!PORTBbits.RB4);
			flag_f++;
			if(flag_f==4)flag_f=0;
		}
		
		if(flag_f==0){
			Lcd_XY(2,9);
			Lcd_Out("  ");
			
		}
		if(flag_f==1){
			Lcd_XY(2,12);
			Lcd_Out("  ");
			
		}
		if(flag_f==2){
			Lcd_XY(2,15);
			Lcd_Out("  ");
		}
		if(flag_f==3){
			Lcd_XY(3,7);
			Lcd_Out("         ");
		}
		__delay_ms(110);
		Lcd_XY(1,1);
		Lcd_Out("  Configurar fecha  ");
		Lcd_XY(2,2);
		Lcd_Out("Fecha: ");
		Lcd_Char((data_dia/10)+48);
		Lcd_Char((data_dia%10)+48);
		Lcd_Char('/');
		Lcd_Char((data_mes/10)+48);
		Lcd_Char((data_mes%10)+48);
		Lcd_Char('/');
		Lcd_Char((data_year/10)+48);
		Lcd_Char((data_year%10)+48);
		Lcd_Char(' ');
		Lcd_Char(' ');
		Lcd_Char(' ');
		Lcd_XY(3,1);
		Lcd_Out("Dia:  ");
		if(data_diaS==0)Lcd_Out("Lunes         ");
		if(data_diaS==1)Lcd_Out("Martes        ");
		if(data_diaS==2)Lcd_Out("Miercoles     ");
		if(data_diaS==3)Lcd_Out("Jueves        ");
		if(data_diaS==4)Lcd_Out("Viernes       ");
		if(data_diaS==5)Lcd_Out("Sabado        ");
		if(data_diaS==6)Lcd_Out("Domingo       ");
		Lcd_XY(4,1);
		Lcd_Out("                    ");
        __delay_ms(110);
	}
    USART_printString("Fecha Actualizada: ");
    printf("%02d/%02d/%02d\r",data_dia,data_mes, data_year);
    USART_printString("Dia: ");
    if(data_diaS==0)USART_printString("Lunes\r");
    if(data_diaS==1)USART_printString("Martes\r");
    if(data_diaS==2)USART_printString("Miercoles\r");
    if(data_diaS==3)USART_printString("Jueves\r");
    if(data_diaS==4)USART_printString("Viernes\r");
    if(data_diaS==5)USART_printString("Sabado\r");
    if(data_diaS==6)USART_printString("Domingo\r");
	flag_f=0;
}

void menu4(void){
	while(menu==4){
		if(!PORTBbits.RB3){
			while(!PORTBbits.RB3);
			if(flag_a==0){
				a_min++;
				if(a_min==60)a_min=0;
			}
			if(flag_a==1){
				a_hor++;
				if(a_hor==23)a_hor=0;
			}
			if(flag_a==2){
				alarmaOn++;
				if(alarmaOn==2)alarmaOn=0;
			}
		}
		if(!PORTBbits.RB4){
			while(!PORTBbits.RB4);
			flag_a++;
			if(flag_a==3)flag_a=0;
		}
		
		if(flag_a==0){
			Lcd_XY(3,12);
			Lcd_Out("  ");
			
		}
		if(flag_a==1){
			Lcd_XY(3,9);
			Lcd_Out("  ");
		}
		if(flag_a==2){
			Lcd_XY(4,9);
			Lcd_Out("   ");
		}
		
		__delay_ms(110);
		Lcd_XY(1,1);
		Lcd_Out("Configurar alarma:");
		Lcd_XY(2,1);
		Lcd_Out("                 ");
		Lcd_XY(3,1);
		Lcd_Out("Alarma: ");
		Lcd_Char((a_hor/10)+48);
		Lcd_Char((a_hor%10)+48);
		Lcd_Char(':');
		Lcd_Char((a_min/10)+48);
		Lcd_Char((a_min%10)+48);
		Lcd_Char(' ');
		Lcd_Char(' ');
		Lcd_Char(' ');
		Lcd_Char(' ');
		Lcd_Char(' ');
		Lcd_Char(' ');
		if(alarmaOn==1){
			Lcd_XY(4,1);
			Lcd_Out("ESTADO: ON          ");
			}else{
			Lcd_XY(4,1);
			Lcd_Out("ESTADO: OFF         ");
		}
		__delay_ms(110);
	}
	flag_a=0;
	if(alarmaOn==1){
        USART_printString("Alarma Actualizada: ");
        printf("%02d:%02d\r",a_hor,a_min);
        
        USART_printString("\r");
    }
	else{USART_printString("Estado: Apagada\r\r");}
    
}



void visualizar_Parametros(void){
	
	if(menu==1){
		
		Lcd_XY(2,1);
		Lcd_Out("Hora:  ");
		Lcd_Char((data_hor/10)+48);
		Lcd_Char((data_hor%10)+48);
		Lcd_Char(':');
		Lcd_Char((data_min/10)+48);
		Lcd_Char((data_min%10)+48);
		Lcd_Char(':');
		Lcd_Char((data_seg/10)+48);
		Lcd_Char((data_seg%10)+48);
		Lcd_Char(' ');
		Lcd_Char(' ');
		if(alarmaOn==1){Lcd_Char('A');}
        else Lcd_Char(' ');
		Lcd_Char(' ');
				
		Lcd_XY(3,1);
		Lcd_Out("Fecha: ");
		Lcd_Char((data_dia/10)+48);
		Lcd_Char((data_dia%10)+48);
		Lcd_Char('/');
		Lcd_Char((data_mes/10)+48);
		Lcd_Char((data_mes%10)+48);
		Lcd_Char('/');
		Lcd_Char((data_year/10)+48);
		Lcd_Char((data_year%10)+48);
		Lcd_Char(' ');
		Lcd_Char(' ');
		
		if(data_diaS==0)Lcd_Out("Lu");
		if(data_diaS==1)Lcd_Out("Ma");
		if(data_diaS==2)Lcd_Out("Mi");
		if(data_diaS==3)Lcd_Out("Ju");
		if(data_diaS==4)Lcd_Out("Vi");
		if(data_diaS==5)Lcd_Out("Sa");
		if(data_diaS==6)Lcd_Out("Do");
		
        LM35=(float)(ADC_Read(_ADC_0)*(VREF/RESOLUCION)*100);
		sprintf(lcd_buffer,"Temp: %2.1f",LM35);
		Lcd_XY(4,1);
		Lcd_Out(lcd_buffer);
		Lcd_Char(0xDF);
		Lcd_Char('C');
		Lcd_Out("        ");
        if(data_hor==a_hor && data_min == a_min){
            if(!PORTBbits.RB3){
                while(!PORTBbits.RB3);
                alarmaOn=0;    
            }
            if(!PORTBbits.RB4){
                while(!PORTBbits.RB4);
                alarmaOn=0; 
            }
        }
		return;
	}
}
void Config_Clock(void){
    OSCCONbits.IRCF=0b111; //trabajar a 16Mhz Clock interno
    OSCCONbits.SCS=0b10; //Seleccionar el bloque de oscilador interno
}

void __interrupt(high_priority)ISR1(){
    if(INTCONbits.TMR0IF){
        TMR0 = 34286;
        cont_T0++;
        data_seg++;
        if(data_seg > 59){
            data_seg=0;
            data_min++;
            if(data_min > 59){
                data_min=0;
                data_hor++;
                if(data_hor > 23){
                    data_hor = 0;
                    data_dia++;
                    data_diaS++;
                    if(data_diaS>7)data_diaS=0;
                    if(data_dia>30){
                        data_dia=0;
                        data_mes++;
                        if(data_mes>12){
                            data_mes=0;
                            data_year++;
                            if(data_year > 99){
                                data_year = 00;
                            }
                        }
                    }
                }
                
            }
        }
        if(menu==1 && cont_T0==5){
            USART_printString("Hora: ");
            printf("%02d:%02d:%02d\r",data_hor,data_min,data_seg);
            USART_printString("Fecha: ");
            printf("%02d/%02d/%02d\r",data_dia,data_mes,data_year);
            USART_printString("Dia: ");
            if(data_diaS==0)USART_printString("Lunes\r");
            if(data_diaS==1)USART_printString("Martes\r");
            if(data_diaS==2)USART_printString("Miercoles\r");
            if(data_diaS==3)USART_printString("Jueves\r");
            if(data_diaS==4)USART_printString("Viernes\r");
            if(data_diaS==5)USART_printString("Sabado\r");
            if(data_diaS==6)USART_printString("Domingo\r");
            USART_printString("Temperatura: ");
            printf("%.02f\r",LM35);
            USART_printString("Estado Alarma: ");
            if(alarmaOn==0)USART_printString("Apagada\r");
            if(alarmaOn==1){USART_printString("Enecendida: ");printf("%02d:%02d\r",a_hor,a_min);}
            USART_printString("\r");
            cont_T0=0;
        }
        if(data_hor==a_hor && data_min == a_min && alarmaOn==1)LATC^=(1<<0);
        if(cont_T0>5)cont_T0=0;
        INTCONbits.TMR0IF=0; 
    }
}
  
void __interrupt(low_priority)ISR2(){
    if(INTCON3bits.INT1F){
        
        if(config==0){
		menu=1;
        }
        if(config==1){
            menu=2;
        }
        if(config==2){
            menu=3;
        }
        if(config==3){
            menu=4;
        }
        INTCON3bits.INT1F=0;
    }
    
    if(INTCON3bits.INT2F==1 ){
        if(menu==0){
            config++;
            if(config==4)config=0;
        }
        if(menu==1){menu=0;}
        if(menu==2){menu=0;}
        if(menu==3){menu=0;}	
        if(menu==4){menu=0;}	
        INTCON3bits.INT2F=0; 
        
    }
    
    if(PIR1bits.RCIF){
        PIR1bits.RCIF=0;
        dato_rx=RCREG;
        
        //aplicación:
        if(menu==1){
            if(dato_rx == 'a'){
                USART_printString("Alarma Encendida\r");
                alarmaOn = ENCENDIDO;

            }
            else if(dato_rx == 's'){
                USART_printString("Alarma Apagada\r");
                alarmaOn = APAGADO;
            }
        }
        
    }
}

