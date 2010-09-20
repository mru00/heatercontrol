#pragma once

typedef uint8_t byte;
#define REG_EXPLICIT_ACCESS

// ----------------------- file definitions

typedef struct {
} reg_byte_t;

typedef struct {
  float kp;     // some documentation about this entry
  float ki;     // 
} reg_float_t;

typedef struct {
  short temp_vorlauf;     // 
  short temp_abgas;     // 
} reg_short_t;


// register in memory layout
typedef struct  {
  reg_byte_t reg_byte;
  reg_float_t reg_float;
  reg_short_t reg_short;
} reg_file_t;


extern volatile reg_file_t reg_file;


// persistance of variables: eeprom layout

typedef struct {
  float pers_float_ki;
} reg_file_persist_t;


// ----------------------- accessors


extern float get_kp(void);
extern void  set_kp(const float f);

extern float get_ki(void);
extern void  set_ki(const float f);

extern short get_temp_vorlauf(void);
extern void  set_temp_vorlauf(const short f);

extern short get_temp_abgas(void);
extern void  set_temp_abgas(const short f);


// ----------------------- serial comm accessors


extern void receive(char ch);

