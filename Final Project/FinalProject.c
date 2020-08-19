// Yeray Lopez, Chikaodri Nwachukwu, Chris Thompson

/* 
Final Project: Morse Code Translator
	Works by using the LCD lab as a base and incorparating 
	a morse code library to translate a string to morse code
	by displaying it on the LCD screen.
	
Current Code Functionality:
	- Displays Morse code in 16 char increments if the morse code doesnt fit
		it holds contents shortly and then clears and continues to write
	
	
TODO:
	- Make Speaker beep for different increments based on dots and dashes
		BONUS: if you can sync to the screen as it's writing that would be cool
		
Possible Improvements:
	- Print Original String on the bottom half of the LCD

Designs Notes:
	- Cleaned up the fuck out of the original functions in this code
		LCD functions can be found in LCD.h 
		morse libraries include morse.h & morse.c

*/ 

#include "LPC214x.h"	// LPC21xx Definition File...Includes LPC2148 
#include "LCD.h"		// Library used to control LCD
#include <string.h>		// String Functions used for strlen
#include <stdlib.h>		// Standard Library
#include "morse.h"      // Morse Code Translator Definitions
#include "morse.c"		// Morse Code Control Functions


// PIN SELECT and IODIR is intialized via this function for our purposes
// This can go back into main but is in function form for now to remove clutter
// I would personally consider this an overengineered function but it keeps main clean ~ Yeray

void PINSEL_IO_Init(void) {
	
	// Pin Setup
	PINSEL0 = 0x00000000;					/* All pins P0.0 to P0.15 set as GPIO */
	PINSEL1 = 0x05000000;					/* Define A/D pins: P0.28, P0.29 = AIN1, AIN2 */
	PINSEL2 = 0x00000000;					/* P1.0 - P1.36 as GPIO */
	
	// IODIR Setup
	IODIR0 = (255<<8 | 1<<21 | 1<<22 | 1<<30);	/* P0.8 - P0.15 as output - LEDs P0.22 P0.30 as Outputs - LCD 'R/W' and Backlight */																	/* P0.12 and P0.21 as outputs for stepper motor */
	IODIR1 = (255<<16 | 1<<24 | 1<<25);					/* P1.16 - P1.23 as outputs: LCD Data...P1.24 as output: RS...P1.25 as output E: */
	
}

// Prepares the LCD by intializing it, turning on the backlight and clearing the display

void LCD_Prep(void) {
		LCD_Init();	// Initialize the LCD Display to our operating conditions 
		IOSET0 = Backlight;	// Turn on the LCD Backlight 
		LCD_CommandWrite (CLEAR_DISP); // Clear the display 
}


int main(void)
{
	// YOU HAVE TO DECLARE ALL VARIABLES BEFORE RUNNING FUNCTIONS

	const char* str = "SOS PLS HALP"; // Input String
	const char* morseStr;   // Converted Morse String
	
	size_t n = strlen(str); // String Length of input
	size_t m;               // String Length of morse String
	size_t vCount = 0;      // Overflow count, MAX = 16 (LCD Width)
	
	int i;
	int j;
	
	PRINTDELAY = 300000; 		// Predefined to 200000Us = 200 ms
	
	
	PINSEL_IO_Init();     	// Intialize PINS & IODIR
	delayUs(8000); 			// Delay 8 ms to give LCD time to intialize internally

	LCD_Prep();				// Prepare LCD
	
	//LCD_DataWrite(n + '0'); // DEBUG: Print Length of inputted String
			
	// vCount increments everytime a character is printed on the LCD 
	// if it goes over 16 it rolls over to 0 and clears the LCD
	// it holds the input for a delayed amount and continues to print

	for (i = 0; i<n;i++) {
		
		// if string contains a space print a space
		// *** Morse Code library has issues handling spaces ***
		if (str[i] == ' ') {
			LCD_DataWrite(' ');
			vCount +=1;
			continue;
		}
		
		// Convert Current Char in string to morse code and store as char*
		// Store Length of morse code string
		morseStr = char_to_morse(str[i]);
		m = strlen(morseStr);
		
		vCount += m;
		
		if (vCount > 16) {
				delayUs(100000);
				LCD_CommandWrite (CLEAR_DISP);
				vCount = 0;
		}

		// Printer for morse code 
		for(j = 0; j < m; j++) {
			LCD_DataWrite(morseStr[j]);
		}
		
		// Spaces each morse code string
		LCD_DataWrite(' '); // 
		vCount +=1;
		
	}
	
	// Turns off Cursor
	LCD_CommandWrite((DISP | D | !C ));
	
	// Sit here and do nothing
	while(1);
	
}
