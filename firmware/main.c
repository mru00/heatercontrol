/************************************************************************
 * This file is part of HeaterControl.                                  *
 *                                                                      *
 * HeaterControl is free software; you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published    *
 * by the Free Software Foundation; either version 2 of the License, or *
 * (at your option) any later version.                                  *
 *                                                                      *
 * HeaterControl is distributed in the hope that it will be useful,     *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with HeaterControl; if not, write to the Free Software         *
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 *
 * USA                                                                  *
 * Written and (c) by mru                                               *
 * Contact <mru@sisyphus.teil.cc> for comment & bug reports             *
 ************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include "pid.h"

#include "common.h"

#define UART_BAUD_RATE     38400

static void __attribute__((constructor))
uart_constructor(void) {
  uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,XTAL));
}


#define K_P     1.00f
#define K_I     0.01f
#define K_D     0.00f

static struct PID_DATA pidData;

static const float fir_temp_coeffs[] = { 1./12., 1./12., 1./12.,
										 1./12., 1./12., 1./12.,
										 1./12., 1./12., 1./12.,
										 1./12., 1./12., 1./12.};

static float fir_push(const float xn,
					  float *buffer,
					  int *pos,
                     const float *coeff,
                     const int count)
{
  float yn = 0.0f;
  int i =0;

  *pos = (*pos + 1) % count;
  *(buffer + *pos) = xn;
  for (i = 0; i < count; i++) 
	yn += *(buffer+((i+*pos)%count)) * *(coeff + ((i+*pos)%count));
  return yn;
}



#define uchar unsigned char
#define uint unsigned int

static float last_temp_abgas[12];
static float last_temp_vorlauf[12];
static int temp_abgas_pos=0;
static int temp_vorlauf_pos=0;


static float calc_temp_abgas(uint16_t ambient_temp) {
  uint16_t adc_value;
  float volt;
  float temp;

  const float v_1190_deg = 69.553e-3;
  const float v_0_deg = 0.0;
  const float k = (1190.0-0.0)/(v_1190_deg-v_0_deg);

  const float amp_gain = 50.0;

  adc_select_channel(0);
  adc_single_conversion(&adc_value);
  adc_result2volt(adc_value, &volt, 2.54);

  temp = volt * k;
  temp /= amp_gain;
  temp += ambient_temp;

  return fir_push(temp, last_temp_abgas, &temp_abgas_pos, fir_temp_coeffs, 12);
}


static float calc_temp_vorlauf(uint16_t ambient_temp) {
  uint16_t adc_value;
  float volt;
  float temp;

  const float v_1190_deg = 69.553e-3;
  const float v_0_deg = 0.0;
  const float k = (1190.0-0.0)/(v_1190_deg-v_0_deg);

  const float amp_gain = 50.0;

  adc_select_channel(1);
  adc_single_conversion(&adc_value);
  adc_result2volt(adc_value, &volt, 2.56f);

  temp = volt * k;
  temp /= amp_gain;
  temp += ambient_temp;

  return fir_push(temp, last_temp_vorlauf, &temp_vorlauf_pos, fir_temp_coeffs, 12);
}

int main(void)
{
  char str_val[100];

  _delay_ms(100);

  // init section -------------------------------------

  DDRC = 0;
  PORTC = 0x00;

  DDRD &= ~_BV(PD7);
  DDRD |= _BV(PD6);
  PORTD |= _BV(PD7);

  DDRB = 0x07;

  uart_puts("hello from heater-control\n");
  adc_init();
  ln5623_init();
  pid_init(K_P * SCALING_FACTOR, 
		   K_I * SCALING_FACTOR, 
		   K_D * SCALING_FACTOR, 
		   &pidData);
  
  sei();
  /* main loop section ---------------------------------- */

  for(;;) {
	unsigned int recv = uart_getc();

	if ( (recv & UART_NO_DATA) == 0  ) {
	  receive(recv & 0xff);
	}

	const uint16_t ambient_temp = 25;
    uint16_t temp_vorlauf = (int)calc_temp_vorlauf(ambient_temp);
    uint16_t temp_abgas = (int)calc_temp_abgas(ambient_temp);

	int16_t inputValue = pid_controller(temp_vorlauf, 80, &pidData);

    set_temp_vorlauf(temp_vorlauf);
    set_temp_abgas(temp_abgas);


    ln5623_set_output(temp_vorlauf, 0);
    _delay_ms(50);
  }
}


/*  LocalWords:  eeprom
 */
