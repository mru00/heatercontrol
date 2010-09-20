// mru, 2010


/* adc routines for atmega8 */


#ifndef _ADC_H_
#define _ADC_H_



extern status_t adc_init(void);

// selects the channel for a single ended conversion
// (absolute measurement against GND)
extern status_t adc_select_channel(uint8_t channel);


#if defined ( __AVR_ATmega32__)
// selects the channels for a dual-ended conversion
// ( measures V{channel1}-V{channel2} )
extern status_t adc_select_channel_diff(uint8_t ch1, uint8_t ch2, uint8_t g);
#endif

extern status_t adc_single_conversion(uint16_t* retval);

// only valid if internal vref=2.56 is selected
extern status_t adc_result2volt(uint16_t result, float* pVolt, float vref);

#endif
