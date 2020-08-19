// Yeray Lopez, Chikaodri Nwachukwu, Chris Thompson

/************************************************************************/
/* This program provides the necessary code for displaying information	*/
/* on the LPC2148 Educational Board. The LCD on the board is based upon	*/
/* the Samsung SK0070B LCD controller chip. Refer to the datasheet for	*/
/* that chip for programming information. 								*/
/* The hardware on the LPC2148 board for the LCD interface is:			*/
/*	LCD D0 - D7 = P1.16 - P1.23, respectively							*/
/*	LCD RS = P1.24 ... LCD R/W = P0.22 ... LCD E = P1.25 ...			*/
/*	LCD Backlight  = P0.30....High lights the display.					*/
/*  This program intializes the LCD controller for:						*/
/*	8-bit interface ... 2 lines ... 5x7 dot matrix font ...				*/
/*	Display and Cursor ON ... Blink OFF ... Entry Mode is Increment		*/
/************************************************************************/


#include "LPC214x.h"				/* LPC21xx Definition File...Includes LPC2148 */

	int RS = 0;	/* Bit mask for RS pin on LCD = P1.24 */
	int E =  0;	/* Bit mask for E pin on LCD = P1.25 */
	int RW = 0;	/* Bit mask for RW pin on LCD = P0.22 */
	int Backlight = (1<<30);	/* Bit mask for Backlight pin on LCD = P0.30 */
	int DL = (1<<4);	/* Bit position for DL bit in Function Set Command  SET FOR 8 BITS*/
	int N = (1<<3);		/* 2-line if 1 */
	int F = (0<<2);		/* 5 * 10 if 1 */
	int I_D = (0<<1);	/* cursor blink move to right if 1 */
	int SH = (0<<0);	/* shift entire display if 1 */
	int D = (1<<2);		/* Display On/Off Command */
	int C = (1<<1);		/* Cursor on/off*/
	int B = (1<<0);		/* Blink */
	int FUNC = (1<<5);	/* Function Mode Set Command */
	int ENTRY = (1<<2);	/* Entry Mode Set Command */
	int DISP = (1<<3);	/* Display On/Off Command) */
	int CLEAR_DISP = 0x01;	/* Clear LCD Display Command */

/************************************************************/
/* The following function implements a time delay using the	*/
/* Timer T1 in a polled mode.								*/
/* The delay amount is in terms of number of microseconds.	*/
/************************************************************/
void delayUs(unsigned int delayInus)
{

			/* Setup timer #1 for delay in the order of microseconds */
  T1TCR = 0x02;          					/* Stop and reset timer */
  T1PR  = 0x00;								/* Set prescaler to zero */
  T1MR0 = delayInus * 15;					/* Based upon Pclk = 15 MHz. */
  T1IR  = 0xff;								/* Reset all the interrupt flags */
  T1MCR = 0x04;								/* Stop the timer on a match */
  T1TCR = 0x01;								/* Start the timer now */
  
			/* Wait until the timer has timed out TCR[0] will be set 0 when MR[2] = 1*/
  while (T1TCR & 0x01);

} /* end of delayInus */



/************************************************************/
/* The following function the code necessaty to write		*/
/* commands to the LCD display. To accomodate the time		*/
/* needed for the display clear code, we will delay 4 mS	*/
/* after the command is written.							*/
/************************************************************/

void LCD_CommandWrite (unsigned int command)	
	{
		IO1CLR = 0x01000000; // RS=0,
		IO0CLR = 0x00400000; // RW=0,
		IO1SET = (command <<16); // write command
		IO1SET = 0x02000000;// Set E high
		IO1CLR = 0x02000000;// SET E Low
		delayUs(4000);         //DELAY  4ms
		IO1CLR = 0x00FF0000; // Clear data pins
				
	} /* end of LCD_CommandWrite */



/************************************************************/
/* The following function provides code necessary to write	*/
/* data to the LCD display. To accomodate the time			*/
/* needed for displaying a charatecr, we will delay 100 uS	*/
/* after the command is written.							*/
/************************************************************/

void LCD_DataWrite (unsigned int Character)
	{ 
		IO1SET = 0x01000000; // RS = 1
		IO0CLR = 0x00400000; // RW = 0

		IO1SET = (Character <<16);

		IO1SET = 0x02000000;// Set E high
		IO1CLR = 0x02000000;// SET E Low
		delayUs(4000);         //DELAY  4ms
		IO1CLR = 0x00FF0000; // Clear data pins
	} /* end of LCD_DataWrite */
		

void LCD_Init (void)
	{
		LCD_CommandWrite (FUNC | DL | N);	/* Send Function Set command with 8-bit I/F, 2 lines, 5x7 font */
		LCD_CommandWrite (DISP | D | C);	/* Send Display On/Off command with Display ON, Cursor ON, Blink OFF */
		LCD_CommandWrite (ENTRY | I_D);		/* Send Entry Mode Set command with Increment Selected */
	}/* end of LCD_Init */



int main(void)
{

	PINSEL0 = 0x00000000;					/* All pins P0.0 to P0.15 set as GPIO */
	PINSEL1 = 0x05000000;					/* Define A/D pins: P0.28, P0.29 = AIN1, AIN2 */
	PINSEL2 = 0x00000000;					/* P1.0 - P1.36 as GPIO */

	IODIR0 = (255<<8 | 1<<21 | 1<<22 | 1<<30);	/* P0.8 - P0.15 as output - LEDs P0.22 P0.30 as Outputs - LCD 'R/W' and Backlight */
											/* P0.12 and P0.21 as outputs for stepper motor */
	IODIR1 = (255<<16 | 1<<24 | 1<<25);		/* P1.16 - P1.23 as outputs: LCD Data...P1.24 as output: RS...P1.25 as output E: */
	IOSET0 = Backlight;	// Turn on the LCD Backlight
	//LCD_DataWrite ('E');
/************************************/
/* Here is the main program loop.	*/
/************************************/

	while (1)
	{
		delayUs(8000);	// Delay 8 ms to give LCD controller time to intialize internally 

		LCD_Init();	// Initialize the LCD Display to our operating conditions 

		IOSET0 = Backlight;	// Turn on the LCD Backlight 

		LCD_CommandWrite (CLEAR_DISP);	// Clear the display 
		LCD_DataWrite ('G');	// Write a 'E' onto the LCD display in 1st location 
		LCD_DataWrite ('G');	// Write a 'C' onto the LCD display in 2nd location 
		LCD_DataWrite ('E');	// Write a 'E' onto the LCD display in 3rd location 
		LCD_DataWrite ('Z');
		LCD_DataWrite (' ');
		LCD_DataWrite ('B');
		LCD_DataWrite ('O');
		LCD_DataWrite ('I');
		// To be continued for writing 5450... 
		
		LCD_CommandWrite((DISP | D | !C )); 

		while(1);	// Now sit here and do nothing else 

	} // end of main
	
}