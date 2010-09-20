#ifndef _regaccess_h
#define _regaccess_h

typedef uint8_t byte;
#define REG_EXPLICIT_ACCESS

// ----------------------- file definitions

typedef struct {
  byte led_on;     // 
} reg_byte_t;

typedef struct {
  float kp;     // some documentation about this entry
  float ki;     // 
  float kd;     // 
  float abgas_v1190;     // 
  float abgas_amp_gain;     // 
  float vorlauf_v1190;     // 
  float vorlauf_amp_gain;     // 
} reg_float_t;

typedef struct {
  short temp_vorlauf;     // 
  short temp_abgas;     // 
  short temp_ambient;     // 
  short controller_output;     // 
} reg_short_t;


// register in memory layout
typedef struct  {
  reg_byte_t reg_byte;
  reg_float_t reg_float;
  reg_short_t reg_short;
} reg_file_t;


extern volatile reg_file_t reg_file;




// ----------------------- accessors


extern byte get_led_on(void);
extern void  set_led_on(const byte f);

extern float get_kp(void);
extern void  set_kp(const float f);

extern float get_ki(void);
extern void  set_ki(const float f);

extern float get_kd(void);
extern void  set_kd(const float f);

extern float get_abgas_v1190(void);
extern void  set_abgas_v1190(const float f);

extern float get_abgas_amp_gain(void);
extern void  set_abgas_amp_gain(const float f);

extern float get_vorlauf_v1190(void);
extern void  set_vorlauf_v1190(const float f);

extern float get_vorlauf_amp_gain(void);
extern void  set_vorlauf_amp_gain(const float f);

extern short get_temp_vorlauf(void);
extern void  set_temp_vorlauf(const short f);

extern short get_temp_abgas(void);
extern void  set_temp_abgas(const short f);

extern short get_temp_ambient(void);
extern void  set_temp_ambient(const short f);

extern short get_controller_output(void);
extern void  set_controller_output(const short f);

// ----------------------- serial comm accessors


extern void receive_reg(const uint8_t ch);

extern void regfile_init(void);

#endif

