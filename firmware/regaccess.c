#include <avr/eeprom.h>

#include "common.h"

#define EEPROM_VALID_MASK 0xA5

enum {
REG_OP_write_float_register,
REG_OP_write_short_register,
REG_OP_read_float_register,
REG_OP_read_short_register,
REG_OP_set_led,
REG_OP_get_if_version,
REG_OP_ping,
REG_OP_ln5623_set_output,
};

#define REG_ST_FAIL  1
#define REG_ST_OK  0
#define REG_ST_NO_ACCESS  3
#define REG_ST_NO_SUCH_REGISTER  2
#define REG_ST_NOT_IMPLEMENTED  5
#define REG_ST_INVALID_OPCODE  4
#define REG_ST_PONG  6

byte ping(void) { return 0; }

byte get_fw_version(void) { return 0; }
byte get_if_version(byte* version) { return 2; }


// ----------------------- file definitions

// register in memory layout
typedef struct {
  float kp;     // 
  float ki;     // 
  float kd;     // 
  float abgas_v1190;     // 
  float abgas_amp_gain;     // 
  float vorlauf_v1190;     // 
  float vorlauf_amp_gain;     // 
  short temp_vorlauf;     // 
  short temp_abgas;     // 
  short temp_ambient;     // 
  short controller_output;     // 
} reg_file_t;

volatile reg_file_t reg_file;


// persistance of variables: eeprom layout

typedef struct {
  float kp;
  float ki;
  float kd;
  short temp_ambient;
} reg_file_persist_t;


reg_file_persist_t reg_persist_eeprom EEMEM;
uint8_t ee_valid_configuration EEMEM;


static void __attribute__((constructor))
regfile_autoinit(void) 
{
  reg_file.kp = 1.0;
  reg_file.ki = 0.0;
  reg_file.kd = 0.0;
  reg_file.abgas_v1190 = 69.553e-3;
  reg_file.abgas_amp_gain = 50.0;
  reg_file.vorlauf_v1190 = 69.553e-3;
  reg_file.vorlauf_amp_gain = 50.0;
  reg_file.temp_vorlauf = 0;
  reg_file.temp_abgas = 0;
  reg_file.temp_ambient = 25;
  reg_file.controller_output = 0;
	
  if ( eeprom_read_byte(&ee_valid_configuration) == EEPROM_VALID_MASK) {
	  
    eeprom_read_block ((void *) &reg_file.kp, 
					   (const void *) &reg_persist_eeprom.kp, 
					   sizeof(float));
    eeprom_read_block ((void *) &reg_file.ki, 
					   (const void *) &reg_persist_eeprom.ki, 
					   sizeof(float));
    eeprom_read_block ((void *) &reg_file.kd, 
					   (const void *) &reg_persist_eeprom.kd, 
					   sizeof(float));
    eeprom_read_block ((void *) &reg_file.temp_ambient, 
					   (const void *) &reg_persist_eeprom.temp_ambient, 
					   sizeof(short));
  }
  else {
    eeprom_write_block( (const void*) &reg_file.kp, 
						&reg_persist_eeprom.kp, 
						sizeof(float));
    eeprom_write_block( (const void*) &reg_file.ki, 
						&reg_persist_eeprom.ki, 
						sizeof(float));
    eeprom_write_block( (const void*) &reg_file.kd, 
						&reg_persist_eeprom.kd, 
						sizeof(float));
    eeprom_write_block( (const void*) &reg_file.temp_ambient, 
						&reg_persist_eeprom.temp_ambient, 
						sizeof(short));
    eeprom_write_byte(&ee_valid_configuration, EEPROM_VALID_MASK);
  }
}



// ----------------------- file accessors


extern float get_kp(void) { return reg_file.kp; }
extern void  set_kp(const float v) {   
  reg_file.kp = v; 
  eeprom_write_block( (const void*) &reg_file.kp, 
					  &reg_persist_eeprom.kp, 
					  sizeof(float));  
}

extern float get_ki(void) { return reg_file.ki; }
extern void  set_ki(const float v) {   
  reg_file.ki = v; 
  eeprom_write_block( (const void*) &reg_file.ki, 
					  &reg_persist_eeprom.ki, 
					  sizeof(float));  
}

extern float get_kd(void) { return reg_file.kd; }
extern void  set_kd(const float v) {   
  reg_file.kd = v; 
  eeprom_write_block( (const void*) &reg_file.kd, 
					  &reg_persist_eeprom.kd, 
					  sizeof(float));  
}

extern float get_abgas_v1190(void) { return reg_file.abgas_v1190; }
extern void  set_abgas_v1190(const float v) {   
  reg_file.abgas_v1190 = v; 
}

extern float get_abgas_amp_gain(void) { return reg_file.abgas_amp_gain; }
extern void  set_abgas_amp_gain(const float v) {   
  reg_file.abgas_amp_gain = v; 
}

extern float get_vorlauf_v1190(void) { return reg_file.vorlauf_v1190; }
extern void  set_vorlauf_v1190(const float v) {   
  reg_file.vorlauf_v1190 = v; 
}

extern float get_vorlauf_amp_gain(void) { return reg_file.vorlauf_amp_gain; }
extern void  set_vorlauf_amp_gain(const float v) {   
  reg_file.vorlauf_amp_gain = v; 
}

extern short get_temp_vorlauf(void) { return reg_file.temp_vorlauf; }
extern void  set_temp_vorlauf(const short v) {   
  reg_file.temp_vorlauf = v; 
}

extern short get_temp_abgas(void) { return reg_file.temp_abgas; }
extern void  set_temp_abgas(const short v) {   
  reg_file.temp_abgas = v; 
}

extern short get_temp_ambient(void) { return reg_file.temp_ambient; }
extern void  set_temp_ambient(const short v) {   
  reg_file.temp_ambient = v; 
  eeprom_write_block( (const void*) &reg_file.temp_ambient, 
					  &reg_persist_eeprom.temp_ambient, 
					  sizeof(short));  
}

extern short get_controller_output(void) { return reg_file.controller_output; }
extern void  set_controller_output(const short v) {   
  reg_file.controller_output = v; 
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
  union { char b[4]; float f; } v;
  v.b[0] = receive_byte();
  v.b[1] = receive_byte();
  v.b[2] = receive_byte();
  v.b[3] = receive_byte();
  return v.f;
}

static short receive_short(void) {
  union { char b[2]; short f; } v;
  v.b[0] = receive_byte();
  v.b[1] = receive_byte();
  return v.f;
}

static short receive_ushort(void) {
  union { char b[2]; ushort f; } v;
  v.b[0] = receive_byte();
  v.b[1] = receive_byte();
  return v.f;
}

static void send_byte(const byte value) {
  uart_putc(value);
}

static void send_float(const float value) {
  union { char b[4]; float f; } v;
  v.f = value;

  send_byte( v.b[0] );
  send_byte( v.b[1] );
  send_byte( v.b[2] );
  send_byte( v.b[3] );
}

static void send_short(const short value) {
  union { char b[2]; short f; } v;
  v.f = value;

  send_byte( v.b[0] );
  send_byte( v.b[1] );
}

static void send_ushort(const short value) {
  union { char b[2]; ushort f; } v;
  v.f = value;

  send_byte( v.b[0] );
  send_byte( v.b[1] );
}

extern void on_receive_byte(const uint8_t ch) {

  byte status = 0;

  switch(ch) {
  case REG_OP_write_float_register: {
	  byte id = receive_byte();
	  float value = receive_float();

      status = write_float_register(id,value);

	  send_byte(status);
	}
    break;
  case REG_OP_write_short_register: {
	  byte id = receive_byte();
	  short value = receive_short();

      status = write_short_register(id,value);

	  send_byte(status);
	}
    break;
  case REG_OP_read_float_register: {
	  byte id = receive_byte();
	  float value = 0.0;

      status = read_float_register(id,&value);

	  send_byte(status);
	  if ( REG_ST_OK == status ) {
	    send_float(value);
	  }
	}
    break;
  case REG_OP_read_short_register: {
	  byte id = receive_byte();
	  short value = 0;

      status = read_short_register(id,&value);

	  send_byte(status);
	  if ( REG_ST_OK == status ) {
	    send_short(value);
	  }
	}
    break;
  case REG_OP_set_led: {
	  byte on = receive_byte();

      status = set_led(on);

	  send_byte(status);
	}
    break;
  case REG_OP_get_if_version: {
	  byte version = 0;

      status = get_if_version(&version);

	  send_byte(status);
	  if ( REG_ST_OK == status ) {
	    send_byte(version);
	  }
	}
    break;
  case REG_OP_ping: {

      status = ping();

	  send_byte(status);
	}
    break;
  case REG_OP_ln5623_set_output: {
	  ushort value = receive_ushort();
	  byte dp = receive_byte();

      status = ln5623_set_output(value,dp);

	  send_byte(status);
	}
    break;

  default:
	send_byte(REG_ST_INVALID_OPCODE);
  }
}


