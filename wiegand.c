/*
 * wiegand.c
 *
 *  Created on: 19/12/2013
 *      Author: Administrador
 */

#include <msp430.h>
#include "hardware.h"
#include "wiegand.h"

static unsigned char count;
static unsigned long int data;
static unsigned long int temp;
#define WIEGAND_BIT_LENGHT 26

void init_wiegand(void)
{
	count=WIEGAND_BIT_LENGHT;

}


/*
 *       40us
 *     |---- |							   |---- |
 *     |     |							   |     |
 *     |     |			2ms				   |     |
 * ----|     |--------------//-------------|     |-------
 *
 *
 *
 */





#pragma vector=PORT1_VECTOR
__interrupt void isr_wiegand(void)

{

	if(	P1IFG & WIEGAND_0 )
	{

		P1IFG &=~WIEGAND_0;  // Clear Flag

//		P1OUT = RED | PULLUPS;
		if (count)					// Avoid process a new card if previos card was not read by main
		count--;
		data<<=1;
		data|=0;
	}




	if(	P1IFG & WIEGAND_1 )
	{

		P1IFG &=~WIEGAND_1;

//		P1OUT = GREEN | PULLUPS;
		if (count)						// Avoid process a new card if previos card was not read by main
		count--;
		data<<=1;
		data|=1;
	}


}


unsigned char wiegand_get_status(void)
{

	return(!count);

}


/*================================================
 * get_card_facility code
 * This functions returns the 8 bit FC
 *
 *===============================================*/
unsigned int wiegand_get_card_fc(void)
{
		temp=data;		//preserve data
		temp>>=17;		//align 8 bit Facility Code to lsb
		temp&=0xFF; 	// Keep 8 bit FC
		count=WIEGAND_BIT_LENGHT; //Restart counter for next reading
		return(temp);   // return data for caller
}


/*================================================
 * Get_card_number
 * This functions returns the 16 bit card number
 *
 *===============================================*/
unsigned int wiegand_get_card_number(void)
{
		temp=data;		//preserve data
		temp>>=1;		//align 16 bit Card Number by stripping out trailing parity bit
		temp&=0xFFFF; 	// Keep 16 bit card number
		count=WIEGAND_BIT_LENGHT; //Restart counter for next reading
		return(temp);   // return data for caller
}
