/*
 * File:   ADC_1.c
 * Author: César Ciriaco Martinez
 * Conversion a 8 bits del ADRESH
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fusibles.h"

void adc_Init()
{   ADCON0bits.CHS=0;          
    ADCON1bits.PCFG=0b1101;   //  AN0 y AN1
    ADCON1bits.VCFG=0;        //VREF  5v y GND
    ADCON2bits.ADFM=0;        // justificacion a la izquierda
    ADCON2bits.ACQT=3;       // 6TAD de tiempo de adquisicion para 2.45uS   6*TAD= 4.8 uS > 2.45uS
    ADCON2bits.ADCS=5;       // FOSC/16 ó 16*TOSC    ES 0.8 uS   es > 0.7 uS < TAD < 3.5us
    ADCON0bits.GO_DONE=0;     //ADC en reposo
    ADCON0bits.ADON=1;
    __delay_us(3); 
}
void main(void) {
    uint8_t VALOR_ADC_1,VALOR_ADC_2;
    TRISD=0;
    LATD=0;
    adc_Init();
    
    while (1)
    {
        ADCON0bits.CHS=0;
        ADCON0bits.GO_DONE=1;          //Iniciamos la conversion
        while(ADCON0bits.GO_DONE==1 ); //sale del bucle while cuando es 0
        VALOR_ADC_1=ADRESH;
        __delay_ms(1);  
        ADCON0bits.CHS=1;
        ADCON0bits.GO_DONE=1;          //Iniciamos la conversion
        while(ADCON0bits.GO_DONE==1 ); //sale del bucle while cuando es 0
        VALOR_ADC_2=ADRESH;  
        
        if (VALOR_ADC_1>VALOR_ADC_2)
        {
           LATD=255;
        }
        else
        {
            LATD=0;
        }   
        
        
    }
    return;
}
