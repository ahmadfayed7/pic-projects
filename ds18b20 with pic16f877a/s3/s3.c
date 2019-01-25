// LCD module connections
sbit LCD_RS at RB2_bit;
sbit LCD_EN at RB3_bit;
sbit LCD_D4 at RB4_bit;
sbit LCD_D5 at RB5_bit;
sbit LCD_D6 at RB6_bit;
sbit LCD_D7 at RB7_bit;

sbit LCD_RS_Direction at TRISB2_bit;
sbit LCD_EN_Direction at TRISB3_bit;
sbit LCD_D4_Direction at TRISB4_bit;
sbit LCD_D5_Direction at TRISB5_bit;
sbit LCD_D6_Direction at TRISB6_bit;
sbit LCD_D7_Direction at TRISB7_bit;
// End LCD module connections
#include "delay.c" __CONFIG(0x3f72);
#define DQ         portc.f0
#define DQ_DIR     TRISC.f0
#define FOSC       10000
#define BAUD_RATE  9.6      // 9600 Baudrate
#define BAUD_VAL   (char)(FOSC/ (16 * BAUD_RATE )) - 1;
// Calculation For 9600 Baudrate @10Mhz
unsigned char i=0;
float Temerature=0;
void ds1820_init();
void Reset(void);
void write(unsigned char);
unsigned char Read(void);
void main()
{   unsigned int t;
   unsigned char Temp[9];
   Delay_Ms(100);
   Serial_init();
   ds1820_init();
   Delay_Ms(100);
     Lcd_Init();                                    // Initialize LCD
  Lcd_Cmd(_LCD_CLEAR);                           // Clear LCD
  Lcd_Cmd(_LCD_CURSOR_OFF);                      // Turn cursor off
   while(1)
   {
      Reset();
      write(0xcc);
      write(0x44);
      DQ_DIR    = 1;
      Delay_Us(10);
      while(!DQ);
  // this will be raised after finishing conversion
      Reset();
      write(0xcc);
      write(0xbe);
      t=Read();

      for(i=0;i<9;i++)
      Temp[i] = Read();      // Read 9 bytes
      for(i=0;i<9;i++)


      DelayMs(250);
      DelayMs(250);
      DelayMs(250);
      DelayMs(250);
   }
}

void ds1820_init()
{
      DQ    = 1;
      DQ_DIR = 1;         // pull up
}

void Reset(void)
{
      DQ_DIR    = 0;
      DQ     = 0;
      DelayMs(1);
      DelayUs(250);      // 500us
      DQ_DIR    = 1;
      DelayUs(100);      // 40us
      while(DQ==1);      // wait until presence pulse
      DelayMs(1);
      DelayUs(250);      // 500us
}

void write(unsigned char cmd)
{
      for(i=0;i<8;i++){
      DQ_DIR   = 0;                // pull down
      DQ     = 0;
      DelayUs(25);                 // 10us
      DQ       = (cmd & 0x01)?1:0; // Send bit
      cmd      = cmd >> 1;
      DelayUs(120);                // >45us
      DQ_DIR   = 1;                // release
      }
}

unsigned char Read(void)
{
      unsigned char temp=0,RecDat=0;
      for(i=0;i<8;i++)
      {
         DQ_DIR   = 0;
         DQ     = 0;        // pull down
    DelayUs(25);       // 10us
    DQ_DIR   = 1;      // release
        DelayUs(25);       // 10us
        temp   = DQ;       // read bit
        temp   = temp<<i;
       RecDat |= temp;
        DelayUs(70);       // 30us
      }
      return RecDat;
}