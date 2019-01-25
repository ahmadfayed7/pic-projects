#include <avr/io.h> 
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "lcd.h"

#define PROGRESSPIXELS_PER_CHAR	6


#define RS_s LCD_CMD_PORT |= (1<<LCD_RS_PIN)
#define RS_c LCD_CMD_PORT &= ~(1<<LCD_RS_PIN)
#define E_s LCD_CMD_PORT |= (1<<LCD_E_PIN)
#define E_c LCD_CMD_PORT &= ~(1<<LCD_E_PIN)



unsigned char __attribute__ ((progmem)) LcdCustomChar[] =
{
	7,5,7,0,0,0,0,0, // 0. 0/5 full progress block
	15,16,7,8,16,0,0,6, // 1. 1/5 full progress block
	30,1,28,2,1,0,0,24, // 2. 2/5 full progress block
	0,0,16,8,8,30,15,15, // 3. 3/5 full progress block
	30,14,2,2,1,0,0,0, // 4. 4/5 full progress block
	0,0,0,8,4,19,8,7, // 5. 5/5 full progress block
	0,0,0,2,4,25,2,28, // 6. rewind arrow
	15,14,8,8,16,0,0,0, // 7. stop block
/*	0x18, 0x1C, 0x1E, 0x1F, 0x1E, 0x1C, 0x18, 0x00, // 9. fast-forward arrow
	0x00, 0x04, 0x04, 0x0E, 0x0E, 0x1F, 0x1F, 0x00, // 10. scroll up arrow
	0x00, 0x1F, 0x1F, 0x0E, 0x0E, 0x04, 0x04, 0x00, // 11. scroll down arrow
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 12. blank character
	0x00, 0x0E, 0x19, 0x15, 0x13, 0x0E, 0x00, 0x00,	// 13. animated play icon frame 0
	0x00, 0x0E, 0x15, 0x15, 0x15, 0x0E, 0x00, 0x00,	// 14. animated play icon frame 1
	0x00, 0x0E, 0x13, 0x15, 0x19, 0x0E, 0x00, 0x00,	// 15. animated play icon frame 2
	0x00, 0x0E, 0x11, 0x1F, 0x11, 0x0E, 0x00, 0x00,	// 16. animated play icon frame 3*/
};



void lcd_write(unsigned char val, unsigned char mode)
{
	if (mode == CMD) {RS_c;} else {RS_s;}
	
	LCD_DATA_PORT &= 0xF0; 
	LCD_DATA_PORT |= ((0xF0 & val)>>4);
	E_s;
	_delay_us(50);
	E_c;
	_delay_us(50);
	LCD_DATA_PORT &= 0xF0;
	LCD_DATA_PORT |= (0x0F & val);
	E_s;
	_delay_us(50);
	E_c;
	_delay_ms(2);
	
}

void LCDWriteChar(unsigned char val)
{
	lcd_write(val,DATA);
}

void LCDputs(const char *s)
{
    register char c;

    while ( (c = *s++) ) {
        LCDWriteChar(c);
    }

}

void lcd_gotoxy(uint8_t x, uint8_t y)
{
	if ( y==0 ) 
        lcd_write((1<<7)+0x00+x,CMD);
    else
        lcd_write((1<<7)+0x40+x,CMD);
}


void LCDcursorOn(void) //displays LCD cursor
{
	lcd_write(0x0E,CMD);
}

void LCDcursorOnBlink(void)	//displays LCD blinking cursor
{
	lcd_write(0x0F,CMD);
}

void LCDcursorOFF(void)	//turns OFF cursor
{
	lcd_write(0x0C,CMD);
}

void LCDblank(void)		//blanks LCD
{
	lcd_write(0x08,CMD);
}

void LCDvisible(void)		//Shows LCD
{
	lcd_write(0x0C,CMD);
}

void LCDcls(void)
{
	lcd_write(0x01,CMD);
}


void lcdLoadCustomChar(char* lcdCustomCharArray, char romCharNum, char lcdCharNum)
{
	register char i;

	// multiply the character index by 8
	lcdCharNum = (lcdCharNum<<3);	// each character occupies 8 bytes
	romCharNum = (romCharNum<<3);	// each character occupies 8 bytes

	// copy the 8 bytes into CG (character generator) RAM
	for(i=0; i<8; i++)
	{
		// set CG RAM address
		lcd_write((1<<6) | (lcdCharNum+i),CMD);
		// write character data
		lcd_write( pgm_read_byte(lcdCustomCharArray+romCharNum+i),DATA);
	}
}

void lcdProgressBar(short int progress, short int maxprogress, char length)
{
	char i;
	short int pixelprogress;
	char c;

	// draw a progress bar displaying (progress / maxprogress)
	// starting from the current cursor position
	// with a total length of "length" characters
	// ***note, LCD chars 0-5 must be programmed as the bar characters
	// char 0 = empty ... char 5 = full

	// total pixel length of bargraph equals length*PROGRESSPIXELS_PER_CHAR;
	// pixel length of bar itself is
	pixelprogress = ((progress*(length*PROGRESSPIXELS_PER_CHAR))/maxprogress);
	
	// print exactly "length" characters
	for(i=0; i<length; i++)
	{
		// check if this is a full block, or partial or empty
		// (u16) cast is needed to avoid sign comparison warning
		if( ((i*(unsigned short int)PROGRESSPIXELS_PER_CHAR)+5) > pixelprogress )
		{
			// this is a partial or empty block
			if( ((i*(unsigned short int)PROGRESSPIXELS_PER_CHAR)) > pixelprogress )
			{
				// this is an empty block
				// use space character?
				c = 0;
			}
			else
			{
				// this is a partial block
				c = pixelprogress % PROGRESSPIXELS_PER_CHAR;
			}
		}
		else
		{
			// this is a full block
			c = 5;
		}
		
		// write character to display
		LCDWriteChar(c);
	}

}

void lcdInit(void)
{
	LCD_DATA_PORT_DDR=0xff;
	_delay_ms(15);
	
	lcd_write(0x33,CMD);
	lcd_write(0x32,CMD);
	lcd_write(0x28,CMD);
	lcd_write(0x08,CMD);
	lcd_write(0x06,CMD);
	lcd_write(0x01,CMD);
	lcd_write(0x0f,CMD);

	lcdLoadCustomChar((char*)LcdCustomChar,0,0);
	lcdLoadCustomChar((char*)LcdCustomChar,1,1);
	lcdLoadCustomChar((char*)LcdCustomChar,2,2);
	lcdLoadCustomChar((char*)LcdCustomChar,3,3);
	lcdLoadCustomChar((char*)LcdCustomChar,4,4);
	lcdLoadCustomChar((char*)LcdCustomChar,5,5);
	lcdLoadCustomChar((char*)LcdCustomChar,6,6);
	lcdLoadCustomChar((char*)LcdCustomChar,7,7);
	
}

// Funkcija "briši"
void brisi(void)
{
	
	lcd_gotoxy(0,0);
	LCDputs("                  ");
	lcd_gotoxy(0,1);
	LCDputs("                   ");
	LCDcursorOFF();
}
///////////////////////////////////////////////////
void welcome(void)
{
	lcd_gotoxy(0,0);
	LCDputs("FlashElectronics");
	_delay_ms(300);
	lcd_gotoxy(0,1);
	LCDputs("Termometer Ver.3");
	LCDcursorOFF();
}
///////////////////////////////////////////////////
void stopinj1(void)
{	
	lcd_gotoxy(14,0);
	LCDWriteChar(0);
	lcd_gotoxy(15,0);
	LCDputs("C");
}
///////////////////////////////////////////////////
void stopinj2(void)
{	
	lcd_gotoxy(14,1);
	LCDWriteChar(0);
	lcd_gotoxy(15,1);
	LCDputs("C");
}

