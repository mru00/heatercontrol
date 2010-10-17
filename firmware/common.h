/************************************************************************
 * This file is part of HeaterControl.								    *
 * 																	    *
 * HeaterControl is free software; you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published    *
 * by the Free Software Foundation; either version 2 of the License, or *
 * (at your option) any later version.								    *
 * 																	    *
 * HeaterControl is distributed in the hope that it will be useful,	    *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	    *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	    *
 * GNU General Public License for more details.						    *
 * 																	    *
 * You should have received a copy of the GNU General Public License    *
 * along with HeaterControl; if not, write to the Free Software		    *
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 *
 * USA																    *
 * Written and (c) by mru											    *
 * Contact <mru@sisyphus.teil.cc> for comment & bug reports				*
 ************************************************************************/
/* 
 * mru, november 2009
 *
 *
 * common include stuff, very fragile
 *
 */

#pragma once
#include <util/delay.h>
#include <stdint.h>
#include <math.h>


typedef unsigned char bool;
typedef uint32_t date_t;

struct global_settings_t {
};



// these variables constitute the status and the setup of the
// controller. some are also persisted into eeprom, see
// eeprom.h/eeprom.c for initialization.
extern struct global_settings_t settings;

#ifndef INIT_DEBUG
#  define LOG_INIT()
#  define LOG_INIT_EXIT()
#else
#  define LOG_INIT() uart_puts ( "<" __FILE__  )
#  define LOG_INIT_EXIT() uart_puts ( ">" NEWLINE )
#endif


#define NEWLINE "\n"
#define DEGREE_SYMBOL "\xdf"


#ifndef NULL
#  define NULL ((void*)(0))
#endif

#define nop() do { asm volatile("nop"); } while (0)


// include everything from here for simplicity
#include "uart.h"
#include "version.h"
#include "build-id.h"
#include "err.h"
#include "adc.h"
#include "ln5623.h"
#include "regaccess.h"
