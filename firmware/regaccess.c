#include <avr/eeprom.h>

#include "common.h"

#define EEPROM_VALID_MASK 0xA5

#define REG_OP_READ_BYTE 1
#define REG_OP_PING 0
#define REG_OP_READ_FLOAT 3
#define REG_OP_WRITE_BYTE 2
#define REG_OP_READ_SHORT 5
#define REG_OP_WRITE_FLOAT 4
#define REG_OP_SPECIAL 's'
#define REG_OP_WRITE_SHORT 6

#define REG_ST_FAIL  1
#define REG_ST_OK  0
#define REG_ST_NO_ACCESS  3
#define REG_ST_NO_SUCH_REGISTER  2
#define REG_ST_NOT_IMPLEMENTED  5
#define REG_ST_INVALID_OPCODE  4
#define REG_ST_PONG  6


// ----------------------- file definitions

volatile reg_file_t reg_file;


// persistance of variables: eeprom layout

typedef struct {
  float pers_float_kp;
  float pers_float_ki;
  float pers_float_kd;
  short pers_short_temp_ambient;
} reg_file_persist_t;


reg_file_persist_t reg_persist_eeprom EEMEM;
uint8_t ee_valid_configuration EEMEM;


extern void regfile_init(void) {
  reg_file.reg_byte.led_on = 0;
  reg_file.reg_float.kp = 1.0;
  reg_file.reg_float.ki = 0.0;
  reg_file.reg_float.kd = 0.0;
  reg_file.reg_float.abgas_v1190 = 69.553e-3;
  reg_file.reg_float.abgas_amp_gain = 50.0;
  reg_file.reg_float.vorlauf_v1190 = 69.553e-3;
  reg_file.reg_float.vorlauf_amp_gain = 50.0;
  reg_file.reg_short.temp_vorlauf = 0;
  reg_file.reg_short.temp_abgas = 0;
  reg_file.reg_short.temp_ambient = 25;
  reg_file.reg_short.controller_output = 0;
	
	if ( eeprom_read_byte(&ee_valid_configuration) == EEPROM_VALID_MASK) {
	  
	  eeprom_read_block ((void *) &reg_file.reg_float.kp, 
						 (const void *) &reg_persist_eeprom.pers_float_kp, 
						 sizeof(float));
	  eeprom_read_block ((void *) &reg_file.reg_float.ki, 
						 (const void *) &reg_persist_eeprom.pers_float_ki, 
						 sizeof(float));
	  eeprom_read_block ((void *) &reg_file.reg_float.kd, 
						 (const void *) &reg_persist_eeprom.pers_float_kd, 
						 sizeof(float));
	  eeprom_read_block ((void *) &reg_file.reg_short.temp_ambient, 
						 (const void *) &reg_persist_eeprom.pers_short_temp_ambient, 
						 sizeof(short));
	}
	else {
	  eeprom_write_block( (const void*) &reg_file.reg_float.kp, 
						  &reg_persist_eeprom.pers_float_kp, 
						  sizeof(float));
	  eeprom_write_block( (const void*) &reg_file.reg_float.ki, 
						  &reg_persist_eeprom.pers_float_ki, 
						  sizeof(float));
	  eeprom_write_block( (const void*) &reg_file.reg_float.kd, 
						  &reg_persist_eeprom.pers_float_kd, 
						  sizeof(float));
	  eeprom_write_block( (const void*) &reg_file.reg_short.temp_ambient, 
						  &reg_persist_eeprom.pers_short_temp_ambient, 
						  sizeof(short));
	eeprom_write_byte(&ee_valid_configuration, EEPROM_VALID_MASK);
	}
}



// ----------------------- file accessors


extern byte get_led_on(void) { return reg_file.reg_byte.led_on; }
extern void  set_led_on(const byte f) {   
  reg_file.reg_byte.led_on = f; 
}

extern float get_kp(void) { return reg_file.reg_float.kp; }
extern void  set_kp(const float f) {   
  reg_file.reg_float.kp = f; 
  eeprom_write_block( (const void*) &reg_file.reg_float.kp, 
					  &reg_persist_eeprom.pers_float_kp, 
					  sizeof(float));  
}

extern float get_ki(void) { return reg_file.reg_float.ki; }
extern void  set_ki(const float f) {   
  reg_file.reg_float.ki = f; 
  eeprom_write_block( (const void*) &reg_file.reg_float.ki, 
					  &reg_persist_eeprom.pers_float_ki, 
					  sizeof(float));  
}

extern float get_kd(void) { return reg_file.reg_float.kd; }
extern void  set_kd(const float f) {   
  reg_file.reg_float.kd = f; 
  eeprom_write_block( (const void*) &reg_file.reg_float.kd, 
					  &reg_persist_eeprom.pers_float_kd, 
					  sizeof(float));  
}

extern float get_abgas_v1190(void) { return reg_file.reg_float.abgas_v1190; }
extern void  set_abgas_v1190(const float f) {   
  reg_file.reg_float.abgas_v1190 = f; 
}

extern float get_abgas_amp_gain(void) { return reg_file.reg_float.abgas_amp_gain; }
extern void  set_abgas_amp_gain(const float f) {   
  reg_file.reg_float.abgas_amp_gain = f; 
}

extern float get_vorlauf_v1190(void) { return reg_file.reg_float.vorlauf_v1190; }
extern void  set_vorlauf_v1190(const float f) {   
  reg_file.reg_float.vorlauf_v1190 = f; 
}

extern float get_vorlauf_amp_gain(void) { return reg_file.reg_float.vorlauf_amp_gain; }
extern void  set_vorlauf_amp_gain(const float f) {   
  reg_file.reg_float.vorlauf_amp_gain = f; 
}

extern short get_temp_vorlauf(void) { return reg_file.reg_short.temp_vorlauf; }
extern void  set_temp_vorlauf(const short f) {   
  reg_file.reg_short.temp_vorlauf = f; 
}

extern short get_temp_abgas(void) { return reg_file.reg_short.temp_abgas; }
extern void  set_temp_abgas(const short f) {   
  reg_file.reg_short.temp_abgas = f; 
}

extern short get_temp_ambient(void) { return reg_file.reg_short.temp_ambient; }
extern void  set_temp_ambient(const short f) {   
  reg_file.reg_short.temp_ambient = f; 
  eeprom_write_block( (const void*) &reg_file.reg_short.temp_ambient, 
					  &reg_persist_eeprom.pers_short_temp_ambient, 
					  sizeof(short));  
}

extern short get_controller_output(void) { return reg_file.reg_short.controller_output; }
extern void  set_controller_output(const short f) {   
  reg_file.reg_short.controller_output = f; 
}




static byte read_byte_register(const byte id, byte* value)  {
  switch(id) {
  case 0: *value = get_led_on(); return REG_ST_OK; break;
  }

  *value = 0; 
  return REG_ST_NO_SUCH_REGISTER;
}

static byte write_byte_register(const byte id, const byte value)  {
  
  switch(id) {
  case 0: set_led_on(value); return REG_ST_OK; break;
  }
  return REG_ST_NO_SUCH_REGISTER;
}


static byte read_float_register(const byte id, float* value)  {
  switch(id) {
  case 0: *value = get_kp(); return REG_ST_OK; break;
  case 1: *value = get_ki(); return REG_ST_OK; break;
  case 2: *value = get_kd(); return REG_ST_OK; break;
  case 3: *value = get_abgas_v1190(); return REG_ST_OK; break;
  case 4: *value = get_abgas_amp_gain(); return REG_ST_OK; break;
  case 5: *value = get_vorlauf_v1190(); return REG_ST_OK; break;
  case 6: *value = get_vorlauf_amp_gain(); return REG_ST_OK; break;
  }

  *value = 0; 
  return REG_ST_NO_SUCH_REGISTER;
}

static byte write_float_register(const byte id, const float value)  {
  
  switch(id) {
  case 0: set_kp(value); return REG_ST_OK; break;
  case 1: set_ki(value); return REG_ST_OK; break;
  case 2: set_kd(value); return REG_ST_OK; break;
  case 3: set_abgas_v1190(value); return REG_ST_OK; break;
  case 4: set_abgas_amp_gain(value); return REG_ST_OK; break;
  case 5: set_vorlauf_v1190(value); return REG_ST_OK; break;
  case 6: set_vorlauf_amp_gain(value); return REG_ST_OK; break;
  }
  return REG_ST_NO_SUCH_REGISTER;
}


static byte read_short_register(const byte id, short* value)  {
  switch(id) {
  case 0: *value = get_temp_vorlauf(); return REG_ST_OK; break;
  case 1: *value = get_temp_abgas(); return REG_ST_OK; break;
  case 2: *value = get_temp_ambient(); return REG_ST_OK; break;
  case 3: *value = get_controller_output(); return REG_ST_OK; break;
  }

  *value = 0; 
  return REG_ST_NO_SUCH_REGISTER;
}

static byte write_short_register(const byte id, const short value)  {
  
  switch(id) {
  case 0: return REG_ST_NO_ACCESS; break;
  case 1: return REG_ST_NO_ACCESS; break;
  case 2: return REG_ST_NO_ACCESS; break;
  case 3: return REG_ST_NO_ACCESS; break;
  }
  return REG_ST_NO_SUCH_REGISTER;
}




// ----------------------- serial comm accessors


static byte receive_byte(void) {
  unsigned int v;
  while ( (v=uart_getc()) & UART_NO_DATA );
  return (byte) v&0xff;
}

static float receive_float(void) {
  union {
	char b[4];
	float f;
  } v;

  v.b[0] = receive_byte();
  v.b[1] = receive_byte();
  v.b[2] = receive_byte();
  v.b[3] = receive_byte();
  return v.f;
}

static short receive_short(void) {
  char b[2];
  unsigned int i;
  for (i = 0; i < 2; i ++ ) {
	b[i] = receive_byte();
  }
  return *( (short*) b );
}

static void send_byte(const byte value) {
  uart_putc(value);
}

static void send_float(const float value) {
  union {
	char b[4];
	float f;
  } v;
  v.f = value;

  send_byte( v.b[0] );
  send_byte( v.b[1] );
  send_byte( v.b[2] );
  send_byte( v.b[3] );
}

static void send_short(const short value) {
  send_byte( ( (byte*)&value) [0] );
  send_byte( ( (byte*)&value) [1] );
}

extern void receive_reg(const uint8_t ch) {

  byte id = 0;
  byte status = 0;
  union {
	float f;
	byte b;
	short s;
  } v;

  if ( REG_OP_PING == ch ) {
	uart_putc(REG_ST_PONG);
  } else if ( REG_OP_READ_BYTE == ch ) {
	id = receive_byte();
	status = read_byte_register(id, &v.b);
	uart_putc(status);
	send_byte(v.b);
  } else if (REG_OP_WRITE_BYTE == ch) {
	id = receive_byte();
	v.b = receive_byte();
	status = write_byte_register(id, v.b);
	uart_putc(status);
  } else if (ch == REG_OP_READ_FLOAT) {
	id = receive_byte();
	status = read_float_register(id, &v.f);
	uart_putc(status);
	send_float(v.f);
  } else if (ch == REG_OP_WRITE_FLOAT) {
	id = receive_byte();
	v.f = receive_float();
	status = write_float_register(id, v.f);
	uart_putc(status);
  } else if (ch == REG_OP_READ_SHORT) {
	id = receive_byte();
	read_short_register(id, &v.s);
	uart_putc(status);
	send_short(v.s);
  } else if (ch == REG_OP_WRITE_SHORT) {
	id = receive_byte();
	v.s = receive_short();
	status = write_short_register(id, v.s);
	uart_putc(status);
  } else if (ch == REG_OP_SPECIAL) {
	uart_putc(REG_ST_NOT_IMPLEMENTED);
  }  else{
	uart_putc(REG_ST_INVALID_OPCODE);
  }
}



