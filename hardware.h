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
#define  GREEN P1_3
//#define  BUTTON P1_3   // only on launch pad

#define  WIEGAND_0 P1_4
#define  WIEGAND_1 P1_5

#define IO_FUNCTION 0

#define  LED   RED    // Cambiar aca el color del led RED or GREEN

//#define PULLUPS (BUTTON | P1_4 | P1_5) // BUTTON era el boton de la placa launchpad P1.3 ahora uso
/// P1.3 para el led verde que antes estaba en P1.6
// Hubo quer liberar P1.6 pues lo usa iic
// ojo no dejar un pullup en una salida la tira abajo
// Ej si la carga es led serie con 470 ohm cae a 2V en lugar de 3.3v

#define PULLUPS ( P1_4 | P1_5)

typedef unsigned int UINT;


#endif /* HARDWARE_H_ */
