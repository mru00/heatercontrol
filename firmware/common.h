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
/* 
 * mru, november 2009
 *
 *
 * common include stuff, very fragile
 *
 */

#pragma once

// index in daytime-arrays
enum {
  NIGHT,
  DAY
};

enum {
  DAYTIME_BEGIN,
  DAYTIME_END
};

enum {
  OUTPUT_FIRST,
  OUTPUT_L1 = OUTPUT_FIRST,
  OUTPUT_L2,
  OUTPUT_FOGGER,
  OUTPUT_HEATING_WIRE,
  OUTPUT_HEATING_LAMP,
  OUTPUT_LAST
};


// maximum length of controller title
#define CONTROLER_TITLE_LEN 9
#define N_TIMESWITCHES 8
 


/*  some debug flags */
//#define INIT_DEBUG
//#define COMMANDLINE_DEBUG
//#define TIMESWITCH_DEBUG
//#define PORTMAP_DEBUG
//#define SHT11_DEBUG
//#define HD4478_DEBUG
//#define I2C_DEBUG

/* DUMMY = no hardware flags */
//#define PCF8574A_DUMMY
//#define SHT11_DUMMY
//#define DS1307_DUMMY
//#define HD4478_DUMMY
//#define SHT11_DUMMY_TMP101 1

/*
 * SHT11_DUMMY: puts SHT11 in dummy (=no hardware) mode
 *
 * SHT11_DUMMY_TMP101: puts SHT11 in dummy mode but uses a TMP101
 * to get the temperature. the value of the define is used as 
 * TMP101 address
 */


//#define TIME_STRUCT // das kostet 300 bytes!


#ifdef TIME_STRUCT
struct time_struct {
  uint8_t h, m, s;
};
typedef struct time_struct time_t;
#else
typedef uint32_t time_t;
#endif

typedef unsigned char bool;
typedef uint32_t date_t;
typedef uint8_t  pin_t;
typedef volatile uint8_t* port_t;


struct timeswitch_t {
  time_t on, off;
  uint8_t output;   // sign bit is enabled flag!
};


struct global_settings_t {
  uint8_t temp;
  uint8_t temp_setpoint[2];
  uint8_t humidity;
  uint8_t humidity_setpoint[2];
  time_t  daytime[2];
  date_t  wintertime[2];
  char* controller_title[CONTROLER_TITLE_LEN];

  uint8_t hyst_temp;
  uint8_t hyst_humidity;

  struct timeswitch_t timeswitches[N_TIMESWITCHES];
};


// these variables constitute the status and the setup of the
// controller. some are also persisted into eeprom, see
// eeprom.h/eeprom.c for initialization.
extern struct global_settings_t settings;
extern uint8_t temp;
extern uint8_t humidity;


// which outputs are currently switched on? a bit-mask.
extern uint8_t output_values;


struct port_pin_t {
  uint8_t po:4;
  uint8_t pi:4;
};




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
#include "itoa.h"
#include "uart.h"
#include "time.h"
#include "i2c.h"
#include "timeswitch.h"
#include "portmap.h"
#include "ds1307.h"
#include "commandline.h"
#include "version.h"
#include "build-id.h"
#include "hd4478.h"
#include "selftest.h"
#include "eeprom.h"
#include "sht11.h"
#include "pcf8574a.h"




/*  LocalWords:  eeprom sht pcf
 */
