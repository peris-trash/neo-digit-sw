/* 
 * File:   main.c
 * Author: Rafa
 *
 * Created on January 3, 2019, 5:23 PM
 */

// PIC18F45K42 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config FEXTOSC = OFF    // External Oscillator Selection (Oscillator not enabled)
#pragma config RSTOSC = HFINTOSC_64MHZ// Reset Oscillator Selection (HFINTOSC with HFFRQ = 64 MHz and CDIV = 1:1)

// CONFIG1H
#pragma config CLKOUTEN = OFF    // Clock out Enable bit (CLKOUT function is disabled)
#pragma config PR1WAY = ON      // PRLOCKED One-Way Set Enable bit (PRLOCK bit can be cleared and set only once)
#pragma config CSWEN = OFF      // Clock Switch Enable bit (The NOSC and NDIV bits cannot be changed by user software)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)

// CONFIG2L
#pragma config MCLRE = EXTMCLR  // MCLR Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTS = PWRT_OFF // Power-up timer selection bits (PWRT is disabled)
#pragma config MVECEN = ON      // Multi-vector enable bit (Multi-vector enabled, Vector table used for interrupts)
#pragma config IVT1WAY = ON     // IVTLOCK bit One-way set enable bit (IVTLOCK bit can be cleared and set only once)
#pragma config LPBOREN = OFF    // Low Power BOR Enable bit (ULPBOR disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled , SBOREN bit is ignored)

// CONFIG2H
#pragma config BORV = VBOR_2P45 // Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR) set to 2.45V)
#pragma config ZCD = OFF        // ZCD Disable bit (ZCD disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = OFF    // PPSLOCK bit One-Way Set Enable bit (PPSLOCK bit can be set and cleared repeatedly (subject to the unlock sequence))
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config DEBUG = OFF      // Debugger Enable bit (Background debugger disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period selection bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled; SWDTEN is ignored)

// CONFIG3H
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4L
#pragma config BBSIZE = BBSIZE_512// Boot Block Size selection bits (Boot Block size is 512 words)
#pragma config BBEN = OFF       // Boot Block enable bit (Boot block disabled)
#pragma config SAFEN = OFF      // Storage Area Flash enable bit (SAF disabled)
#pragma config WRTAPP = OFF     // Application Block write protection bit (Application Block not write protected)

// CONFIG4H
#pragma config WRTB = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-30000Bh) not write-protected)
#pragma config WRTC = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)
#pragma config WRTSAF = OFF     // SAF Write protection bit (SAF not Write Protected)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored)

// CONFIG5L
#pragma config CP = OFF         // PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code protection disabled)

// CONFIG5H

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ 64000000ul

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "color.h"

#define RED_OUT_REG     LATA		// red enable pins are located on the PORTA (1 through 7)
#define GREEN_OUT_REG	LATD	// green enable pins are located on the PORTC (1 through 7)
#define BLUE_OUT_REG	LATC	// blue enable pins are located on the PORTD (1 through 7)

struct registers
{
    
    volatile uint8_t control_reg;	// timer counter register
    volatile uint8_t status_reg;	// timer counter register

    volatile uint8_t counter;	// timer counter register

    volatile uint8_t red_reg;	// red timer compare register
    volatile uint8_t green_reg;	// green timer compare register
    volatile uint8_t blue_reg;	// blue timer compare register

    volatile uint8_t enable; // segment enable register
    /*
     * bit 7: segment g
     * bit 6: segment f
     * bit 5: segment e
     * bit 4: segment d
     * bit 3: segment c
     * bit 2: segment b
     * bit 1: segment a
     * bit 0: unimplemented 
     */

};

struct registers REG;

static inline void set_color(COLOR color);

void __interrupt(irq(IRQ_TMR0), high_priority) timer_tick(void)
{
	if(REG.counter == 255)
	{
		REG.counter = 0;
		RED_OUT_REG |= (REG.enable & 0xFE);
		GREEN_OUT_REG |= (REG.enable & 0xFE);
		BLUE_OUT_REG |= (REG.enable & 0xFE);
	}
	else REG.counter++;
	
	if(REG.counter == REG.red_reg) RED_OUT_REG &= 0x01;

	if(REG.counter == REG.green_reg) GREEN_OUT_REG &= 0x01;

	if(REG.counter == REG.blue_reg) BLUE_OUT_REG &= 0x01;
	
	PIR3bits.TMR0IF = 0;
}

void __interrupt(irq(default)) DEFAULT_ISR(void)
{
	// unhandled interrupts go here
}

void main(void)
{
	// variable init ___________________________________________
	
	REG.counter = 0x00;

	REG.red_reg = 0x7F;
	REG.green_reg = 0x7F;
	REG.blue_reg = 0x7F;

	REG.enable = 0xFF; // 0b 0000 0010 seg a is enabled
	
	// I/O init ____________________________________________________
	
	// PORT A
	ANSELA = 0x00;	// Set PORT A as digital pins
	LATA = 0x00;	// Set PORT A output state low
	TRISA = 0x01;	// Set PORT A as outputs excluding PA0 (SS)
	
	// PORT B
	ANSELB = 0x00;	// Set PORT B as digital pins
	LATB = 0x00;	// Set PORT B output state low
	TRISB = 0xDF;	// Set PORT B as inputs excluding PB0 (SDO)
	
	// PORT C
	ANSELC = 0x00;	// Set PORT C as digital pins
	LATC = 0x00;	// Set PORT C output state low
	TRISC = 0x01;	// Set PORT C as outputs excluding PC0 (unused)
	
	// PORT D
	ANSELD = 0x00;	// Set PORT D as digital pins
	LATD = 0x00;	// Set PORT D output state low
	TRISD = 0x01;	// Set PORT D as outputs excluding PD0 (unused)
	
	// PORT E
	ANSELE = 0x00;	// Set PORT E as digital pins
	LATE = 0x00;	// Set PORT E output state low
	TRISE = 0xFF;	// Set PORT E as inputs
	
	// PPS
    
	PPSLOCKbits.PPSLOCKED = 0;	// unlock changes to PPS module
	
	//inputs
	SPI1SCKPPS = 0b001110;	// 001 (PBx) 110 (Px6) SPI1 SCK is PB6 (ICSPCLK) pin 16
	SPI1SDIPPS = 0b001111;	// 001 (PBx) 111 (Px7) SPI1 SDI is PB7 (ICSPDAT) pin 17
	SPI1SSPPS = 0b000000;	// 000 (PAx) 000 (Px0) SPI1 SS is PA0 pin 19
	
	//outputs
	RB5PPS = 0b011111;		// RB5 - 0b01 1111 SPI1 (SDO)
	
	PPSLOCKbits.PPSLOCKED = 1;	// lock changes to PPS module
	
	// SPI init __________________________________________________
	
	SPI1CON1bits.SDOP = 1;	// SDI Output Polarity Control bit (1 = SDO output is active-low)
	SPI1CON1bits.SDIP = 1;	// SDI Input Polarity Control bit (1 = SDI input is active-low)
	SPI1CON1bits.SSP = 1;	// SS Input/Output Polarity Control bit (1 = SS is active-low)
	SPI1CON1bits.CKP = 1;	// Clock Polarity Select bit (1 = Idle state for SCK is high level)
	SPI1CON1bits.CKE = 0;	// Clock Edge Select bit (0 = Output data changes on transition from idle to active clock state)
	
	SPI1CON0bits.MST = 0;	// SPI Operating Mode Master Select bit	(0 = SPI module operates as a bus slave)
	SPI1CON0bits.LSBF = 0;	// LSb-First Data Exchange bit (0 = Data is exchanged MSb first (traditional SPI operation))
	
	SPI1CON2bits.RXR = 0;	// Receive FIFO Space-Required Control bit (0 = Received data is not stored in the FIFO)
	SPI1CON2bits.TXR = 0;	// Transmit Data-Required Control bit (0 = TxFIFO data is not required for a transfer)
	SPI1CON2bits.SSET = 0;	// Slave Select Enable bit (0 = SS(in) enables/disables data input and tri-states SDO if the TRIS bit associated with the SDO pin)
	
	SPI1CLKbits.CLKSEL = 0b0001; //  SPI Clock Source Selection bits (HFINTOSC)
	
	SPI1BAUD = 0x00; //  Baud Clock Prescaler Select bits
	
	SPI1CON0bits.EN = 1; // SPI Module Enable Control bit (1 =SPI is enabled)
	
	// Timer init _________________________________________________
	
	// frequency target 1kHz
	// Fosc = 64MHz
	// Timer clock source 510kHz
	// prescaler out freq 32MHz
	// 32M / 510k = 62
	
	T0CON0bits.OUTPS = 0b000;	// TMR0 Output Postscaler (Divider) (0000 = 1:1 Postscaler)
	T0CON0bits.MD16 = 0;		// TMR0 Operating as 8-Bit Timer 
	
	T0CON1bits.CKPS = 0b0001;	//  Prescaler Rate Select 0001 = 1:2
	T0CON1bits.ASYNC = 1;		// TMR0 counter is not synchronized to system clocks
	T0CON1bits.CS = 0b011;		// Timer0 Clock Source 011 = HFINTOSC (64 MHz)
	
	TMR0L = 0x00;		// clear timer counter
	
	TMR0H = 124;		// set timer period
	
	T0CON0bits.EN = 1;		// TMR0 Enable
	
	// interrupt init ___________________________________________
	
	// general interrupt config
	INTCON0bits.IPEN = 1;		// Enable interrupt priority
	INTCON0bits.GIEH = 1;	// Enable high priority interrupts
	INTCON0bits.GIEL = 1;	// Enable low priority interrupts
	
	// timer interrupt config
	IPR3bits.TMR0IP = 1;	// Make TMR0 interrupt low priority
	PIE3bits.TMR0IE = 1;	// Enable TMR0 interrupt
	PIR3bits.TMR0IF = 0;  // Clear interrupt request Flag
    
	ei();	// enable interrupts
    
	for(;;)
	{ 
        
        set_color(color_rgb(255,0,0));
		__delay_ms(400);
        set_color(color_rgb(0,255,0));
		__delay_ms(400);
        set_color(color_rgb(0,0,255));
		__delay_ms(400);
        
        for(uint8_t c = 127; c < 254; c++)  
        {  
           set_color(color_rgb(gamma[c],gamma[c],gamma[c]));
            __delay_ms(10);
        }
        
        for(uint8_t c = 255; c > 127; c--)  
        {  
            set_color(color_rgb(gamma[c],gamma[c],gamma[c]));
            __delay_ms(10);
        }
        
        set_color(color_Hex(navy));
		__delay_ms(400);
        
        set_color(color_Hex(blue));
		__delay_ms(400);
        
        set_color(color_Hex(aqua));
		__delay_ms(400);
        
        set_color(color_Hex(teal));
		__delay_ms(400);
	
        set_color(color_Hex(olive));
		__delay_ms(400);
        
        set_color(color_Hex(green));
		__delay_ms(400);
        
        set_color(color_Hex(lime));
		__delay_ms(400);
        
        set_color(color_Hex(yellow));
		__delay_ms(400);
        
        set_color(color_Hex(orange));
		__delay_ms(400);
        
        set_color(color_Hex(red));
		__delay_ms(400);
        
        set_color(color_Hex(marron));
		__delay_ms(400);
	
        set_color(color_Hex(fushcia));
		__delay_ms(400);
        
        set_color(color_Hex(purple));
		__delay_ms(400);
        
        
        uint8_t state = 0;
        uint8_t a = 255;
        
        REG.red_reg = 255;
        REG.green_reg = 0;
        REG.blue_reg = 0;
        
        for(uint8_t loop = 0; loop < 5; loop++)
        {
            if(state == 0){
                REG.green_reg++;
                if(REG.green_reg == 255)
                    state = 1;
            }
            if(state == 1){
                REG.red_reg--;
                if(REG.red_reg == 0)
                    state = 2;
            }
            if(state == 2){
                REG.blue_reg++;
                if(REG.blue_reg == 255)
                    state = 3;
            }
            if(state == 3){
                REG.green_reg--;
                if(REG.green_reg == 0)
                    state = 4;
            }
            if(state == 4){
                REG.red_reg++;
                if(REG.red_reg == 255)
                    state = 5;
            }
            if(state == 5){
                REG.blue_reg--;
                if(REG.blue_reg == 0)
                    state = 0;
            }
            __delay_us(500);
        }
    }
}

static inline void set_color(COLOR color)
{
    REG.red_reg = color.r;
    REG.green_reg = color.g;
    REG.blue_reg = color.b;
}

