/*
 * hardware.h
 *
 *  Created on: 19/12/2013
 *      Author: Administrador
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#define IO_FUNCTION 0

// Port1
#define P1_0 (1<<0)    //Led
#define P1_1 (1<<1)    //uart
#define P1_2 (1<<2)    // uart
#define P1_3 (1<<3)    //Led
#define P1_4 (1<<4)	   // Lector
#define P1_5 (1<<5)	   // Lector
#define P1_6 (1<<6)		// I2C    scl
#define P1_7 (1<<7)		//  I2C   sda

// Port 2

#define P2_0 (1<<0)    //
#define P2_1 (1<<1)    //
#define P2_2 (1<<2)    //
#define P2_3 (1<<3)    //
#define P2_4 (1<<4)	   //
#define P2_5 (1<<5)	   //

// Hardware resources used

// Card reader
#define  WIEGAND_0 P1_4
#define  WIEGAND_1 P1_5
#define  PULLUPS ( P1_4 | P1_5)

// Leds
#define  RED   P1_0
#define  GREEN P1_3
#define  DOOR  P2_2



///////#define  LED   RED    // Cambiar aca el color del led RED or GREEN



// Types definition

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u16;
typedef signed int s16;



//#define  BUTTON P1_3   // only on launch pad



//#define PULLUPS (BUTTON | P1_4 | P1_5) // BUTTON era el boton de la placa launchpad P1.3 ahora uso
/// P1.3 para el led verde que antes estaba en P1.6
// Hubo quer liberar P1.6 pues lo usa iic
// ojo no dejar un pullup en una salida la tira abajo
// Ej si la carga es led serie con 470 ohm cae a 2V en lugar de 3.3v






typedef unsigned int UINT;

#define FALSE 0
#define TRUE 1

#endif /* HARDWARE_H_ */
