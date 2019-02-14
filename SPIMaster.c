#include <xc.h>
#include "LCD.h"


#pragma config FOSC=HSPLL
#pragma config WDTEN=OFF
#pragma config XINST=OFF


/*
Connections:
    Master RD4 <-> Slave RD5
    Master RD5 <-> Slave RD4
    Master RD6 <-> Slave RD6
    Master RD7 <-> Slave RD7
    GND <-> GND
 */


void InitPins(void);
void ConfigInterrupts(void);
void ConfigPeriph(void);

unsigned char SPIReadWrite(unsigned char byte);

#define _XTAL_FREQ 32000000L

void main(void) {
    OSCTUNEbits.PLLEN = 1;
    LCDInit();
    LCDClear();
    InitPins();
    ConfigPeriph();
    lprintf(0, "SPI Master");

    while (1) {
        //Master code here
    }
}

unsigned char SPIReadWrite(unsigned char byte) {
    unsigned char r;
    SSP2BUF = byte; //transmit byte
    while (SSP2STATbits.BF == 0); //Wait for tx/rx
    r = SSP2BUF; //read received byte
    return r;
}

void InitPins(void) {
    LATD = 0; //LED's are outputs
    TRISD = 0; //Turn off all LED's

    TRISD = 0b00100000; //MMSP2 uses RD4 as SDO, RD5 as SDI, RD6 as SCK
    LATDbits.LATD7 = 1; //Use RD7 as SS
}

void ConfigPeriph(void) {

    //Configure peripherals here

    SSP2STATbits.CKE = 1;
    SSP2CON1bits.CKP = 0; //SPI mode 0,0
    SSP2CON1bits.SSPM = 0b0001; //SPI Master - FOSC/16 = 2 MHz
    SSP2CON1bits.SSPEN = 1; //Enable MSSP
}

void ConfigInterrupts(void) {
    //Configure any interrupts here
}

void __interrupt(high_priority) HighIsr(void) {
    //Interrupt code goes here if needed
}


