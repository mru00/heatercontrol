#ifndef _regaccess_h
#define _regaccess_h

typedef uint8_t byte;
typedef uint16_t ushort;

// ----------------------- accessors


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

extern void on_receive_byte(const uint8_t ch);



// ----------------------- declarations of external functions

extern byte set_led(byte on);
extern byte ping(void);
extern byte ln5623_set_output(ushort value,byte dp);

#endif

