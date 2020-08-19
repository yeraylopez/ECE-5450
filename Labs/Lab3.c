// Code for LAB 3, written by Yeray Lopez, Christopher Thompson, and Chikaodiri Nwachukwu
#include <LPC214x.H>

/* LAB 3 REQUIREMENTS

In your delay routine, instead of using a defined constant for the
delay amount, use a value obtained from the A/D converter
• Use potentiometer connected to AIN2 (P0.29)
• Potentiometer generates different voltages.
• As you change the potentiometer setting, the speed of the LED pattern will
speed up and slow down.
• Set the ADC@ 10bit, 2.5MHz (resolution and rate)

*/

unsigned int ADCread() // reads the AD converter and returns a value
{
	unsigned int resultADC, adcdata, usabledata; //declare variables
	//PINSEL1 = 0x04000000; // select pin ad0.2
	AD0CR = 0x00210504; // values to select pin AD0.2 and the CLK DIV value and set PDN
	// Start conversion
	//AD0CR = AD0CR | (1 << 24); // start 
	while(!((resultADC = AD0GDR) & 0x80000000)) //while its still converting, do nothing
	{
	} 
	adcdata = (resultADC >> 6) & 0x000003FF; // convert to a usable number
	usabledata = (0x000003FF - adcdata) + 1; //so when we turn the potentiometer it increases instead of decreasing
	return(usabledata); // what will go into the delay function later
}

void initialize()
{
	PINSEL1 = 1 << 26; // select pin ad0.2
}

void delay(unsigned int d)
{
	T1TCR = 0x02; // reset timer
	T1PR = 0x00; // set prescaler to 0
	T1MR0 = d * 15; // PCLK = 15MHz
	T1IR = 0xFF; // reset interrupts
	T1MCR = 0x04; // catch when MR0 = TC
	T1TCR = 0x01; // go
	/* Wait until the timer has timed out TCR[0] will be set 0 when MR[2] = 1*/
	while (T1TCR & 0x01);
}

int main (void)
{
	
	unsigned int i, dval, adcon; 
	i = 0;

	IODIR0 = 0x0000FF00;
	IOSET0 = 0x0000FF00;
	initialize(); // select the pin

	while(1)
	{
		AD0CR = AD0CR | (1 << 24); // start 
		adcon = ADCread(); // get the reading from ADC
		dval = adcon *100; //input for delay
		for (i = 1<<8; i < 1<<15; i <<= 1)
		{
			IOCLR0 = i;
			delay(dval);
			IOSET0 = i;
		}
		for (i = 1 << 15; i > 1<<8; i >>= 1)
		{
			IOCLR0 = i;
			delay(dval);
			IOSET0 = i;
		}
		
	}		
	

}