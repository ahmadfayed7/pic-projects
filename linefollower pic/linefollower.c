
int dutyl;
int dutyr;
int ch0;
int ch1;
int ch2;
int x;
int y ;
int o;
int final_read ;
void init () ;
void output_direct();
void output_left();
void output_right();
int adc1_read () ;
int adc2_read () ;
int adc3_read () ;
void main() {
       init();
       for(;;)
       {
        x= adc1_read () ;
          delay_ms(10);
        y=adc2_read () ;
          delay_ms(10);
        o=adc3_read () ;
          delay_ms(10);
        final_read=(x*(2^0))+(y*(2^1))+(z*(2^2)) ;
       if(final_read==0)
       {output_direct();
       }
       if(final_read==1)
       {output_right();
       }
       if(final_read==2)
       output_direct();
       if(final_read==3)
       output_right();
       if(final_read==4)
       output_left();
       if(final_read==5)
       output_right();
       if(final_read==6)
       output_left();
       if(final_read==7)
       output_direct();

       }
}

void init ()
{
trisa=0xff;
porta=0;
trisc=0;
portc=0;
adcon1=0b10000010;
pwm1_init(5000);
pwm2_init(5000);
pwm1_start();
pwm2_start();
}

int input ()
{
return porta;
}
void output_direct()
{        dutyl=255 ;
        dutyr= 255;
         pwm1_set_duty(dutyr);
         pwm2_set_duty(dutyl);

}
void output_left()
{         dutyl=100 ;
        dutyr= 255;
         pwm1_set_duty(dutyr);
         pwm2_set_duty(dutyl);
}
void output_right()
{        dutyl=255 ;
        dutyr= 100 ;
         pwm1_set_duty(dutyr);
         pwm2_set_duty(dutyl);
}
int adc1_read ()
{ int vmv0;
adcon0=0b10000001;
adcon0.go=1 ;
  ch0=adc_read(0);
  vmv0=(5000*ch0)/1024 ;
  if(vmv0>2000)
  return 1 ;
  else
  return 0;
}
int adc2_read ()
{  int vmv1;
adcon0=0b10001001;
adcon0.go=1 ;
 ch1=adc_read(1);
   vmv1=(5000*ch1)/1024 ;
  if(vmv1>2000)
  return 1 ;
  else
  return 0;
}
int adc3_read ()
{ int vmv2;
adcon0=0b10010001;
adcon0.go=1 ;
 ch2=adc_read(2);
   vmv2=(5000*ch2)/1024 ;
  if(vmv2>2000)
  return 1 ;
  else
  return 0;
}