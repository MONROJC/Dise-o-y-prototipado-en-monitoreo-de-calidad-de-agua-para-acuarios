
#include <xc.h>
#include "fusibles.h"
#include "xlcd.h"

void OpenXLCD(unsigned char lcdtype)
{
	DelayPORXLCD();		// Delay 15ms

        // The data bits must be either a 8-bit port or the upper or
        // lower 4-bits of a port. These pins are made into inputs
#ifdef BIT8                             // 8-bit mode, use whole port
        DATA_PORT = 0;
        TRIS_DATA_PORT = 0xff;
#else                                   // 4-bit mode
     #ifdef UPPER                            // Upper 4-bits of the port
        DATA_PORT &= 0x0f;
        TRIS_DATA_PORT |= 0xf0;
     #else                                   // Lower 4-bits of the port
        DATA_PORT &= 0xf0;
        TRIS_DATA_PORT |= 0x0f;
     #endif
#endif
        TRIS_RW = 0;                    // All control signals made outputs
        TRIS_RS = 0;
        TRIS_E = 0;
        RW_PIN = 0;                     // R/W pin made low
        RS_PIN = 0;                     // Register select pin made low
        E_PIN = 0;                      // Clock pin made low

        // Delay for 15ms to allow for LCD Power on reset
    DelayPORXLCD();

        // Setup interface to LCD
#ifdef BIT8                             // 8-bit mode interface
        TRIS_DATA_PORT = 0;             // Data port output
        DATA_PORT = 0b00110000;         // Function set cmd(8-bit interface)
#else                                   // 4-bit mode interface
    #ifdef UPPER                            // Upper nibble interface
        TRIS_DATA_PORT &= 0x0f;
        DATA_PORT &= 0x0f;
        DATA_PORT |= 0b00100000;        // Function set cmd(4-bit interface)
    #else                                   // Lower nibble interface
        TRIS_DATA_PORT &= 0xf0;
        DATA_PORT &= 0xf0;
        DATA_PORT |= 0b00000010;        // Function set cmd(4-bit interface)
    #endif
#endif
        E_PIN = 1;                      // Clock the cmd in
        Delay_1us();
        E_PIN = 0;
        // Delay for at least 4.1ms
    DelayXLCD();

        // Setup interface to LCD
#ifdef BIT8                             // 8-bit interface
        DATA_PORT = 0b00110000;         // Function set cmd(8-bit interface)
#else                                   // 4-bit interface
    #ifdef UPPER                            // Upper nibble interface
        DATA_PORT &= 0x0f;              // Function set cmd(4-bit interface)
        DATA_PORT |= 0b00100000;
    #else                                   // Lower nibble interface
        DATA_PORT &= 0xf0;              // Function set cmd(4-bit interface)
        DATA_PORT |= 0b00000010;
    #endif
#endif
        E_PIN = 1;                      // Clock the cmd in
        Delay_1us();
        E_PIN = 0;

        // Delay for at least 100us
    DelayXLCD();

        // Setup interface to LCD
#ifdef BIT8                             // 8-bit interface
        DATA_PORT = 0b00110000;         // Function set cmd(8-bit interface)
#else                                   // 4-bit interface
    #ifdef UPPER                            // Upper nibble interface
        DATA_PORT &= 0x0f;              // Function set cmd(4-bit interface)
        DATA_PORT |= 0b00100000;
    #else                                   // Lower nibble interface
        DATA_PORT &= 0xf0;              // Function set cmd(4-bit interface)
        DATA_PORT |= 0b00000010;
    #endif
#endif
        E_PIN = 1;                      // Clock cmd in
        Delay_1us();
        E_PIN = 0;

#ifdef BIT8                             // 8-bit interface
        TRIS_DATA_PORT = 0xff;          // Make data port input
#else                                   // 4-bit interface
    #ifdef UPPER                            // Upper nibble interface
        TRIS_DATA_PORT |= 0xf0;         // Make data nibble input
    #else                                   // Lower nibble interface
        TRIS_DATA_PORT |= 0x0f;         // Make data nibble input
    #endif
#endif

        // Set data interface width, # lines, font
        WriteCmdXLCD(lcdtype);          // Function set cmd
        WriteCmdXLCD(lcdtype);          // Function set cmd
        // Turn the display on then off
        WriteCmdXLCD(DOFF&CURSOR_OFF&BLINK_OFF);        // Display OFF/Blink OFF
        WriteCmdXLCD(DON&CURSOR_ON&BLINK_ON);           // Display ON/Blink ON
        // Clear display
        WriteCmdXLCD(0x01);                            // Clear display
        // Set entry mode inc, no shift
        WriteCmdXLCD(SHIFT_CUR_LEFT);                  // Entry Mode
        // Set DD Ram address to 0
        SetDDRamAddr(0);                                // Set Display data ram address to 0
        return;
}
void SetCGRamAddr(unsigned char CGaddr)
{
       while(BusyXLCD());
#ifdef BIT8                                     // 8-bit interface
        TRIS_DATA_PORT = 0;                     // Make data port ouput
        DATA_PORT = CGaddr | 0b01000000;        // Write cmd and address to port
        RW_PIN = 0;                             // Set control signals
        RS_PIN = 0;
        Delay_1us();
        E_PIN = 1;                              // Clock cmd and address in
        Delay_1us();
        E_PIN = 0;
        Delay_1us();
        TRIS_DATA_PORT = 0xff;                  // Make data port inputs
#else                                           // 4-bit interface
    #ifdef UPPER                                    // Upper nibble interface
        TRIS_DATA_PORT &= 0x0f;                 // Make nibble input
        DATA_PORT &= 0x0f;                      // and write upper nibble
        DATA_PORT |= ((CGaddr | 0b01000000) & 0xf0);
    #else                                           // Lower nibble interface
        TRIS_DATA_PORT &= 0xf0;                 // Make nibble input
        DATA_PORT &= 0xf0;                      // and write upper nibble
        DATA_PORT |= (((CGaddr | 0b01000000)>>4) & 0x0f);
    #endif
        RW_PIN = 0;                             // Set control signals
        RS_PIN = 0;
        Delay_1us();
        E_PIN = 1;                              // Clock cmd and address in
        Delay_1us();
        E_PIN = 0;
#ifdef UPPER                                    // Upper nibble interface
        DATA_PORT &= 0x0f;                      // Write lower nibble
        DATA_PORT |= ((CGaddr<<4)&0xf0);
#else                                           // Lower nibble interface
        DATA_PORT &= 0xf0;                      // Write lower nibble
        DATA_PORT |= (CGaddr&0x0f);
    #endif
        Delay_1us();
        E_PIN = 1;                              // Clock cmd and address in
        Delay_1us();
        E_PIN = 0;
    #ifdef UPPER                                    // Upper nibble interface
        TRIS_DATA_PORT |= 0xf0;                 // Make inputs
    #else                                           // Lower nibble interface
        TRIS_DATA_PORT |= 0x0f;                 // Make inputs
    #endif
#endif
        DelayExecution();
        return;
}

void SetDDRamAddr(unsigned char DDaddr)
{
       while(BusyXLCD());
#ifdef BIT8                                     // 8-bit interface
        TRIS_DATA_PORT = 0;                     // Make port output
        DATA_PORT = DDaddr | 0b10000000;        // Write cmd and address to port
        RW_PIN = 0;                             // Set the control bits
        RS_PIN = 0;
        Delay_1us();
        E_PIN = 1;                              // Clock the cmd and address in
        Delay_1us();
        E_PIN = 0;
        Delay_1us();
        TRIS_DATA_PORT = 0xff;                  // Make port input
#else                                           // 4-bit interface
    #ifdef UPPER                                    // Upper nibble  interface
        TRIS_DATA_PORT &= 0x0f;                 // Make port output
        DATA_PORT &= 0x0f;                      // and write upper nibble
        DATA_PORT |= ((DDaddr | 0b10000000) & 0xf0);
    #else                                           // Lower nibble interface
        TRIS_DATA_PORT &= 0xf0;                 // Make port output
        DATA_PORT &= 0xf0;                      // and write upper nibble
        DATA_PORT |= (((DDaddr | 0b10000000)>>4) & 0x0f);
    #endif
        RW_PIN = 0;                             // Set control bits
        RS_PIN = 0;
        Delay_1us();
        E_PIN = 1;                              // Clock the cmd and address in
        Delay_1us();
        E_PIN = 0;
    #ifdef UPPER                                    // Upper nibble interface
        DATA_PORT &= 0x0f;                      // Write lower nibble
        DATA_PORT |= ((DDaddr<<4)&0xf0);
    #else                                           // Lower nibble interface
        DATA_PORT &= 0xf0;                      // Write lower nibble
        DATA_PORT |= (DDaddr&0x0f);
    #endif
        Delay_1us();
        E_PIN = 1;                              // Clock the cmd and address in
        Delay_1us();
        E_PIN = 0;
    #ifdef UPPER                                    // Upper nibble interface
        TRIS_DATA_PORT |= 0xf0;                 // Make port input
    #else                                           // Lower nibble interface
        TRIS_DATA_PORT |= 0x0f;                 // Make port input
    #endif
#endif
        DelayExecution();
        return;
}

unsigned char BusyXLCD(void)
{
        RW_PIN = 1;                     // Set the control bits for read
        RS_PIN = 0;
        Delay_1us();
        E_PIN = 1;                      // Clock in the command
        Delay_1us();
#ifdef BIT8                             // 8-bit interface
        if(DATA_PORT&0x80)                      // Read bit 7 (busy bit)
        {                               // If high
                E_PIN = 0;              // Reset clock line
                RW_PIN = 0;             // Reset control line
                return 1;               // Return TRUE
        }
        else                            // Bit 7 low
        {
                E_PIN = 0;              // Reset clock line
                RW_PIN = 0;             // Reset control line
                return 0;               // Return FALSE
        }
#else                                   // 4-bit interface
    #ifdef UPPER                            // Upper nibble interface
        if( DATA_PORT & 0x80 )
    #else                                   // Lower nibble interface
     	if( DATA_PORT & 0x08 )
    #endif
        {
                E_PIN = 0;              // Reset clock line
                Delay_1us();
                E_PIN = 1;              // Clock out other nibble
                Delay_1us();
                E_PIN = 0;
                RW_PIN = 0;             // Reset control line
                return 1;               // Return TRUE
        }
        else                            // Busy bit is low
        {
                E_PIN = 0;              // Reset clock line
                Delay_1us();
                E_PIN = 1;              // Clock out other nibble
                Delay_1us();
                E_PIN = 0;
                RW_PIN = 0;             // Reset control line
                return 0;               // Return FALSE
        }
       
#endif
}

unsigned char ReadAddrXLCD(void)
{
        char data;                      // Holds the data retrieved from the LCD
        while(BusyXLCD());
        
#ifdef BIT8                             // 8-bit interface
        RW_PIN = 1;                     // Set control bits for the read
        RS_PIN = 0;
        Delay_1us();
        E_PIN = 1;                      // Clock data out of the LCD controller
        Delay_1us();
        data = DATA_PORT;               // Save the data in the register
        E_PIN = 0;
        RW_PIN = 0;                     // Reset the control bits
#else                                   // 4-bit interface
        RW_PIN = 1;                     // Set control bits for the read
        RS_PIN = 0;
        Delay_1us();
        E_PIN = 1;                      // Clock data out of the LCD controller
        Delay_1us();
    #ifdef UPPER                            // Upper nibble interface
        data = DATA_PORT&0xf0;          // Read the nibble into the upper nibble of data
    #else                                   // Lower nibble interface
        data = (DATA_PORT<<4)&0xf0;     // Read the nibble into the upper nibble of data
    #endif
        E_PIN = 0;                      // Reset the clock
        Delay_1us();
        E_PIN = 1;                      // Clock out the lower nibble
        Delay_1us();
    #ifdef UPPER                            // Upper nibble interface
        data |= (DATA_PORT>>4)&0x0f;    // Read the nibble into the lower nibble of data
    #else                                   // Lower nibble interface
        data |= DATA_PORT&0x0f;         // Read the nibble into the lower nibble of data
    #endif
        E_PIN = 0;
        RW_PIN = 0;                     // Reset the control lines
#endif
        return (data&0x7f);             // Return the address, Mask off the busy bit
}

char ReadDataXLCD(void)
{
        char data;
        
        while(BusyXLCD());
        
#ifdef BIT8                             // 8-bit interface
        RS_PIN = 1;                     // Set the control bits
        RW_PIN = 1;
        Delay_1us();
        E_PIN = 1;                      // Clock the data out of the LCD
        Delay_1us();
        data = DATA_PORT;               // Read the data
        E_PIN = 0;
        RS_PIN = 0;                     // Reset the control bits
        RW_PIN = 0;
#else                                   // 4-bit interface
        RW_PIN = 1;
        RS_PIN = 1;
        Delay_1us();
        E_PIN = 1;                      // Clock the data out of the LCD
        Delay_1us();
    #ifdef UPPER                            // Upper nibble interface
        data = DATA_PORT&0xf0;          // Read the upper nibble of data
    #else                                   // Lower nibble interface
        data = (DATA_PORT<<4)&0xf0;     // read the upper nibble of data
    #endif
        E_PIN = 0;                      // Reset the clock line
        Delay_1us();
        E_PIN = 1;                      // Clock the next nibble out of the LCD
        Delay_1us();
    #ifdef UPPER                            // Upper nibble interface
        data |= (DATA_PORT>>4)&0x0f;    // Read the lower nibble of data
    #else                                   // Lower nibble interface
        data |= DATA_PORT&0x0f;         // Read the lower nibble of data
    #endif
        E_PIN = 0;                                      
        RS_PIN = 0;                     // Reset the control bits
        RW_PIN = 0;
#endif  
        return(data);                   // Return the data byte
}

void WriteCmdXLCD(unsigned char cmd)
{
    while(BusyXLCD());
#ifdef BIT8                             // 8-bit interface
        TRIS_DATA_PORT = 0;             // Data port output
        DATA_PORT = cmd;                // Write command to data port
        RW_PIN = 0;                     // Set the control signals
        RS_PIN = 0;                     // for sending a command
        Delay_1us();
        E_PIN = 1;                      // Clock the command in
        Delay_1us();
        E_PIN = 0;
        Delay_1us();
        TRIS_DATA_PORT = 0xff;          // Data port input
#else                                   // 4-bit interface
    #ifdef UPPER                            // Upper nibble interface
        TRIS_DATA_PORT &= 0x0f;
        DATA_PORT &= 0x0f;
        DATA_PORT |= cmd&0xf0;
    #else                                   // Lower nibble interface
        TRIS_DATA_PORT &= 0xf0;
        DATA_PORT &= 0xf0;
        DATA_PORT |= (cmd>>4)&0x0f;
    #endif
        RW_PIN = 0;                     // Set control signals for command
        RS_PIN = 0;
        Delay_1us();
        E_PIN = 1;                      // Clock command in
        Delay_1us();
        E_PIN = 0;
    #ifdef UPPER                            // Upper nibble interface
        DATA_PORT &= 0x0f;
        DATA_PORT |= (cmd<<4)&0xf0;
    #else                                   // Lower nibble interface
        DATA_PORT &= 0xf0;
        DATA_PORT |= cmd&0x0f;
    #endif
        Delay_1us();
        E_PIN = 1;                      // Clock command in
        Delay_1us();
        E_PIN = 0;
    #ifdef UPPER                            // Make data nibble input
        TRIS_DATA_PORT |= 0xf0;
    #else
        TRIS_DATA_PORT |= 0x0f;
    #endif
#endif  
        DelayExecution();
        return;
}

void WriteDataXLCD(char data)
{
      while(BusyXLCD());
#ifdef BIT8                             // 8-bit interface
        TRIS_DATA_PORT = 0;             // Make port output
        DATA_PORT = data;               // Write data to port
        RS_PIN = 1;                     // Set control bits
        RW_PIN = 0;
        Delay_1us();
        E_PIN = 1;                      // Clock data into LCD
        Delay_1us();
        E_PIN = 0;
        RS_PIN = 0;                     // Reset control bits
        TRIS_DATA_PORT = 0xff;          // Make port input
#else                                   // 4-bit interface
    #ifdef UPPER                            // Upper nibble interface
        TRIS_DATA_PORT &= 0x0f;
        DATA_PORT &= 0x0f;
        DATA_PORT |= data&0xf0;
    #else                                   // Lower nibble interface
        TRIS_DATA_PORT &= 0xf0;
        DATA_PORT &= 0xf0;
        DATA_PORT |= ((data>>4)&0x0f);
    #endif
        RS_PIN = 1;                     // Set control bits
        RW_PIN = 0;
        Delay_1us();
        E_PIN = 1;                      // Clock nibble into LCD
        Delay_1us();
        E_PIN = 0;
        Delay_1us();

    #ifdef UPPER                            // Upper nibble interface
        DATA_PORT &= 0x0f;
        DATA_PORT |= ((data<<4)&0xf0);
    #else                                   // Lower nibble interface
        DATA_PORT &= 0xf0;
        DATA_PORT |= (data&0x0f);
    #endif
        Delay_1us();
        E_PIN = 1;                      // Clock nibble into LCD
        Delay_1us();
        E_PIN = 0;
    #ifdef UPPER                            // Upper nibble interface
        TRIS_DATA_PORT |= 0xf0;
    #else                                   // Lower nibble interface
        TRIS_DATA_PORT |= 0x0f;
    #endif
#endif
        DelayExecution();        
        return;
}

void putsXLCD(char *buffer)
{       while(*buffer)                  // Write data to LCD up to null
        {       WriteDataXLCD(*buffer); // Write character to LCD
                buffer++;               // Increment buffer
        }
        return;
}

void putrsXLCD(const char *buffer)
{        while(*buffer)                  // Write data to LCD up to null
        {       WriteDataXLCD(*buffer); // Write character to LCD
                buffer++;               // Increment buffer
        }
        return;
}

/**************** LCD SUPPORT FUNCTIONS *******************/
// Requerido por la biblioteca de funciones LCD 
// Proporcionar un retardo de 1us 
void Delay_1us( void ){__delay_us(2);return;}
// Proporcione al menos un retardo de 15 ms 
void DelayPORXLCD( void ){__delay_ms(25);return;}
// Proporcione al menos un retardo de 5 ms 
void DelayXLCD( void ){__delay_ms(7);return;}

void DelayExecution( void ){ __delay_us(100);return;} //  46us to Read/Write data operation

//              Fila , columna
void XLCDgotoXY(int x,int y)
{   if ( x>0 ){WriteCmdXLCD(DDRAM_LINE2+y);}
         else {WriteCmdXLCD(DDRAM_LINE1+y);}
    return;
}
// ----------------------------- Inicializacion del LCD 2x16 --------------------------------------------------
void XLCDInit(void)
{   DelayPORXLCD();                                  // retardo incial para que la tension de alimentacion se estabilice
    OpenXLCD(FOUR_BIT & LINES_5X7 );                 // Initialize LCD
    WriteCmdXLCD( DON & CURSOR_OFF & BLINK_OFF );    // Parametros de configuración (DISPLAY ON,CURSOR OFF, PARPADEO OFF)
    WriteCmdXLCD(CLEAR_XLCD);                        // LIMPIA EL LCD  
    WriteCmdXLCD(0x80);                              // Escribe el comando para poner el cursor a una dirección
                                                     // en la memoria DDRAM
                                                     // X=0 Y=0 esto seria equivalente a escribir XLCDgotoXY(0,0)
/* ----------------------------- Fin de Inicializacion del LCD 2x16 -------------------------------------------*/
}

void XLCD_WriteChr_CGRAM( const char *buffer, unsigned char Address)
{ unsigned char i=0;  
         SetCGRamAddr(Address*8);  	          // Set address
         for (i=0;i<8;i++)                    // Write data to CGRAM LCD
                {WriteDataXLCD(*buffer);      // Write character to LCD
                 ++buffer;
                }
}