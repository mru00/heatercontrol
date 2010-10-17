#include <stdio.h>
#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "common.h"

#define F_INTERRUPTS 1e3

// codes from:
// http://en.wikipedia.org/wiki/Seven-segment_display


#define LA _BV(0)
#define LB _BV(1)
#define LC _BV(2)
#define LD _BV(3)
#define LE _BV(4)
#define LF _BV(5)
#define LG _BV(6)

// in abcdefg coding
static uint8_t seven_seg_codes[] = {
  /*0*/  LA|LB|LC|LD|LE|LF,
  /*1*/  LB|LC,
  /*2*/  LA|LB|LD|LE|LG,
  /*3*/  LA|LB|LC|LD|LG,
  /*4*/  LB|LC|LF|LG,
  /*5*/  LA|LC|LD|LF|LG,
  /*6*/  LA|LC|LD|LE|LF|LG,
  /*7*/  LA|LB|LC,
  /*8*/  LA|LB|LC|LD|LE|LF|LG,
  /*9*/  LA|LB|LC|LD|LF|LG
};



typedef struct  {
  uint8_t value[3];
  uint8_t dp    : 3;
} display_data_t;

static volatile uint8_t        current_position;
static volatile display_data_t display_data; 

// ln5623-11 pin-assignment:
// a  11
// b  7
// c  4
// d  2
// e  1
// f  10
// g  5
// dp 3

// atmel pin assignmen:
/*
 * a-g,dp: portb/0-7
 * digit1: portd/pd2
 * digit2: portd/pd3
 * digit3: portd/pd4
 */

extern void ln5623_init(void) 
{
  LOG_INIT();

  OCR1A =  (F_CPU/F_INTERRUPTS) - 1;   // compare value: 1/10000 of CPU frequency
  TCCR1B= _BV(WGM12) | _BV(CS10);      // switch CTC Mode on, set prescaler to 1
  TIMSK = 1 << OCIE1A;                 // OCIE1A: Interrupt by timer
									   // compare
  DDRB = 0xff;
  PORTB = 0;
  DDRD |= _BV(PD2) | _BV(PD3) | _BV(PD4);
  PORTD |= _BV(PD2) | _BV(PD3) | _BV(PD4);

  LOG_INIT_EXIT();
}

ISR(TIMER1_COMPA_vect) {
  PORTB = 0xff;

  PORTD &= ~ (_BV(PD2) | _BV(PD3) | _BV(PD4));
  PORTD |= _BV(PD2 + current_position);

  PORTB = ~((seven_seg_codes[display_data.value[2-current_position]]) | 
			(((display_data.dp & _BV(current_position)) != 0) << 7));
  
  current_position++;
  if (current_position == 3) {
	current_position = 0;
  }
}

extern uint8_t ln5623_set_output(uint16_t value,
							  uint8_t dp)
{
  if ( value > 999 ) {
	value = 999;
	dp = 7;
  }

  display_data.value[2] = value/100;
  display_data.value[1] = (value-100*display_data.value[2])/10;
  display_data.value[0] = (value-10*display_data.value[1]-100*display_data.value[2]);
  display_data.dp = dp;

  return 0;
}

extern void ln5623_clear_output(void) 
{
  display_data.value[2] = 0;
  display_data.value[1] = 0;
  display_data.value[0] = 0;
  display_data.dp = 0;
}

