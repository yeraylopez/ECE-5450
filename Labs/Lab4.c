// Code for LAB 4, written by Yeray Lopez, Christopher Thompson, and Chikaodiri Nwachukwu
#include <LPC214x.H>

/* 
Tonight’s Lab Assignment
• Start with my code we just went over in detail
• Add your own code functions called: ‘reverse’ and ‘forward’
• No parameters passed to it
• No parameters passed from it
• Use a step counter of ’50’ for each direction of the motor (see flow
chart)

*/

void wait(void) // wait function
{
	int d;
	int speed = 50000; // amount of delay and speed of rotation
	for(d = 0; d < speed; d++);
}

void forward(void)
{
	unsigned int stateAB, stateA, stateB;
	stateAB = (1 << 21 | 1 << 12);
	stateA = (1 << 12);
	stateB = (1 << 21);
	IODIR0 = stateAB;
	IOSET0 = stateAB;
	wait();
	IOCLR0 = stateA;
	wait();
	IOCLR0 = stateAB;
	wait();
	IOSET0 = stateA;
	wait();
	IOSET0 = stateB;

}

void reverse(void)
{
	unsigned int stateAB, stateA, stateB;
	stateAB = (1 << 21 | 1 << 12);
	stateA = (1 << 12);
	stateB = (1 << 21);
	IODIR0 = stateAB;
	IOSET0 = stateAB;
	wait();
	IOCLR0 = stateB;
	wait();
	IOCLR0 = stateAB;
	wait();
	IOSET0 = stateB;
	wait();
	IOSET0 = stateA;

}

int main(void)
{
	unsigned int stepCount = 50;
	int i, j;
	while(1)
	{
		for(i = 0; i < stepCount; i++)
		{
			forward();
		}

		for(j = 0; j < stepCount; j++)
		{
			reverse();
		}
	}

}