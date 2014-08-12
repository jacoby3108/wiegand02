/*
 * led.h
 *
 *  Created on: 14/07/2014
 *      Author: Dany
 *
 *
 *
 *
 */


#ifndef LED_H_
#define LED_H_

#define COUNTED 1
#define NORMAL 0

void init_led(void);
void led_on(u8 ledcolor);
void led_off(u8 ledcolor);
void flash_led(u8 ledcolor,UINT interval, u8 count );



void isr_led(void);
#endif /* LED_H_ */


