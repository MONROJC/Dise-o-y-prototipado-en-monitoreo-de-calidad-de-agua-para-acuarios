/*
 * File:   lab5_dir_main.c
 * Author: DANIEL
 *
 * Created on 11 de julio de 2020, 03:10 PM
 */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fusibles.h"
#include "xlcd.h"
void Init_TMR0();
int cuenta;
uint8_t string_cuenta[1000]; 
void InitPort(void)      // inicializamos los PUERTOS
{   TRISD = 0x00;        //para el LCD
    
}
void main(void) {
    InitPort() ;
    XLCDInit();  
     Init_TMR0();
     while(1){
     }
    return;
}


void Init_TMR0(){
   
    T0CONbits.T0CS=0;    //TEMPORIZADOR
    T0CONbits.T08BIT=0;  //16 BITS
    T0CONbits.PSA=0;     //HABILITAMOS EL PRESCALER
    T0CONbits.T0PS=7;    //PRECALER X4
    TMR0L=0XEC;
    TMR0H=0XEC;
    INTCONbits.TMR0IF=0;  //LIPIAMOS LA BANDERA DE DESBORDAMIENTO DEL TMR0
    INTCONbits.TMR0IE=1;  //HABILITAMOS LA INTERRUPCION DEL 
    INTCONbits.GIE=1;     //ACTIVAMOS LA INTERRUPCION
    INTCONbits.PEIE=1;    //ACTIVAMOS EL PERIFERICO DEL TIMER0
    T0CONbits.TMR0ON=1;   //PUESTA EN MARCHA DEL TIMER0
}

__interrupt(high_priority) void TIMER_0(void)    //High priority interrupt ISR
{  
   if(INTCONbits.TMR0IF==1)//polling for INT0 interrupt
   { for(cuenta=0;cuenta<100;cuenta++){
        XLCDgotoXY(0,0);  
      sprintf(string_cuenta,"%u",cuenta);
      XLCDgotoXY(0,5);                           //posiciona el curso en la columna 0 y fila 0
      putrsXLCD("CONTADOR");
      XLCDgotoXY(1,5);  
      putsXLCD(string_cuenta);                     //imprime al lcd lo que hay en el arreglo
      TMR0L=0XEC;
    TMR0H=0XEC;
    }
   cuenta=0;
}
     INTCONbits.TMR0IF=0;
}