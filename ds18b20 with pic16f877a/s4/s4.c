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


/*******************************************************************************
* Defination of label                                                                  *
*******************************************************************************/

#define Skip_ROM                 0xCC
#define Convert_T                 0x44
#define Read_scratchpad 0xBE

#define Port_18B20         portd.f0
#define Tx_18B20         TRISd.f0 = 0
#define Rx_18B20         TRISd.f0 = 1


/*******************************************************************************
* PRIVATE FUNCTION PROTOTYPES                                                  *
*******************************************************************************/
void delay_ms(unsigned int ui_value);
unsigned char reset();
void write(char WRT);
unsigned char read();
void lcd_bcd(unsigned char uc_digit, unsigned int ui_number);
/*******************************************************************************
* Global Variables                                                             *
*******************************************************************************/

/*******************************************************************************
* MAIN FUNCTION                                                                *
*******************************************************************************/
int main(void)
{
        unsigned temp;
        unsigned short tempL, tempH, fraction;
        unsigned int i = 0;        // declare a variable to store
        char txt[4];
        char new;
        // ensure all the hardware port in zero initially
        PORTA = 0;
        PORTB = 0;
        PORTC = 0;
        PORTD = 0;
        PORTE = 0;

        // Initialize the I/O port direction, this must be configured according to circuit
        // TRISX control pin direction, output pin must be configure as '0'
        // while input must be configure as '1'
        TRISA = 0;
        TRISB = 0b00001110;
        TRISC = 0;
        TRISD = 0x01;


        //Initialize LCD to be use
        lcd_init();



        delay_ms(1000);
        LCD_CMD(_LCD_CLEAR);
        LCD_CMD(_LCD_CURSOR_OFF);
         LCD_out(1,1,"TEMPERATURE:");
        while(1)         // create an infinite loop
        {

                        if(!reset())
                        {
                        write(Skip_ROM);
                        write(Convert_T);
                        delay_ms(750);

                        reset();
                        write(Skip_ROM);
                        write(Read_scratchpad);

                        tempL = read();                        //read low temp value
                        tempH = read();                        //read high temp value
                        i=((unsigned int)tempH << 8 ) + (unsigned int)tempL;                //put both value in one variable
                        i = (float)i * 6.25 ;                //calcuation from the table provided
                        //i=i/100;
                        FloatToStr(i,txt)   ;
                       //new=txt[2];
                         //txt[2]='.';
                         //txt[3]=new;
                       //lcd_bcd(5,i);
                       lcd_out(2,1,txt);                                //display the temperation in LCD
                       lcd_out(2,6,"                  ");
                        }
        }
        while(1) continue;        // infinite loop to prevent PIC from reset if there is no more program
}

/*******************************************************************************
* PRIVATE FUNCTION: delay_ms
*
* PARAMETERS:
* ~ ui_value        - The period for the delay in miliseconds.
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Delay in miliseconds.
*
*******************************************************************************/
void delay_ms(unsigned int ui_value)
{
        while (ui_value-- > 0) {
                delay_ms(1);                // macro from HI-TECH compiler which will generate 1ms delay base on value of _XTAL_FREQ in system.h
        }
}

/*******************************************************************************
* PRIVATE FUNCTION: beep
*
* PARAMETERS:
* ~ uc_count        - How many times we want to beep.
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Beep for the specified number of times.
*
*******************************************************************************/


unsigned char reset()
{
        Tx_18B20; // Tris = 0 (output)
        Port_18B20 = 0; // set pin# to low (0)
         delay_us(480); // 1 wire require time delay
        Rx_18B20; // Tris = 1 (input)
        delay_us(60); // 1 wire require time delay

                if (Port_18B20 == 0) // if there is a presence pluse
                {
                delay_us(480);
                return 0; // return 0 ( 1-wire is presence)
                }
                else
                {
                delay_us(480);
                return 1; // return 1 ( 1-wire is NOT presence)
                }
}

void write(char WRT)
{
        char i,Cmd;
        Cmd=WRT;
        Rx_18B20; // set pin# to input (1)

                for(i = 0; i < 8; i++)
                {
                        if((Cmd & (1<<i))!= 0)
                        {
                        // write 1
                        Tx_18B20; // set pin# to output (0)
                        Port_18B20 = 0; // set pin# to low (0)
                        delay_us(1); // 1 wire require time delay
                        Rx_18B20; // set pin# to input (release the bus)
                        delay_us(60); // 1 wire require time delay
                        }
                        else
                        {
                        //write 0
                        Tx_18B20; // set pin# to output (0)
                        Port_18B20 = 0; // set pin# to low (0)
                        delay_us(60); // 1 wire require time delay
                        Rx_18B20; // set pin# to input (release the bus)
                        }
                }
}

unsigned char read()
{
        char i,result = 0;
        Rx_18B20; // TRIS is input(1)
                for(i = 0; i < 8; i++)
                {
                Tx_18B20; // TRIS is output(0)
                Port_18B20 = 0; // genarate low pluse for 2us
                delay_us(2);
                Rx_18B20; // TRIS is input(1) release the bus
                if(Port_18B20 != 0)
                result |= 1<<i;
                delay_us(60); // wait for recovery time
                }
        return result;
}

/*******************************************************************************
* PUBLIC FUNCTION: lcd_putchar
*
* PARAMETERS:
* ~ c_data	- The character to display.
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Display a character on the LCD.
*
*******************************************************************************/
void lcd_putchar(char c_data)
{
	// Send the data to display.
//	send_lcd_data(1, (unsigned char)c_data);
}

/*******************************************************************************
* PUBLIC FUNCTION: lcd_bcd
*
* PARAMETERS:
* ~ uc_digit, ui_number
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* convert the value in ui_number to BCD(Binary Code Decimal) and display the digit according to uc_digit needed.
*
*******************************************************************************/
void lcd_bcd(unsigned char uc_digit, unsigned int ui_number)
{
	unsigned int ui_decimal[5] ={ 0 };
	//extract 5 single digit from ui_number
		ui_decimal[4] = ui_number/10000;	// obtain the largest single digit, digit4
		ui_decimal[3] = ui_number%10000;	// obtain the remainder
		ui_decimal[2] = ui_decimal[3]%1000;
		ui_decimal[3] = ui_decimal[3]/1000;			// obtain the 2nd largest single digit, digit3
		ui_decimal[1] = ui_decimal[2]%100;
		ui_decimal[2] = ui_decimal[2]/100;			// obtain the 3rd largest single digit, digit2
		ui_decimal[0] = ui_decimal[1]%10;			// obtain the smallest single digit, digit0
		ui_decimal[1] = ui_decimal[1]/10;			// obtain the 4th largest single digit, digit1


			lcd_out(2,1,ui_decimal[4] + '0');
			lcd_out(2,2,ui_decimal[3] + '0');
			lcd_out(2,3,ui_decimal[2] + '0');
			lcd_out(2,4,'.');
			lcd_out(2,5,ui_decimal[1] + '0');
			lcd_out(2,6,ui_decimal[0] + '0');

}