#ifndef _LCD_H_
#define _LCD_H_

#include <inttypes.h>

#define LCD_DATA_PORT	 	PORTB
#define LCD_DATA_PORT_DDR	DDRB

#define LCD_CMD_PORT	 	PORTB
#define LCD_CMD_PORT_DDR 	DDRB
#define LCD_RS_PIN		 4
#define LCD_E_PIN		 5


#define DATA 0
#define CMD 1

void lcdProgressBar(short int progress, short int maxprogress, char length);
void lcd_write(unsigned char val, unsigned char mode);
void LCDWriteChar(unsigned char val);
void LCDputs(const char *s);
void lcd_gotoxy(uint8_t x, uint8_t y);
void lcdInit(void);
void LCDcursorOn(void);
void LCDcursorOnBlink(void);
void LCDcursorOFF(void);	
void LCDblank(void);	
void LCDvisible(void);
void LCDcls(void);



#endif

