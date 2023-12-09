/* Valor de ADC EN LCD
 * Ing. César Ciriaco Martinez
 * MICROCONTROLADORES PIC18F4550
 */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fusibles.h"
#include "xlcd.h"

uint16_t ValorleidoADC=0;          // se crean variables sin signo de 16 bits (0-65535)
uint16_t ValorADC_H,ValorADC_L;
float volt;

void InitPort(void)      // inicializamos los PUERTOS
{   TRISD = 0x00;        //para el LCD
    TRISAbits.TRISA0=1;  //para el Canal AN0 
    TRISBbits.TRISB4=1; // entrada nivel
    TRISBbits.TRISB6=0; // salida bomba 1
    TRISAbits.TRISA1=0; // salida bomba 2
}

void adc_Init()
{   ADCON0bits.CHS=0;         // canal 0
    ADCON1bits.PCFG=0b1101;   //  AN0 y AN1
    ADCON1bits.VCFG=0;        //VREF  5v y GND
    ADCON2bits.ADFM=0;        // justificacion a la izquierda
    ADCON2bits.ACQT=3;       // 6TAD de tiempo de adquisicion para 2.45uS   6*TAD= 4.8 uS > 2.45uS
    ADCON2bits.ADCS=5;       // FOSC/16 ó 16*TOSC    ES 0.8 uS   es > 0.7 uS < TAD < 3.5us
    ADCON0bits.GO_DONE=0;     //ADC en reposo
    ADCON0bits.ADON=1;        // encendemos el ADC
    __delay_us(3); 
}
void main(void) {
    uint8_t string_tmp[10];      // arreglo para almacenar y enviar al LCD
    float string_volt[10];       // arreglo para almacenar y enviar al LCD
    
    InitPort();    //función InitPort 
    XLCDInit();    //función del LCD para Inicializar y configurar
    adc_Init();     //función del ADC para Inicializar y configurar
    
    while(1){
      ADCON0bits.GO_DONE=1;                      //iniciar la adquisicion
      while(ADCON0bits.GO_DONE);                 //espera a que finalice la conversión GO_DONE=0
      ValorADC_H = ADRESH<<2;                    //se almacena en la variable ValorADC_H y rota 2 bits a la izquierda
      ValorADC_L = ADRESL>>6;                    //se almacena en la variable ValorADC_L y rota 6 bits a la derecha
      ValorleidoADC = ValorADC_H + ValorADC_L;   //almacena la suma en ValorleidoADC  (16 BITS)
      volt=ValorleidoADC;                        //pasa el valor a la variable flotante volt
      XLCDgotoXY(0,0);                           //posiciona el curso en la columna 0 y fila 0
      putrsXLCD("VALORES: ");
      XLCDgotoXY(0,11);                           //posiciona el curso en la columna 0 y fila 1
      volt=((volt*14)/1023);                      //se realiza la linealización
      sprintf(string_volt,"%0.2f",volt);            //almacena en el arreglo string_volt en formato flotante de la variable volt
      XLCDgotoXY(1,0);                           //posiciona cursor
      putrsXLCD("Ph: ");
      putsXLCD(string_volt);                     //imprime la cadena y muestra en el LCD
      putrsXLCD(" ");
      __delay_ms(200);
      
               if(  6.5>=volt || volt<7.5  ){  //condicion de medicion del pH para activar las bombas 
             if(PORTBbits.RB4==0){         // condicion para avisar  el nivel del agua alto
             
            LATBbits.LATB6=0;   //puertos para mandar la señal de la bomba 1 desactivada
           LATAbits.LATA1=0;  //puertos para mandar la señal de la bomba 2 desactivada
            XLCDgotoXY(0,11);
    putrsXLCD("B1 OF");
     XLCDgotoXY(1,11);                          //posiciona el curso en la columna 0 y fila 0
      putrsXLCD("B2 OF");
            __delay_ms(500);}
        }else if( volt>7.5 ){ //condicion de medicion del pH para activar las bombas 
            if(PORTBbits.RB4==0){ // condicion para avisar  el nivel del agua alto
        
           LATBbits.LATB6=1; //puertos para mandar la señal de la bomba 1 activada
            __delay_ms(100);
           LATAbits.LATA1=1; //puertos para mandar la señal de la bomba 2 activada
           XLCDgotoXY(0,11);
    putrsXLCD("B1 ON");
     XLCDgotoXY(1,11);                          //posiciona el curso en la columna 0 y fila 0
      putrsXLCD("B2 ON");
      __delay_ms(500);
            }}
        
        
        if(  6.5>=volt || volt<7.5  ){ //condicion de medicion del pH para activar las bombas 
            if(PORTBbits.RB4==1){ // condicion para avisar  el nivel del agua bajo
             
            LATBbits.LATB6=0; //puertos para mandar la señal de la bomba 1 desactivada
           LATAbits.LATA1=0; //puertos para mandar la señal de la bomba 2 desactivada
            XLCDgotoXY(0,11);
    putrsXLCD("B1 OF");
     XLCDgotoXY(1,11);                          //posiciona el curso en la columna 0 y fila 0
      putrsXLCD("B2 OF");
            __delay_ms(500);}
        }else if( volt>7.5 ){ //condicion de medicion del pH para activar las bombas 
            if(PORTBbits.RB4==1){  // condicion para avisar  el nivel del agua bajo
           LATBbits.LATB6=0; //puertos para mandar la señal de la bomba 1 desactivada
            __delay_ms(100);
           LATAbits.LATA1=1; //puertos para mandar la señal de la bomba 2 activada
           XLCDgotoXY(0,11);
    putrsXLCD("B1 OF");
     XLCDgotoXY(1,11);                          //posiciona el curso en la columna 0 y fila 0
      putrsXLCD("B2 ON");
      __delay_ms(500);}
        }
    }    
    return;
}

