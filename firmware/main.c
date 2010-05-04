/************************************************************************
 * This file is part of TerraControl.								    *
 * 																	    *
 * TerraControl is free software; you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published    *
 * by the Free Software Foundation; either version 2 of the License, or *
 * (at your option) any later version.								    *
 * 																	    *
 * TerraControl is distributed in the hope that it will be useful,	    *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	    *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	    *
 * GNU General Public License for more details.						    *
 * 																	    *
 * You should have received a copy of the GNU General Public License    *
 * along with TerraControl; if not, write to the Free Software		    *
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 *
 * USA																    *
 * Written and (c) by mru											    *
 * Contact <mru@sisyphus.teil.cc> for comment & bug reports				*
 ************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>

#include <stdlib.h>

#include "common.h"

#define UART_BAUD_RATE     38400


static void __attribute__((constructor)) 
uart_constructor(void) {
  uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,XTAL));
}

#define PHASE_A 0
#define PHASE_B 1

#define PORT_BIP_PHASE_A &PORTB, PB0
#define PORT_BIP_PHASE_B &PORTB, PB1

#define PORT_UNI_PHASE_A1 &PORTC, PC0
#define PORT_UNI_PHASE_A2 &PORTC, PC1
#define PORT_UNI_PHASE_B1 &PORTC, PC2
#define PORT_UNI_PHASE_B2 &PORTC, PC3



static void set_phase(uint8_t phase, uint8_t value) {
  if ( phase == PHASE_A ) xpin2 ( value,  PORT_BIP_PHASE_A );
  else 	xpin2 ( value,  PORT_BIP_PHASE_B );
}


static void bip_sleep_phase(void) {
  _delay_ms(4);
}

static void uni_sleep_phase(void) {
  _delay_ms(20);
}

static void bip_step_forward(void) {
  set_phase(PHASE_A, 0);  bip_sleep_phase();
  set_phase(PHASE_B, 0);  bip_sleep_phase();
  set_phase(PHASE_A, 1);  bip_sleep_phase();
  set_phase(PHASE_B, 1);  bip_sleep_phase();
}

static void bip_step_backward(void) {
  set_phase(PHASE_B, 0);  bip_sleep_phase();
  set_phase(PHASE_A, 0);  bip_sleep_phase();
  set_phase(PHASE_B, 1);  bip_sleep_phase();
  set_phase(PHASE_A, 1);  bip_sleep_phase();
}


static void uni_step_forward(void) {

  clearpin2(PORT_UNI_PHASE_A2);
  clearpin2(PORT_UNI_PHASE_B1);
  setpin2(PORT_UNI_PHASE_A1);
  setpin2(PORT_UNI_PHASE_B2);
  uni_sleep_phase();

  clearpin2(PORT_UNI_PHASE_A2);
  clearpin2(PORT_UNI_PHASE_B2);
  setpin2(PORT_UNI_PHASE_A1);
  setpin2(PORT_UNI_PHASE_B1);
  uni_sleep_phase();

  clearpin2(PORT_UNI_PHASE_A1);
  clearpin2(PORT_UNI_PHASE_B2);
  setpin2(PORT_UNI_PHASE_A2);
  setpin2(PORT_UNI_PHASE_B1);
  uni_sleep_phase();

  clearpin2(PORT_UNI_PHASE_A1);
  clearpin2(PORT_UNI_PHASE_B1);
  setpin2(PORT_UNI_PHASE_A2);
  setpin2(PORT_UNI_PHASE_B2);
  uni_sleep_phase();

  
}

static void uni_step_backward(void) {

  clearpin2(PORT_UNI_PHASE_A1);
  clearpin2(PORT_UNI_PHASE_B1);
  setpin2(PORT_UNI_PHASE_A2);
  setpin2(PORT_UNI_PHASE_B2);
  uni_sleep_phase();


  clearpin2(PORT_UNI_PHASE_A1);
  clearpin2(PORT_UNI_PHASE_B2);
  setpin2(PORT_UNI_PHASE_A2);
  setpin2(PORT_UNI_PHASE_B1);
  uni_sleep_phase();



  clearpin2(PORT_UNI_PHASE_A2);
  clearpin2(PORT_UNI_PHASE_B2);
  setpin2(PORT_UNI_PHASE_A1);
  setpin2(PORT_UNI_PHASE_B1);
  uni_sleep_phase();

  clearpin2(PORT_UNI_PHASE_A2);
  clearpin2(PORT_UNI_PHASE_B1);
  setpin2(PORT_UNI_PHASE_A1);
  setpin2(PORT_UNI_PHASE_B2);
  uni_sleep_phase();

}

#define uchar unsigned char
#define uint unsigned int

#define	xRC5_IN		PIND
#define	xRC5		PD7			// IR input low active


#define RC5TIME 	1.778e-3		// 1.778msec
#define PULSE_MIN	(uchar)(XTAL / 512 * RC5TIME * 0.4 + 0.5)
#define PULSE_1_2	(uchar)(XTAL / 512 * RC5TIME * 0.8 + 0.5)
#define PULSE_MAX	(uchar)(XTAL / 512 * RC5TIME * 1.2 + 0.5)


volatile uchar	rc5_bit;				// bit value
volatile uchar	rc5_time;				// count bit time
volatile uint	rc5_tmp;				// shift bits in
volatile uint	rc5_data;				// store result


SIGNAL (SIG_OVERFLOW0)
{
  uint tmp = rc5_tmp;				// for faster access

  TCNT0 = -2;					// 2 * 256 = 512 cycle

  if( ++rc5_time > PULSE_MAX ){			// count pulse time
    if( !(tmp & 0x4000) && tmp & 0x2000 )	// only if 14 bits received
      rc5_data = tmp;
    tmp = 0;
  }

  if( (rc5_bit ^ xRC5_IN) & 1<<xRC5 ){		// change detect
    rc5_bit = ~rc5_bit;				// 0x00 -> 0xFF -> 0x00

    if( rc5_time < PULSE_MIN )			// to short
      tmp = 0;

    if( !tmp || rc5_time > PULSE_1_2 ){		// start or long pulse time
      if( !(tmp & 0x4000) )			// not to many bits
        tmp <<= 1;				// shift
      if( !(rc5_bit & 1<<xRC5) )		// inverted bit
        tmp |= 1;				// insert new bit
      rc5_time = 0;				// count next pulse time
    }
  }

  rc5_tmp = tmp;
}

static volatile uint8_t counter = 0;

ISR(INT0_vect) {
  counter ++;
}


int main(void)
{

  uint i;
  char s[30];
  _delay_ms(100);

  // init section -------------------------------------

  TCCR0 = 1<<CS02;			//divide by 256
  TIMSK = 1<<TOIE0;			//enable timer interrupt


  DDRC |= 0x0f;
  DDRD &= ~_BV(PD7);

  // internal pullup seems NOT to work 
  //  PORTD |= _BV(PD2);

  //  PORTD &= ~_BV(PD2);

  // The rising edge of INT0 generates an interrupt request.  
  // enable external interrupt 0

  //  MCUCR |= ~_BV(ISC01) | _BV(ISC00);
  //  GICR  |= _BV(INT0);                   

  DDRB = 0x03;
  PORTC &= 0xf0;
  uni_step_backward();  uni_step_backward();

  uni_step_forward();

  sei();
  /* main loop section ---------------------------------- */

  for(;;) {
#if 0
	if ( i < 100 ) uni_step_forward();
	else if (i < 200) uni_step_backward();
	else i = 0;
#elif 0
	if (counter > 0) {
	  _delay_ms(100);
	  for (i = 0; i< counter; i++ ) {
		//34
		step_forward();
		_delay_ms(500);
	  }

	  counter = 0;
	}
#else 
    cli();
    i = rc5_data;			// read two bytes from interrupt !
    rc5_data = 0;
    sei();
    if( i ){
	  uint device = i >> 6 & 0x1F;
	  uint code = (i & 0x3F) | (~i >> 7 & 0x40);
      itoa( i >> 6 & 0x1F, s, 10);	// Device address
      itoa((i & 0x3F) | (~i >> 7 & 0x40), s, 10); // Key Code
	  

	  // VOLUP
	  if ( code == 16 ) uni_step_forward();
	  else if ( code == 17 ) uni_step_backward();
	  
	  
	  clearpin2(PORT_UNI_PHASE_A1);
	  clearpin2(PORT_UNI_PHASE_A2);
	  clearpin2(PORT_UNI_PHASE_B1);
	  clearpin2(PORT_UNI_PHASE_B2);

    }

	
#endif
  }

}


/*  LocalWords:  eeprom
 */
