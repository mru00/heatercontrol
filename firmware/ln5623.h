/* led 3-digit display */
/* http://www.ledtech.com.tw/newweb/newweb/img/images_product/LN%28M%295623-11%20EWRN.pdf */

/* mru, 2010 */


#pragma once


extern void ln5623_init(void);

extern void ln5623_set_output(uint16_t value, 
							  uint8_t dp);

extern void ln5623_clear_output(void);

