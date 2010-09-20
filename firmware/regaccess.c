
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <util/delay.h>

#include "uart.h"
#include "regaccess.h"


#define OPCODE_READ_BYTE  ( (byte)1 )
#define OPCODE_PING  ( (byte)0 )
#define OPCODE_READ_FLOAT  ( (byte)3 )
#define OPCODE_WRITE_BYTE  ( (byte)2 )
#define OPCODE_READ_SHORT  ( (byte)5 )
#define OPCODE_WRITE_FLOAT  ( (byte)4 )
#define OPCODE_SPECIAL  ( (byte)'s' )
#define OPCODE_WRITE_SHORT  ( (byte)6 )

#define STATUS_FAIL  ( (byte)1 )
#define STATUS_OK  ( (byte)0 )
#define STATUS_NO_ACCESS  ( (byte)3 )
#define STATUS_NO_SUCH_REGISTER  ( (byte)2 )


// ----------------------- file definitions

volatile reg_file_t reg_file;

// ----------------------- accessors


extern float get_kp(void) { return reg_file.reg_float.kp; }
extern void  set_kp(const float f) { reg_file.reg_float.kp = f; }

extern float get_ki(void) { return reg_file.reg_float.ki; }
extern void  set_ki(const float f) { reg_file.reg_float.ki = f; }

extern short get_temp_vorlauf(void) { return reg_file.reg_short.temp_vorlauf; }
extern void  set_temp_vorlauf(const short f) { reg_file.reg_short.temp_vorlauf = f; }

extern short get_temp_abgas(void) { return reg_file.reg_short.temp_abgas; }
extern void  set_temp_abgas(const short f) { reg_file.reg_short.temp_abgas = f; }


// ----------------------- serial comm accessors


static int read_byte_register(int id, byte* value)  {
#ifdef REG_EXPLICIT_ACCESS
  switch(id) {
  default: *value = 0xa5; return STATUS_FAIL;
  }
#else
  *value = *(  ((byte*)&(reg_file.reg_byte)) + id);
  return 0;
#endif
}

static int write_byte_register(int id, const byte value)  {
#ifdef REG_EXPLICIT_ACCESS
  switch(id) {
  default: return STATUS_FAIL;
  }
#else
  *( ((byte*)(&reg_file.reg_byte)) + id) = value;
  return 0;
#endif
}


static int read_float_register(int id, float* value)  {
#ifdef REG_EXPLICIT_ACCESS
  switch(id) {
  case 0: *value = reg_file.reg_float.kp; return STATUS_OK; break;
  case 1: *value = reg_file.reg_float.ki; return STATUS_OK; break;
  default: *value = 0xa5; return STATUS_FAIL;
  }
#else
  *value = *(  ((float*)&(reg_file.reg_float)) + id);
  return 0;
#endif
}

static int write_float_register(int id, const float value)  {
#ifdef REG_EXPLICIT_ACCESS
  switch(id) {
  case 1: reg_file.reg_float.ki = value; return STATUS_OK; break;
  case 0: return STATUS_NO_ACCESS; break;
  default: return STATUS_FAIL;
  }
#else
  *( ((float*)(&reg_file.reg_float)) + id) = value;
  return 0;
#endif
}


static int read_short_register(int id, short* value)  {
#ifdef REG_EXPLICIT_ACCESS
  switch(id) {
  case 0: *value = reg_file.reg_short.temp_vorlauf; return STATUS_OK; break;
  case 1: *value = reg_file.reg_short.temp_abgas; return STATUS_OK; break;
  default: *value = 0xa5; return STATUS_FAIL;
  }
#else
  *value = *(  ((short*)&(reg_file.reg_short)) + id);
  return 0;
#endif
}

static int write_short_register(int id, const short value)  {
#ifdef REG_EXPLICIT_ACCESS
  switch(id) {
  case 0: return STATUS_NO_ACCESS; break;
  case 1: return STATUS_NO_ACCESS; break;
  default: return STATUS_FAIL;
  }
#else
  *( ((short*)(&reg_file.reg_short)) + id) = value;
  return 0;
#endif
}


static byte receive_byte(void) {
  unsigned int v;
  while ( (v=uart_getc()) & UART_NO_DATA ) ;
  return (byte) v&0xff;
}

static float receive_float(void) {
  char b[4];
  unsigned int v, i;
  for (i = 0; i < 4; i ++ ) {
	while ( (v=uart_getc()) & UART_NO_DATA ) ;
	b[i] = (byte) v&0xff;
  }
  return *( (float*) b );
}

static short receive_short(void) {
  char b[2];
  unsigned int v, i;
  for (i = 0; i < 2; i ++ ) {
	while ( (v=uart_getc()) & UART_NO_DATA ) ;
	b[i] = (byte) v&0xff;
  }
  return *( (short*) b );
}


static void send_byte(byte value) {
  uart_putc(value);
}

static void send_float(float value) {
  unsigned int i;
  for (i = 0; i < 4; i ++ ) {
	uart_putc( ( (byte*)&value) [i] );
  }
}

static void send_short(short value) {
  unsigned int i;
  for (i = 0; i < 2; i ++ ) {
	uart_putc( ( (byte*)&value) [i] );
  }
}

extern void receive(char ch) {
  int id, status;
  union {
	float f;
	byte b;
	short s;
  } v;


  uart_puts("received:");
  _delay_ms(100);
  uart_putc(ch);
  uart_putc(OPCODE_PING);


  switch (ch) {
  case OPCODE_PING: 
	uart_putc(0);
	break;
  case OPCODE_READ_BYTE:
	id = receive_byte();
	status = read_byte_register(id, &v.b);
	uart_putc(status);
	send_byte(v.b);
	break;
  case OPCODE_WRITE_BYTE:
	id = receive_byte();
	v.b = receive_byte();
	status = write_byte_register(id, v.b);
	uart_putc(status);
	break;
  case OPCODE_READ_FLOAT:
	id = receive_float();
	status = read_float_register(id, &v.f);
	uart_putc(status);
	send_float(v.f);
	break;
  case OPCODE_WRITE_FLOAT:
	id = receive_byte();
	v.f = receive_float();
	status = write_float_register(id, v.f);
	uart_putc(status);
	break;
  case OPCODE_READ_SHORT:
	id = receive_byte();
	read_short_register(id, &v.s);
	uart_putc(status);
	send_short(v.s);
	break;
  case OPCODE_WRITE_SHORT:
	id = receive_byte();
	v.f = receive_short();
	status = write_short_register(id, v.s);
	uart_putc(status);
	break;
  case OPCODE_SPECIAL:
	uart_putc(1);
	break;
  default:
	uart_putc(1);
  }

}



