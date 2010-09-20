
#ifndef _ERR_H_
#define _ERR_H_

#include <stdint.h>

// error codes follow the win32 HRESULT layout (somehow)

// layout:
//
// s s 0 0 f f f f | e e e e e e e e 
// where s : severity
//       f : facility
//       e : error code


typedef uint16_t status_t;

// facilities
#define FAC_GEN                  0x00
#define FAC_ADC                  0x01
#define FAC_UART                 0x02
#define FAC_LCD                  0x03
#define FAC_PROT                 0x04
#define FAC_EEPROM               0x05
#define FAC_QUE                  0x06
#define FAC_TWI                  0x07


// severity codes
#define SEV_SUCCESS              0x00
#define SEV_WARNING              0x01
#define SEV_ERROR                0x02
#define SEV_FATAL                0x03

// just internal
#define _ERR_CODE(err,fac,sev)   ( (sev<<14) | (fac<<8) | err ) 


// get information about the error code
#define SUCCEEDED(errcode)       ( ((errcode) & 0x8000) == 0 )
#define FAILED(errcode)          ( (errcode) & 0x8000 )
#define ERROR(errcode)           ( (errcode) & 0x00ff )


// gen
#define NO_ERR                   _ERR_CODE(0,FAC_GEN,SEV_SUCCESS)
#define ERR_NOT_IMPL             _ERR_CODE(1,FAC_GEN,SEV_FATAL)
#define ERR_NULLPOINTER          _ERR_CODE(2,FAC_GEN,SEV_FATAL)
#define ERR_INVALID_HANDLE       _ERR_CODE(3,FAC_GEN,SEV_ERROR)
#define ERR_MALLOC_FAILED        _ERR_CODE(4,FAC_GEN,SEV_FATAL)


// adc
#define ERR_ADC_NOT_ENABLED      _ERR_CODE(1,FAC_ADC,SEV_ERROR)
#define ERR_ADC_ILLEGAL_CHANNEL  _ERR_CODE(2,FAC_ADC,SEV_ERROR)

// protocol
#define ERR_PROT_UNKNOWN         _ERR_CODE(1,FAC_PROT,SEV_ERROR)
#define ERR_PROT_COMMAND_RANGE   _ERR_CODE(2,FAC_PROT,SEV_ERROR)
#define ERR_PROT_HANDLER_FAILED  _ERR_CODE(3,FAC_PROT,SEV_ERROR)

// eeprom
#define ERR_EEPROM_NOT_READY     _ERR_CODE(1,FAC_EEPROM,SEV_ERROR)

// que
#define ERR_QUE_FULL             _ERR_CODE(1,FAC_QUE,SEV_ERROR)
#define ERR_QUE_EMPTY            _ERR_CODE(2,FAC_QUE,SEV_ERROR)
#define ERR_QUE_ILLEGAL_INDEX    _ERR_CODE(3,FAC_QUE,SEV_ERROR)

// twi
// the twi defines its own error message. that will be in the
// lowbyte....
#define ERR_TWI                  _ERR_CODE(0,FAC_TWI,SEV_ERROR)


#endif
