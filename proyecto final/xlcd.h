#ifndef __XLCD_H
#define __XLCD_H
/*Notes:
 *      - These libraries routines are written to support the
 *        Hitachi HD44780 LCD controller.
 *      - The user must define the following items:
 *          - The LCD interface type (4- or 8-bits)
 *          - If 4-bit mode
 *              - whether using the upper or lower nibble
 *          - The data port
 *              - The tris register for data port
 *              - The control signal ports and pins
 *              - The control signal port tris and pins
 *          - The user must provide three delay routines:
 *              - DelayFor18TCY() provides a 18 Tcy delay
 *              - DelayPORXLCD() provides at least 15ms delay
 *              - DelayXLCD() provides at least 5ms delay
 */
/* Interface type 8-bit or 4-bit
 * For 8-bit operation uncomment the #define BIT8
 */
/*#define BIT8 */

/* When in 4-bit interface define if the data is in the upper
 * or lower nibble.  For lower nibble, comment the #define UPPER
 */
/*#define UPPER */

/* DATA_PORT defines the port to which the LCD data lines are connected */
#define DATA_PORT  PORTD
#define TRIS_DATA_PORT  TRISD

/* CTRL_PORT defines the port where the control lines are connected.
 * These are just samples, change to match your application.
 */
#define RW_PIN   LATDbits.LATD4   /* PORT for RW */
#define TRIS_RW  TRISDbits.TRISD4    /* TRIS for RW */
#define RS_PIN   LATDbits.LATD5   /* PORT for RS */
#define TRIS_RS  TRISDbits.TRISD5    /* TRIS for RS */
#define E_PIN    LATDbits.LATD6   /* PORT for E  */
#define TRIS_E   TRISDbits.TRISD6    /* TRIS for E  */
//#define PWR_PIN	 PORTDbits.RD7	 /* PORT for PWR */
//#define TRIS_PWR DDRDbits.RD7	 /* TRIS for PWR */

/* Display ON/OFF Control defines */
#define DON         0b00001111  /* Display on      */
#define DOFF        0b00001011  /* Display off     */
#define CURSOR_ON   0b00001111  /* Cursor on       */
#define CURSOR_OFF  0b00001101  /* Cursor off      */
#define BLINK_ON    0b00001111  /* Cursor Blink    */
#define BLINK_OFF   0b00001110  /* Cursor No Blink */

/* -----------------Some Command Type defines----------------------	*/
#define CLEAR_XLCD		0x01		/* Clears the LCD		*/
#define RETURN_CURSOR_HOME	0x02		/* Returns the cursor to the HOME position	*/

/* -----------------Cursor or Display Shift defines -----------------*/
#define SHIFT_CUR_LEFT    0b00010011  /* Cursor shifts to the left   */
#define SHIFT_CUR_RIGHT   0b00010111  /* Cursor shifts to the right  */
#define SHIFT_DISP_LEFT   0b00011011  /* Display shifts to the left  */
#define SHIFT_DISP_RIGHT  0b00011111  /* Display shifts to the right */

// ----------------- Function Set defines------------------------- 
#define FOUR_BIT   0b00101111  /* 4-bit Interface               */
#define EIGHT_BIT  0b00111111  /* 8-bit Interface               */
#define LINE_5X7   0b00110011  /* 5x7 characters, single line   */
#define LINE_5X10  0b00110111  /* 5x10 characters               */
#define LINES_5X7  0b00111011  /* 5x7 characters, multiple line */
/* LINES*/
#define DDRAM_LINE1   0b10000000  /* 4-bit Interface               */
#define DDRAM_LINE2   0b11000000  /* 4-bit Interface               */
#define CGRAM_ADDRESS 0b01000000  /* 4-bit Interface               */

// -----------OpenXLCD --> Configura pines de I/O para el LCD
void OpenXLCD(unsigned char lcdtype);
// -----------SetCGRamAddr  --> Establece la dirección del generador de caracteres
void SetCGRamAddr(unsigned char);
// -----------SetDDRamAddr --> Establece la dirección de visualización de datos
void SetDDRamAddr(unsigned char);
// -----------BusyXLCD --> Retorna el estatus de ocupado del LCD
unsigned char BusyXLCD(void);
// -----------ReadAddrXLCD --> Lee la actual dirección
unsigned char ReadAddrXLCD(void);
// -----------ReadDataXLCD --> Lee un byte de dato
char ReadDataXLCD(void);
// --------------WriteCmdXLCD --> Escribe un comando a la pantalla LCD
void WriteCmdXLCD(unsigned char cmd);
// --------------WriteDataXLCD -->  Escribe un byte de datos en la pantalla LCD
void WriteDataXLCD(char);
// --------------putcXLCD  -->   Un putc es una escritura
#define putcXLCD WriteDataXLCD
#define XLCD_clear WriteCmdXLCD(CLEAR_XLCD);
// --------------putsXLCD --> Escribe una cadena de caracteres en la pantalla LCD
void putsXLCD(char *);
// --------------putrsXLCD --> Escribe una cadena de caracteres en la ROM del LCD
void putrsXLCD(const char *buffer);

/* User defines these routines according to the oscillator frequency */
//extern void DelayFor18TCY(void);   // 1us delay
//extern void DelayPORXLCD(void);    // 15ms delay
//extern void DelayXLCD(void); // 4.1ms

extern void Delay_1us(void);   // 1us delay
extern void DelayPORXLCD(void);    // 15ms delay
extern void DelayXLCD(void); // 4.1ms
extern void DelayExecution(void);

//--------------------Funciones para el LCD utilizando las librerias

void XLCDInit(void);
void XLCDgotoXY(int x,int y);
void XLCD_WriteChr_CGRAM( const char *buffer, unsigned char Addres);

#endif
