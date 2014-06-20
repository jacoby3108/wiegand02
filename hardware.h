/*
 * hardware.h
 *
 *  Created on: 19/12/2013
 *      Author: Administrador
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#define P1_0 (1<<0)
#define P1_1 (1<<1)
#define P1_2 (1<<2)
#define P1_3 (1<<3)
#define P1_4 (1<<4)
#define P1_5 (1<<5)
#define P1_6 (1<<6)
#define P1_7 (1<<7)

// Attached devices

#define  RED  P1_0
#define  GREEN P1_6
#define  BUTTON P1_3

#define  WIEGAND_0 P1_4
#define  WIEGAND_1 P1_5

#define IO_FUNCTION 0

#define  LED   RED    // Cambiar aca el color del led RED or GREEN

#define PULLUPS (BUTTON | P1_4 | P1_5)





#endif /* HARDWARE_H_ */
