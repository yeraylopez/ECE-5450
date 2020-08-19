#ifndef _LCD_h
#define _LCD_h

/************************************************************/
/* The following function the code necessaty to write		*/
/* commands to the LCD display. To accomodate the time		*/
/* needed for the display clear code, we will delay 4 mS	*/
/* after the command is written.							*/
/************************************************************/


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

int PRINTDELAY = 200000;


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
		//int Backlight = (1<<30);
		
		IO1SET = 0x01000000; // RS = 1
		IO0CLR = 0x00400000; // RW = 0

		IO1SET = (Character <<16);

		IO1SET = 0x02000000;// Set E high
		IO1CLR = 0x02000000;// SET E Low
		delayUs(PRINTDELAY);    // Print Delay
		IO1CLR = 0x00FF0000; // Clear data pins
		IOSET0 = Backlight;
	} /* end of LCD_DataWrite */
		

void LCD_Init (void)
	{	
		LCD_CommandWrite (FUNC | DL | N);	/* Send Function Set command with 8-bit I/F, 2 lines, 5x7 font */
		LCD_CommandWrite (DISP | D | C);	/* Send Display On/Off command with Display ON, Cursor ON, Blink OFF */
		LCD_CommandWrite (ENTRY | I_D);		/* Send Entry Mode Set command with Increment Selected */
	}/* end of LCD_Init */


#endif

