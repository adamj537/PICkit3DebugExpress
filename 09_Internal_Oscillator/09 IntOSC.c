//******************************************************************************
//Software License Agreement                                         
//                                                                    
//The software supplied herewith by Microchip Technology             
//Incorporated (the "Company") is intended and supplied to you, the  
//Company’s customer, for use solely and exclusively on Microchip    
//products. The software is owned by the Company and/or its supplier,
//and is protected under applicable copyright laws. All rights are   
//reserved. Any use in violation of the foregoing restrictions may   
//subject the user to criminal sanctions under applicable laws, as   
//well as to civil liability for the breach of the terms and         
//conditions of this license.                                        
//                                                                    
//THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,  
//WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED  
//TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A       
//PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,  
//IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR         
//CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.       
// *******************************************************************
// PIC18F45K20 PICkit 3 Debug Express Lesson 9 - Internal Oscillator
//
// This lesson demonstrates changing the speed of the microcontroller
// clock using the internal clock block from 250kHz to 64MHz.
//
// *******************************************************************
// *    See included documentation for Lesson instructions           *
// *******************************************************************

/** C O N F I G U R A T I O N   B I T S ******************************/

#pragma config FOSC = INTIO67, FCMEN = OFF, IESO = OFF					// CONFIG1H
#pragma config PWRT = OFF, BOREN = OFF, BORV = 30						// CONFIG2L
#pragma config WDTEN = OFF, WDTPS = 32768								// CONFIG2H
#pragma config MCLRE = ON, LPT1OSC = OFF, PBADEN = OFF, CCP2MX = PORTC	// CONFIG3H
#pragma config STVREN = ON, LVP = OFF, XINST = OFF						// CONFIG4L
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF				// CONFIG5L
#pragma config CPB = OFF, CPD = OFF										// CONFIG5H
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF			// CONFIG6L
#pragma config WRTB = OFF, WRTC = OFF, WRTD = OFF						// CONFIG6H
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF		// CONFIG7L
#pragma config EBTRB = OFF												// CONFIG7H

/** I N C L U D E S **************************************************/
#include <xc.h>
#include "09 IntOSC.h"	// header file

/** V A R I A B L E S *************************************************/
static unsigned char LED_Count = 0;		// 8-bit variable
static unsigned char SwitchDelay = 1;	// delay time to "debounce" switch
static IntOSCFreq ClockSpeed = FREQ_250_KHZ;

/** D E C L A R A T I O N S *******************************************/
#define _XTAL_FREQ 250000	// Set for 250 kHz

void main (void)
{
	int i;	// counter
	// Init I/O
	TRISD = 0b00000000;			// PORTD bits 7:0 are all outputs (0)
	TRISEbits.TRISE0 = 1;		// TRISE0 input

	INTCON2bits.RBPU = 0;		// enable PORTB internal pullups
	WPUBbits.WPUB0 = 1;			// enable pull up on RB0
	
	SetIntOSC(&ClockSpeed);		// Set initial clock speed (250kHz)

	SetupINT0Switch();			// Init switch
	EnableInterrupts();			// Turn on interrupts

	while (1)
	{	// delay and count on LEDs here.  Interrupt handles switch and freq changes
		LATD = LED_Count++;		// output count to PORTD LEDs

		for (i = 0; i < 10; i++)// delay 1 sec @ 250kHz
		{
			__delay_ms(100);
		}
	}
}

void SetupINT0Switch(void)
{	// Set up switch interrupt on INT0
	INTCON2bits.INTEDG0 = 0;	// interrupt on falling edge of INT0 (switch pressed)
	INTCONbits.INT0IF = 0;		// ensure flag is cleared
	INTCONbits.INT0IE = 1;		// enable INT0 interrupt
}

void EnableInterrupts(void)
{	// Set up global interrupts
	RCONbits.IPEN = 0;			// Disable priority levels on interrupts
	INTCONbits.PEIE = 1;		// Peripheral interrupts allowed (but none are used)
	INTCONbits.GIE = 1;			// Interrupting enabled.
}

void SetIntOSC(IntOSCFreq *clockSet)
{	// This function sets the internal oscillator to the frequency of
	// the ClockSet argument variable, and then increments clockSet
	// to the next supported frequency.
	switch (*clockSet)
	{
		case FREQ_250_KHZ:
			OSCCON = 0x10;			// IRCFx = 001
			OSCTUNEbits.PLLEN = 0;	// x4 PLL disabled
			*clockSet = FREQ_500_KHZ;
			SwitchDelay = 1;
			break;

		case FREQ_500_KHZ:
			OSCCON = 0x20;			// IRCFx = 010
			OSCTUNEbits.PLLEN = 0;	// x4 PLL disabled
			*clockSet = FREQ_1_MHZ;
			SwitchDelay = 2;
			break;

		case FREQ_1_MHZ:
			OSCCON = 0x30;			// IRCFx = 011
			OSCTUNEbits.PLLEN = 0;	// x4 PLL disabled
			*clockSet = FREQ_2_MHZ;
			SwitchDelay = 4;
			break;

		case FREQ_2_MHZ:
			OSCCON = 0x40;			// IRCFx = 100
			OSCTUNEbits.PLLEN = 0;	// x4 PLL disabled
			*clockSet = FREQ_4MHZ;
			SwitchDelay = 8;
			break;

		case FREQ_4MHZ:
			OSCCON = 0x50;			// IRCFx = 101
			OSCTUNEbits.PLLEN = 0;	// x4 PLL disabled
			*clockSet = FREQ_8_MHZ;
			SwitchDelay = 16;
			break;

		case FREQ_8_MHZ:
			OSCCON = 0x60;			// IRCFx = 110
			OSCTUNEbits.PLLEN = 0;	// x4 PLL disabled
			*clockSet = FREQ_16_MHZ;
			SwitchDelay = 32;
			break;

		case FREQ_16_MHZ:
			OSCCON = 0x70;			// IRCFx = 111
			OSCTUNEbits.PLLEN = 0;	// x4 PLL disabled
			*clockSet = FREQ_32_MHZ;
			SwitchDelay = 64;
			break;

		case FREQ_32_MHZ:
			OSCCON = 0x60;			// IRCFx = 110 (8 MHz)
			OSCTUNEbits.PLLEN = 1;	// x4 PLL enabled = 32MHz
			*clockSet = FREQ_64_MHZ;
			SwitchDelay = 128;
			break;

		case FREQ_64_MHZ:
			OSCCON = 0x70;			// IRCFx = 111 (16 MHz)
			OSCTUNEbits.PLLEN = 1;	// x4 PLL enabled = 64MHz
			*clockSet = FREQ_250_KHZ;
			SwitchDelay = 255;
			break;

		default:
			// should never get here, but just in case
			OSCCON = 0x10;			// IRCFx = 001
			OSCTUNEbits.PLLEN = 0;	// x4 PLL disabled
			*clockSet = FREQ_500_KHZ;
			break;
	}
}


// -------------------- Iterrupt Service Routines --------------------------
void interrupt InterruptService(void)
{
	int i;

	// Check to see what caused the interrupt
	// (Necessary when more than 1 interrupt at a priority level)

	// Check for INT0 interrupt
	if (INTCONbits.INT0IF)
	{
		// Delay about 50ms regardless of frequency to debounce switch.
		// NOTE: a delay function from delay.h isn't used as they are 
		// inline assembly and inline assembly in an interrupt routine
		// causes a very long context save/restore since the compiler
		// doesn't know what resources are being used and so saves everything.
		i = 125 * SwitchDelay;
		while(i)
		{
			i--;
		}
        
		// clear (reset) flag
		INTCONbits.INT0IF = 0;

		if (PORTBbits.RB0 == 0)
		{	// if it's still pressed after 100ms delay.
			// Change oscilator frequency
			SetIntOSC(&ClockSpeed);
		}
	}
	// Check for another interrupt, examples:
	// if (PIR1bits.TMR1IF)     // Timer 1
	// if (PIR1bits.ADIF)       // ADC

}  // return from high-priority interrupt

