#include <LPC214x.H>
/* 
Lab #1
Members: Chikaodiri Nwachukwu, Chris Thompson, Yeray Lopez 
Date: 10/02/19
*/
void wait (void) {
	 int d;
	 
	
	 for (d = 0; d<1000000; d++);

}
int main (void) {
	while(1) {
	unsigned int i;
	IODIR0 = 0x0000FF00;
	IOSET0 = 0x0000FF00;
	wait();
	IOCLR0 = 0x0000FF00;
	wait();
	IOSET0 = 1 << 12;
	}
}