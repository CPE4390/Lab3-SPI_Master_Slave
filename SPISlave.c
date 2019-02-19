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
unsigned int ReadPot(void);
void ConfigInterrupts(void);
void ConfigPeriph(void);


#define _XTAL_FREQ   32000000L

unsigned int count;
int state = 0;
unsigned char command;

void main(void) {
    long i;
    count = 0;
    OSCTUNEbits.PLLEN = 1;
    LCDInit();
    LCDClear();
    InitPins();
    ConfigPeriph();
    ConfigInterrupts();
    while (1) {
        lprintf(0, "%d", count);
        __delay_ms(1000);
        ++count;
    }
}

void InitPins(void) {
    LATD = 0; //LED's are outputs
    TRISD = 0; //Turn off all LED's


    //Set TRIS bits for any required peripherals here.

    TRISD = 0b11100000; //MMSP2 uses RD4 as SDO, RD5 as SDI, RD6 as SCK, RD7 is SS

    //Set up for ADC
    TRISA = 0b00000001;
    ADCON1 = 0b10111010; //Right justify, No calibration, 20 Tad, FOSC/32
    WDTCONbits.ADSHR = 1; //Switch to alternate address to access ANCON0
    ANCON0 = 0b11111110; //AN0 analog - all others digital
    WDTCONbits.ADSHR = 0; //Back to default address
}

unsigned int ReadPot(void) {
    ADCON0bits.CHS = 0; //channel 0
    ADCON0bits.ADON = 1;
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO == 1);
    ADCON0bits.ADON = 0;
    return ADRES;
}

void ConfigInterrupts(void) {

    RCONbits.IPEN = 0; //no priorities.  This is the default.

    //Configure your interrupts here

    SSP2IF = 0;
    SSP2IE = 1;

    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1; //Turn on interrupts
}

void ConfigPeriph(void) {

    //Configure peripherals here

    SSP2STATbits.CKE = 1;
    SSP2CON1bits.CKP = 0; //SPI mod 0,0
    SSP2CON1bits.SSPM = 0b0100; //SPI Slave with SS
    SSP2CON1bits.SSPEN = 1; //Enable MSSP
}

void __interrupt(high_priority) HighIsr(void) {
    //Check the source of the interrupt
    if (SSP2IF) {
        //Handle SPI interrupt here
        unsigned char rx = SSP2BUF;  //Always need to read the buffer       
        
        //State machine goes here
        
        SSP2IF = 0;
    }
}


