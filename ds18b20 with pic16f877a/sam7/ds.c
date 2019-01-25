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

//  Set TEMP_RESOLUTION to the resolution you want:
//   (can be 9,10,11,or 12)
// ( DS 1820 and DS 18S20 automatically set to default )

const unsigned short TEMP_RESOLUTION = 10;                                      // Choose Resolution for DS 18B20 here

unsigned short Chip;
unsigned short CONF_WORD;

unsigned temp;
unsigned wait;

char *text = "000.00";



void Display_Temperature(unsigned int temp2write) {

  char temp_whole;
  unsigned int temp_fraction;
  bit neg;
  neg = 0;

   if ( Chip == 0x10 ) temp2write = temp2write * 8;

  // check if temperature is negative

  if (temp2write & 0x8000)
     {
      text[0] = '-';
      temp2write = ~temp2write + 1;
      neg = 1;
     }

  // extract temp_whole
  temp_whole = ( temp2write & 0x7FF ) >> 4 ;

  // convert temp_whole to characters
  if (temp_whole/100)
     text[0] = temp_whole/100  + 48;
  else
     if (!neg) text[0] = ' ';                                                   // overwrite text[0] only if positive temp.


  if (temp_whole/10)
     text[1] = (temp_whole/10)%10 + 48;                                         // Extract tens digit
  else {

       if (temp_whole/100)
          text[1] = '0';
       else
           text[1] = ' ';
       }


  text[2] =  temp_whole%10     + 48;                                            // Extract ones digit

  // extract temp_fraction and convert it to unsigned int

   temp_fraction = temp2write & 0x000F;
   temp_fraction = temp_fraction * 25/4;

  // convert temp_fraction to characters

  text[4] = (temp_fraction/10)%10  + 48;                                        // Extract tens digit
  text[5] =  temp_fraction%10      + 48;                                        // Extract ones digit

  // print temperature on LCD
  Lcd_Out(2, 4, text);
}

void main() {
  ADCON1 = 7;                                                                   // Configure AN pins as digital I/O
//  Lcd_Config(&PORTB, 4, 5, 6, 3, 2, 1, 0);                                    // Lcd_Init_EP5, see Autocomplete
  PORTC = 0;
  TRISC = 0;
  PORTE = 0;
  TRISE = 0b00000100;


  Lcd_Init();                                                                   // Initialize LCD
  Lcd_Cmd(_LCD_CLEAR);                                                          // Clear LCD
  Lcd_Cmd(_LCD_CURSOR_OFF);                                                     // Turn cursor off
  Lcd_Out(1, 1, "  Temperature:  ");
  // Print degree character, 'C' for Centigrades
  Lcd_Chr(2,11,178);                                                            // different LCD displays have different char code for degree
                                                                                // if you see greek alpha letter try typing 178 instead of 223

  Lcd_Chr(2,12,'C');

  // Identify sensor

  Ow_Reset(&PORTE,2);
  Ow_Write(&PORTE,2,0x33);                                                      // Read ROM Command
  Chip = Ow_Read(&PORTE,2);                                                     // Read first BYTE

 switch (Chip)
  {
   case 0x28: CONF_WORD = ((TEMP_RESOLUTION - 9) << 5) | 0b00011111;break;

   case 0x10: goto NOCONF;
  }
  // Configure DS Resolution

  Ow_Reset(&PORTE,2);
  Ow_Write(&PORTE,2,0xCC);
  Ow_Write(&PORTE,2,0x4E);
  Ow_Write(&PORTE,2,0);
  Ow_Write(&PORTE,2,0);
  Ow_Write(&PORTE,2,CONF_WORD);

  NOCONF:

  //--- main loop

  do {
    //--- perform temperature reading

    Ow_Reset(&PORTE, 2);                                                        // Onewire reset signal
    Ow_Write(&PORTE, 2, 0xCC);                                                  // Issue command SKIP_ROM
    Ow_Write(&PORTE, 2, 0x44);                                                  // Issue command CONVERT_T


 //--- Waiting for measure complete loop

    PORTC.F3 = 1;
  switch (Chip)
  {
   case 0x28 : do
     {
      TRISE.F2 = 0;
      PORTE.F2 = 0;
      //Delay_us(1);                                                            // Delay max = 2µs ...
      TRISE.F2 = 1;
      PORTE.F2 = 1;
      Delay_us (1);                                                             // Why is it necessary ???
     }
      while ( !PORTE.F2 ); break;



 case 0x10 : do
     {
      TRISE.F2 = 0;
      PORTE.F2 = 0;
      Delay_us(5);                                                              // Delay min = 5µs ...
      TRISE.F2 = 1;
      PORTE.F2 = 1;
      // Delay_us (1);                                                          // No Delay !!!
      }
      while ( !PORTE.F2 );
  }

    PORTC.F3 = 0;

 //--- data ready

    Ow_Reset(&PORTE, 2);
    Ow_Write(&PORTE, 2, 0xCC);                                                  // Issue command SKIP_ROM
    Ow_Write(&PORTE, 2, 0xBE);                                                  // Issue command READ_SCRATCHPAD



    temp =  Ow_Read(&PORTE, 2);
    temp = (Ow_Read(&PORTE, 2) << 8) + temp;

    //--- Format and display result on Lcd
    Display_Temperature(temp);

 switch (TEMP_RESOLUTION)
        {
        case 12 : wait = 250;break;
        case 11 : wait = 625;break;
        case 10 : wait = 813;break;
        case  9 : wait = 907;break;
        }
  if (chip == 0x10) wait = 200;

    VDelay_ms(wait);
  } while (1);
}