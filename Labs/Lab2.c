// Code for LAB 2, written by Yeray Lopez, Chris Thompson, and Chika Nwachukwu
// USB ports are COM3
#include <LPC214x.H>

/*void wait (void)
{
	int d;

	for (d = 0; d < 100000; d++);
}
*/
void delay(unsigned int d)
{

	T1TCR = 0x02; // reset timer
	T1PR = 0x00E4E1BF; //prescaler
	T1MR0 = d; //
	T1IR = 0xFF; // reset interrupts
	T1MCR = 0x00000001; // catch when MR0 = TC
	T1TCR = 0x01; // go
	while (T1TC < d);
	T1TCR = 0x00; // stop

}
int main (void)
{
	
	unsigned int i;
	IODIR0 = 0x0000FF00;
	IOSET0 = 0x0000FF00;
	while(1)
	{
		for (i = 1<<8; i < 1<<15; i <<= 1)
		{
			IOCLR0 = i;
			delay(1);
			IOSET0 = i;
		}
		for (i = 1 << 15; i > 1<<8; i >>= 1)
		{
			IOCLR0 = i;
			delay(1);
			IOSET0 = i;
		}
		/*IODIR0 = 0x0000FF00;
		IOSET0 = 0x0000FF00;
		delay(1);
		IOCLR0 = 0x0000FF00;
		delay(1);
		//IOSET0 = 1 << 12;
		//wait();*/
	}		
	

}