/*
 * door.c
 *
 *  Created on: 12/08/2014
 *      Author: Administrador
 */

#include "msp430g2553.h"
#include "hardware.h"
#include "door.h"



void init_door(void)
{

	  P2OUT &= ~(DOOR);			// Prevent
	  P2SEL|=IO_FUNCTION;		// Primary function as I/O port
	  P2SEL2|=IO_FUNCTION;
	  P2DIR |= (DOOR);			// set port direction


}


void OpenDoor(void)
{

	 P2OUT |= DOOR;

}

void CloseDoor(void)
{

	 P2OUT &= ~DOOR;

}



