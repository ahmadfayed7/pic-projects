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
const unsigned short TEMP_RESOLUTION = 12;

const int RES_FACTOR_1[4] = {5000, 2500, 1250, 625};
const unsigned int RES_FACTOR_2[4] = {0x0001, 0x0003, 0x0007, 0x000F};
const unsigned int RES_FACTOR_3[4] = {0x8000, 0xC000, 0xE000, 0xF000};
float alarma;
unsigned temp,temp2,new_temp;
unsigned short  j, RES_SHIFT,j2;

void Display_Temperature(unsigned int temp) {
  const unsigned short RES_SHIFT = TEMP_RESOLUTION - 8;
  unsigned int temp_whole, temp_fraction;
  unsigned short i;
  char text[8];

  // Isolate the fraction and make it a 4-digit decimal integer (for display)
  temp_fraction = temp & RES_FACTOR_2[RES_SHIFT - 1];
  temp_fraction = temp_fraction * RES_FACTOR_1[RES_SHIFT - 1];
  //portc = temp_fraction;
  // Handle the whole part of temperature value
  temp_whole = temp;

  // Is temperature negative?
  if ((temp_whole & 0x8000) != 0u) i = 1;  // Yes, i = 1
  else i = 0;                              // No,  i = 0
//  PORTC = i;
  // Remove the fractional part
  temp_whole >>= RES_SHIFT;

  // Correct the sign if necessary
  if (i)temp_whole |= RES_FACTOR_3[RES_SHIFT - 1];
  //if(i==0)temp_whole &=~ RES_FACTOR_3[RES_SHIFT - 1];
  //portd = temp_whole;
  IntToStr(temp_whole, text);  // Convert whole part to string
  Lcd_Out(2, 6, text);         // Print whole part on LCD
   //Lcd_Out(2, 10, ' ');
  Lcd_Chr_Cp('.');             // Print dot to separate fractional part


  IntToStr(temp_fraction, text); // Convert fractional part to string

  // Add leading zeroes (we display 4 digits fractional part)
  if (temp_fraction < 1000u) Lcd_Chr_Cp('0');
  if (temp_fraction < 100u)  Lcd_Chr_Cp('0');
  if (temp_fraction < 10u)   Lcd_Chr_Cp('0');

  Lcd_Out_Cp(text);            // Print fractional part on LCD

  Lcd_Chr_Cp(223);             // Print degree character
  Lcd_Chr_Cp('C');             // Print 'C' for Centigrades
}//~

void main() {
  ADCON1 = 0xFF;               // Configure RA5 pin as digital I/O
  PORTE  = 0xFF;
  TRISE  = 0x0F;               // PORTE is input
  PORTB  = 0;
  TRISB  = 0;               // PORTB is output
  TRISD=0;
  PORTD=0;
  TRISC=0;
  PORTC=0;
  adcon1=0b00000110  ;
  // Initialize LCD on PORTB and prepare for output
  Lcd_Init();
  //Lcd_Cmd(lcd_cursor_off);
    Lcd_Cmd(_LCD_CURSOR_OFF);
  Lcd_Out(1, 1, "temp:   ");

  do { // main loop

    Ow_Reset(&PORTE,2);        // Onewire reset signal
    Ow_Write(&PORTE,2,0xCC);   // Issue command SKIP_ROM
    Ow_Write(&PORTE,2,0x44);   // Issue command CONVERT_T
    Delay_ms(120);

    Ow_Reset(&PORTE,2);
    Ow_Write(&PORTE,2,0xCC);   // Issue command SKIP_ROM
    Ow_Write(&PORTE,2,0xBE);   // Issue command READ_SCRATCHPAD
    Delay_ms(400);

    j = Ow_Read(&PORTE,2);     // Get temperature LSB
    j2=j;
    temp = Ow_Read(&PORTE,2);  // Get temperature MSB
    temp2=temp;
    temp <<= 8; temp += j;     // Form the result
    temp2<<=5;
    j2>>=3;
    new_temp=temp2^j2;
    portd=new_temp;
  //   alarma=39;
    alarma=((new_temp*127.5)/255);
    if(((alarma>=36.5)&&(alarma<=38)))  {    //YESÝL LED AKTIF
  PORTC.F0=1;
   }

    else  {
    portc.f0=0;}

    Display_Temperature(temp); // Format and display result on LCD
    Delay_ms(500);

  } while (1);

}//~!