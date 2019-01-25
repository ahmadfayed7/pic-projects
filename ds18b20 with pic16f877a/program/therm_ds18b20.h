#include <avr/io.h> 
#include <stdio.h>

#define F_CPU 16000000UL 		//Your clock speed in Hz (3Mhz here)

#define LOOP_CYCLES 8 				//Number of cycles that the loop takes
#define us(num) (num/(LOOP_CYCLES*(1/(F_CPU/1000000.0))))

#define THERM_PORT PORTD
#define THERM_DDR DDRD
#define THERM_PIN PIND
#define THERM_DQ PD2
/* Utils */
#define THERM_INPUT_MODE() THERM_DDR&=~(1<<THERM_DQ)
#define THERM_OUTPUT_MODE() THERM_DDR|=(1<<THERM_DQ)
#define THERM_LOW() THERM_PORT&=~(1<<THERM_DQ)
#define THERM_HIGH() THERM_PORT|=(1<<THERM_DQ)
/* list of these commands translated into C defines:*/
#define THERM_CMD_CONVERTTEMP 0x44
#define THERM_CMD_RSCRATCHPAD 0xbe
#define THERM_CMD_WSCRATCHPAD 0x4e
#define THERM_CMD_CPYSCRATCHPAD 0x48
#define THERM_CMD_RECEEPROM 0xb8
#define THERM_CMD_RPWRSUPPLY 0xb4
#define THERM_CMD_SEARCHROM 0xf0
#define THERM_CMD_READROM 0x33
#define THERM_CMD_MATCHROM 0x55
#define THERM_CMD_SKIPROM 0xcc
#define THERM_CMD_ALARMSEARCH 0xec
/* constants */
#define THERM_DECIMAL_STEPS_12BIT 5 //.0625


void therm_delay(uint16_t delay);
uint8_t therm_reset();
void therm_write_bit(uint8_t bit);
uint8_t therm_read_bit(void);
uint8_t therm_read_byte(void);
void therm_write_byte(uint8_t  byte);
//
void therm_read_temperature(char *buffer);
void therm_read_temperature2(int8_t *digi, uint16_t *deci);

////////////////////////////////////////////SENZOR 2////////////////////////////////////////////////

#define THERM2_PORT PORTD
#define THERM2_DDR DDRD
#define THERM2_PIN PIND
#define THERM2_DQ PD1
/* Utils */
#define THERM2_INPUT_MODE() THERM2_DDR&=~(1<<THERM2_DQ)
#define THERM2_OUTPUT_MODE() THERM2_DDR|=(1<<THERM2_DQ)
#define THERM2_LOW() THERM2_PORT&=~(1<<THERM2_DQ)
#define THERM2_HIGH() THERM2_PORT|=(1<<THERM2_DQ)
/* list of these commands translated into C defines:*/
#define THERM2_CMD_CONVERTTEMP 0x44
#define THERM2_CMD_RSCRATCHPAD 0xbe
#define THERM2_CMD_WSCRATCHPAD 0x4e
#define THERM2_CMD_CPYSCRATCHPAD 0x48
#define THERM2_CMD_RECEEPROM 0xb8
#define THERM2_CMD_RPWRSUPPLY 0xb4
#define THERM2_CMD_SEARCHROM 0xf0
#define THERM2_CMD_READROM 0x33
#define THERM2_CMD_MATCHROM 0x55
#define THERM2_CMD_SKIPROM 0xcc
#define THERM2_CMD_ALARMSEARCH 0xec
/* constants */
#define THERM2_DECIMAL_STEPS_12BIT 5 //.0625


void therm2_delay(uint16_t delay);
uint8_t therm2_reset();
void therm2_write_bit(uint8_t bit);
uint8_t therm2_read_bit(void);
uint8_t therm2_read_byte(void);
void therm2_write_byte(uint8_t  byte);
//
void therm2_read_temperature(char *buffer);
void therm2_read_temperature2(int8_t *digi, uint16_t *deci);
