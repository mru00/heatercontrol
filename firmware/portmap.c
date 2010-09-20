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
 * Contact <mru@fourier> for comment & bug reports					    *
 ************************************************************************/
/* 
 * portmap, provides indirection to ports with port id's 
 *
 */

#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>

#include "common.h"

#define PORTMAP_NAME(o) case o: uart_puts_P( #o ); break

void portmap_init(void) {
  LOG_INIT();

  LOG_INIT_EXIT();
}


void portmap_print_output(uint8_t output) {
  switch(output) {
  case OUTPUT_L1:           uart_puts_P("Lampe1"); break;
  case OUTPUT_L2:           uart_puts_P("Lampe2"); break;
  case OUTPUT_FOGGER:       uart_puts_P("Befeuchter"); break;
  case OUTPUT_HEATING_WIRE: uart_puts_P("HeizDraht"); break;
  case OUTPUT_HEATING_LAMP: uart_puts_P("HeizLampe"); break;
#ifdef PORTMAP_DEBUG
  default: 
	uart_puts_P("wrong output number" NEWLINE);
#endif
  }
}
