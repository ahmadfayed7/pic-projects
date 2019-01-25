 // LCD module connections
sbit LCD_RS at RB1_bit;
sbit LCD_EN at RB0_bit;
sbit LCD_D4 at RB2_bit;
sbit LCD_D5 at RB3_bit;
sbit LCD_D6 at RB4_bit;
sbit LCD_D7 at RB5_bit;

sbit LCD_RS_Direction at TRISB1_bit;
sbit LCD_EN_Direction at TRISB0_bit;
sbit LCD_D4_Direction at TRISB2_bit;
sbit LCD_D5_Direction at TRISB3_bit;
sbit LCD_D6_Direction at TRISB4_bit;
sbit LCD_D7_Direction at TRISB5_bit;
// End LCD module connections
unsigned int cnt=0;                 // Define variable cnt
void interrupt() {
   if(INTCON.TMR0IF)
   {
   if(cnt>=1)
   {
 //  PORTB.f1=~PORTB.f1;
   cnt=0;

   TMR0 = 253;                // Timer TMR0 is returned its initial value

   return ;
   }
   INTCON.TMR0IF=0;               //  bit T0IF is cleared
   cnt++;                      // Interrupt causes cnt to be incremented by 1
   return ;
   }
}
 void main()

{
     char res[4] ;
    //const char Voltage[] = "Digital OhmMeter";
    unsigned char Unit = 0;
    const float R3 = 10000, Vcc = 5.0;               /*  vo = (10k/(90k + 10k)) vcc    */
    unsigned int ADC_Value = 0;
    float VoltageOutput = 0.0;
    float V ;
   unsigned long ResistanceMeasured = 0;           /*  32 bit wide data type          */
   //unsigned char Character[] = "0123456789";
   unsigned long Long32Bit ;
   int x =20 ;
  char val[2];
  CMCON = 0x07;
  TRISA = 0xFF;
  option_reg =0x17 ;
  ADCON1 = 0x00;
  Lcd_Init();
  Lcd_Cmd(_LCD_CLEAR);
  Lcd_Cmd(_LCD_CURSOR_OFF);

  do
  {
     ADC_Value = ADC_Read(0);
     Lcd_Cmd(_LCD_CLEAR);
      Lcd_Out(1,1,"RESISTANCE  ");
    Lcd_Out(2,1,"r=");
     //Lcd_Out(2,10,"OHMS");
    VoltageOutput = ((float)(ADC_Value))* 0.0048828;
    ResistanceMeasured = (unsigned long)((VoltageOutput * R3)/(Vcc - VoltageOutput));

    Long32Bit = ResistanceMeasured ;
          res[0]=(Long32Bit/10000)+'0';
          res[1]=(Long32Bit%10000/1000)+'0';
          res[2]=(Long32Bit%10000%1000/100)+'0';
          res[3]=  (Long32Bit%10000%1000%100/10)+'0';
          res[4]= (Long32Bit%10000%1000%100%10)+'0'  ;
          ByteToStr(x, val);
    //Lcd_Out(2,5,res);
      Lcd_Out(2,5,val);
    lcd_out(2,10,"   ") ;
     Lcd_Out(2,13,"OHMS");
    Delay_ms(250);
  } while(1);
}