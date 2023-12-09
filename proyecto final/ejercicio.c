/* EJERCICIO PC4 CONVERSOR ADC
 * Ing. César Ciriaco Martinez
 * MICROCONTROLADORES PIC18F4550
 */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fusibles.h"
#include "xlcd.h"

uint32_t Valor_temp=0,Valor_hum=0;
uint16_t ValorADC_H,ValorADC_L;
void config_interrupcion();
void InitPort(void)      // inicializamos los PUERTOS
{   TRISD = 0x00;        //para el LCD
    TRISBbits.TRISB0=1;  //para el Canal AN2
    TRISBbits.TRISB0=1;
    TRISAbits.TRISA2=1;  //para el Canal AN2
    TRISAbits.TRISA5=1;  //para el Canal AN4    
}

void adc_Init()
{   ADCON0bits.CHS=2;         //  canal 2
    ADCON1bits.PCFG=0b1010;   //  AN0 hasta AN4 son analogicos y el resto digitales
    ADCON1bits.VCFG=0;        //VREF  5v y GND
    ADCON2bits.ADFM=0;        // justificacion a la izquierda
    ADCON2bits.ACQT=3;       // 6TAD de tiempo de adquisicion para 2.45uS   6*TAD= 4.8 uS > 2.45uS
    ADCON2bits.ADCS=5;       // FOSC/16 ó 16*TOSC    ES 0.8 uS   es > 0.7 uS < TAD < 3.5us
    ADCON0bits.GO_DONE=0;     //ADC en reposo
    ADCON0bits.ADON=1;        // encendemos el ADC
    __delay_us(3); 
}
void main(void) {
    float string_temp[10];      // arreglo para almacenar y enviar al LCD
    uint16_t string_hum[10];       // arreglo para almacenar y enviar al LCD
    
    InitPort();    //función InitPort 
    XLCDInit();    //función del LCD para Inicializar y configurar
    adc_Init();     //función del ADC para Inicializar y configurar
 config_interrupcion();
    while(1){
        ADCON0bits.CHS=2;
      ADCON0bits.GO_DONE=1;                      //iniciar la adquisicion
      while(ADCON0bits.GO_DONE);                 //espera a que finalice la conversión GO_DONE=0
      ValorADC_H = ADRESH<<2;                    //se almacena en la variable ValorADC_H y rota 2 bits a la izquierda
      ValorADC_L = ADRESL>>6;                    //se almacena en la variable ValorADC_L y rota 6 bits a la derecha
      Valor_temp = ValorADC_H + ValorADC_L;   //almacena la suma en ValorleidoADC  (16 BITS)
      Valor_temp=(Valor_temp*150)/306;
      sprintf(string_temp,"%u",Valor_temp);    //convierte el valor numerico ValorleidoADC a una cadena en string_tmp
      XLCDgotoXY(0,0);                           //posiciona el curso en la columna 0 y fila 0
      putrsXLCD("TEMP:");
      putsXLCD(string_temp);                      //imprime al lcd lo que hay en el arreglo
      putrsXLCD("°C");
      __delay_ms(1);
      
      ADCON0bits.CHS=4;
      ADCON0bits.GO_DONE=1;                      //iniciar la adquisicion
      while(ADCON0bits.GO_DONE);                 //espera a que finalice la conversión GO_DONE=0
      ValorADC_H = ADRESH<<2;                    //se almacena en la variable ValorADC_H y rota 2 bits a la izquierda
      ValorADC_L = ADRESL>>6;                    //se almacena en la variable ValorADC_L y rota 6 bits a la derecha
      Valor_hum = ValorADC_H + ValorADC_L;   //almacena la suma en ValorleidoADC  (16 BITS)
      Valor_hum=(Valor_hum*100)/1023;
      sprintf(string_hum,"%u",Valor_hum);
      XLCDgotoXY(1,0);                           //posiciona el curso en la columna 0 y fila 0
      putrsXLCD("HUM:");
      putsXLCD(string_hum);                      //imprime al lcd lo que hay en el arreglo
      putrsXLCD("%");
      
      
      
      __delay_ms(300);
      XLCD_clear;
    }    
    return;
}
void config_interrupcion()
{
  //REGISTRO RCON
                  //IPEN=1 ---HABILITAS LAS PRIORIDADES    IPEN=0 --- SOLO HAY ALTA PRIORIDAD
   RCONbits.IPEN=1; // habilisto las prioridades
                  //INT0  ya está por defecto como ALTA PRIORIDAD
   INTCONbits.INT0IE=1;
   INTCONbits.INT0IF=0;
                 //INT1 configurarlo como BAJA PRIORIDAD
   INTCON3bits.INT1IP=1;  //BAJA PRIORIDAD
   INTCON3bits.INT1IE=1;  //HABILITAS LA INTERRUPCION DEL INT1
   INTCON3bits.INT1IF=0;  // LIMPLIAMOS EL FLAG
                //HABILITAMOS GIEH y GIEL para BAJA Y ALTA PRIORIDAD
   INTCONbits.GIEH=1;
   INTCONbits.GIEL=1;       
}
__interrupt(high_priority) void INT(void)
{
  if (INTCONbits.INT0IF==1 && Valor_temp>38 )
  {
     XLCDgotoXY(0,0);                           //posiciona el curso en la columna 0 y fila 0
      putrsXLCD("TEMPERATURA ALTA!");
      __delay_ms(5000);
      }
      INTCONbits.INT0IF=0;
      
      if (INTCON3bits.INT1IF==1  && Valor_hum>90 )
     {
          XLCDgotoXY(1,0);                           //posiciona el curso en la columna 0 y fila 0
      putrsXLCD("HUMEDAD EXCESIVA!");
      __delay_ms(5000);
      }
      INTCON3bits.INT1IF=0;
}
