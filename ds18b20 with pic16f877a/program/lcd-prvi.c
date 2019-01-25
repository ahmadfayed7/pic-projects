#include <avr\io.h>
#include "lcd.h"
#include <util\delay.h>
#include <avr/pgmspace.h>
#include "therm_ds18b20.h"

#define THERM_DECIMAL_STEPS_12BIT 5	 //.05
#define THERM2_DECIMAL_STEPS_12BIT 5	 //.05
#define F_CPU 16000000UL						//Definiramo 16MHz kristal

#define BLPORT PORTC                           //Definiramo port na katerem je BL
#define BL 2                                  //Definiramo na katerem pinu je BL
#define BL_on BLPORT |= (1<<BL)              //Macro za vžig BL
#define BL_off BLPORT &= ~(1<<BL)            //Macro za izklop BL

#define tipka (!(PINC & (1<<4))) 

unsigned char A;

int main(void)
{
	//Tu se program zaène izvajati
	CLKPR = 0x80; CLKPR = 0x00;				//Izkop deljenja ure
	lcdInit();								//Inicilizacija LCDja
	DDRC |= (1<<BL);						//Nastavimo prvi pin na portu B na izhod
	{
	BL_on;								//Vklop backlight na LCDju
	}
	welcome();								//Izpiše zaèetni program
	_delay_ms(4000);						//Zakasnitev
	brisi();								//Pobrišemo lcd

	while(1)
	{

	int i=0;

	i = (i+1)%10;
	lcd_gotoxy(0,0);
	LCDputs("Notri:");
	lcd_gotoxy(8,0);
	char x[20] = {0};
	therm_read_temperature(x);

	LCDputs(x);

	stopinj1();

	lcd_gotoxy(0,1);
	LCDputs("Zunaj:");
	lcd_gotoxy(8,1);
	char Y[20] = {0};
	therm2_read_temperature(Y);
	LCDputs(Y);
	stopinj2();
	LCDcursorOFF();
	
	for (int i=0;i<5;i++)
		_delay_us(2000);
			if (i%2)
	{ 
        // in se vrti v zanki
           if(tipka==1 && A==0)
			{
  			BL_on;
  			A=1;
  			_delay_ms(30);
 		    while(tipka==1)
   			_delay_ms(1);
			}
           if(tipka==1 && A==1)
			{
  			BL_off;
  			A=0;
  			_delay_ms(30);
 		    while(tipka==1)
   			_delay_ms(1);
			}

   	}
	}	
}
