/****************************************************************************/
/* This program exercises the PWM function on the LPC2148 board. */
/* The PWM frequency is set up for 100 Hz. The program initializes the */
/* Necessary registers to drive the RED LED of the board's tri-color LED */
/* with the PWM2 output. The program then varies the PWM duty cycle up and */
/* down with a slight delay in between changes. This give the sense of the */
/* dimming up and down over time. */
/* File name: Red RGB LED.c */
/****************************************************************************/
#include <LPC214X.H> /* LPX2148 Definitions file...includes the LPC2148 */
void delay1us(void)
{
  /* Setup timer #1 for delay of 1 microsecond */
  T1TCR = (1<<1); /* Stop and reset timer */
  T1PR = 0x00; /* Set prescaler to zero */
  T1MR0 = 15; /* Equals 1 microsecond based upon Pclk = 15 MHz. */
  T1IR = 0xff; /* Reset all the interrupt flags */
  T1MCR = (1<<2); /* Stop the timer on a match */
  T1TCR = (1<<0); /* Start the timer now */
  /* Wait until the timer has timed out */
  while (T1TCR & (1<<0));
} /* end of delay1us() */

int main(void)
{
int i; /* Counter to keep track of intensity */
	while(1)
	{
	    int PWM_Freq = 150000; /* This sets the frequency of the PWM waveform to 100 Hz. */
	    /* PCLK = 15 MHz., so with 150000 clocks per waveform cycle =
	    100 Hz.*/
	    PINSEL0 = (2<<14 | 2<<16 | 2<<18);
	    /* initialize PWM for operation */
	    PWMPR = 0x0; /* Set the PWM prescaler to 0 */
	    PWMMCR = (1<<1); /* The PWM counter resets on MR0 match...sets waveform basic
	    period */
	    PWMMR0 = PWM_Freq; /* MR0 = period cycle time for the PWM waveform...counts number
	    of PCLKS */
	    PWMPCR = (1<<10 | 1<<12); /* PWM enable for PWM2 only */
	    PWMLER = (1<<0); /* Set latch so MR0 gets loaded */
	    PWMTCR = (1<<0 | 1<<3); /* Enable the PWM and Counter to run */
	    
	    /**********************************************************/
	    /* This is the loop that creates a ramp-up in intensity */
	    /**********************************************************/
	    for(i=1; i<100000; i++)
	      {
	      PWMMR2 = i; /* Load duty cycle for PWM2 with ramping value */
				PWMMR4 = i;
	      PWMLER = (1<<2 | 1<<4); /* Cause it to load into the match register 2 */
	      delay1us(); /* Delay for a short time to make it visually pleasing */
	      }
	    /**********************************************************/
	    /* This is the loop that creates a ramp-down in intensity */
	    /**********************************************************/
	    for(i=75000; i>1; i--)
	      {
	      PWMMR2 = i; /* Load duty cycle for PWM2 with ramping value */
				PWMMR4 = i;
	      PWMLER = (1<<2 | 1<<4); /* Cause it to load into the match register 2 */
	      delay1us();
	      }
	} /* end of while(1) */
} /* end of main() */

    //shift left 14 
    //shift left 16
    //shift left 18