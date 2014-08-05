/*
 * fsmtable.c
 *
 *  Created on: 28/07/2014
 *      Author: Administrador
 */


#include "hardware.h"
#include "fsm.h"
#include "fsmtable.h"
#include "led.h"
#include "wiegand.h"
#include "timer.h"
#include "EEprom.h"




extern STATE S_ilde[];
extern STATE S_opening_door[];
extern STATE S_delete_card[];
extern STATE S_add_card[];




extern unsigned int leer_cont(void);


static STATE *p2state=NULL;


#define SUPERVISOR_ADD_CARD			24151
#define SUPERVISOR_ERASE_CARD		35907
#define SUPERVISOR_ERASE_ALL_CARD	41255

#define FOUND 1
#define NOT_FOUND 0



// prototipos


/* HABILITAR ESTO
static void open_door(void);
static void close_door(void);
static void erase_table(void);
static void blink_led(void);
static void erase_card(void);
static void add_card(void);
static void do_nothing(void);
*/

//==============================================

#define MAX_CARD 20

void EE_Clear(void);
void EE_Write_Card(u16 address,u16 *pdata);
void EE_Read_Card(u16 address,u16 *pdata);

u16 search_card_table(u16 card_nr,u16 *address);



/*** tablas de estado ***/

/*** IDLE State ***/

STATE S_ilde[]=
{
  {E_REGISTERED_CARD,S_opening_door,open_door}, // if a registered card was recieved open the door
  {E_UNKOWN_CARD,S_ilde,do_nothing},			// Unknown cards will be rejected
  {E_SUP_ERASE_ALL_CARD,S_ilde,erase_table},	// Erase all registered users
  {E_SUP_ERASE_CARD,S_delete_card,blink_led},	// Erase an existing card
  {E_SUP_ADD_CARD,S_add_card,blink_led},		// Add a new card
  {E_UNKNOWN_EV,S_ilde,do_nothing}   		// Unknown Event
};

/*** Opening_door State ***/

STATE S_opening_door[]=
{

	{E_TIME_OUT,S_ilde,close_door},
	{E_UNKNOWN_EV,S_ilde,do_nothing}   		// Unknown Event

};

/*** Delete_card ***/

STATE S_delete_card[]=
{
	{E_TIME_OUT,S_ilde,do_nothing},
	{E_REGISTERED_CARD,S_ilde,erase_card},
	{E_UNKNOWN_EV,S_ilde,do_nothing}   		// Unknown Event

};

//===================== Add card =======================/
// Waits for a new card from card reader
// If no Card is received within a specified time
// Operation ADD_Card sequence will abort
// Otherwise received card will be added

STATE S_add_card[] =
{
	{E_TIME_OUT,S_ilde,do_nothing},     	// if time elapsed abort
	{E_NEW_CARD,S_ilde,add_card},    		// if a new card was received it will be added
	{E_UNKNOWN_EV,S_ilde,do_nothing}   	// Unknown Event
};



//========interfaz=================

STATE *get_state(void)
{
 return (p2state);
}

void set_init_state(void)
{
 p2state=S_ilde;
}

void store_state(STATE *p2new_state)
{
     p2state=p2new_state;        //Set state

}

///=========Rutinas de accion===============


void open_door(void)
{
 		// led_on(RED);;

	    // LatchKey ON //

 		 flash_led(RED,100,0);

 		 Timer_Set_Delay(2000);	//2seg
// 		 while(Timer_Get_Status()!=TIME_OUT)
// 			 ;

// 		led_off(RED);


}

void close_door(void)
{

	// LatchKey off
	led_off(RED);
}

void erase_table(void)
{
	u8 zero=0;
	int i;

		for(i=0;i<MAX_CARD;i+=2)				// Erase Whole table
		{
			WriteEE(&zero,EE_START+i);			// Erase HI / LO
			WriteEE(&zero,EE_START+i+1);

		}
		flash_led(RED,100,3);					//operation OK feed Back
}




void erase_card(void)
{

	u16	address,card_nr;
	u16 nullcard=0x0000;

	card_nr = wiegand_get_card_number();
	if(search_card_table(card_nr,&address)==FOUND) // search for card
	{

			EE_Write_Card(address,&nullcard);		// Card Found erase it
			led_off(GREEN);
			flash_led(RED,100,3);					//operation OK feed Back
	}
	else
	{
			led_off(GREEN);							// stop flashing
	}



}





void add_card(void)
{
u16	address,card_nr;


	card_nr = wiegand_get_card_number();			// Get card to add
	if (search_card_table(card_nr,&address)==FOUND) // Check if it is already registered
	{
		led_off(GREEN);
		return;											// If yes nothing to add
	}


	//watch for an empty slot (0x000)
	if(search_card_table(0x0000,&address)==FOUND) // search for first empty place (0000)
	{

		EE_Write_Card(address,&card_nr);		  // if found add the new card

	}

	led_off(GREEN);
	flash_led(RED,100,3);					//operation OK feed Back
}


void blink_led(void)						// Used to signal that on operation is in progress
{

	 flash_led(GREEN,50,0);
	 Timer_Set_Delay(3000);	//3seg
}


void do_nothing(void)						// Just That :)
{

	led_off(GREEN);
	led_off(RED);
}


//=================================================================



void EE_Write_Card(u16 address,u16 *pdata)
{

u8 dathi,datlo;
		address*=2;
		dathi=(((*pdata)>>8)& 0xFF);
		datlo=((*pdata)&0xFF);

		WriteEE(&dathi,address);
		WriteEE(&datlo,address+1);
}


//void ReadEE(u8 *pdata,u16 address)
void EE_Read_Card(u16 address,u16 *pdata)
{

u8 dathi,datlo,temp;

		address*=2;
		ReadEE(&dathi,address);
		ReadEE(&datlo,address+1);

		temp=dathi;
		*pdata=((temp<<8)& 0xFF00)|datlo;

}



// ===============================================================




unsigned char ParseCard(void)
{

unsigned char event;
unsigned int card_nr;
u16 address;

		//facility_code=wiegand_get_card_fc(); 	// get fc (no lo usamos por ahora somos poquitos :))
		card_nr = wiegand_get_card_number(); 	// then get its data


		switch (card_nr)
		{
		case SUPERVISOR_ADD_CARD:		// supervisor card : Add new user
			event=E_SUP_ADD_CARD;
			break;
		case SUPERVISOR_ERASE_CARD:		// supervisor card : Remove Existing user
			event=E_SUP_ERASE_CARD;
			break;
		case SUPERVISOR_ERASE_ALL_CARD:	// supervisor card : Remove all users
			event=E_SUP_ERASE_ALL_CARD;
			break;
		default:				// If there was not a supervisor card
			if (search_card_table(card_nr,&address)==FOUND)
			event=E_REGISTERED_CARD;	// then it is an registered user OR ..
			else
			event=E_NEW_CARD;		// this is an new unregistered card.
			break;
		}

	return(event);
}




u16 search_card_table(u16 card_nr,u16 *address)
{
unsigned int i=0;
unsigned int rdcard;

	for(i=EE_START ; i<MAX_CARD ; i++) // search the whole table for the card_nr
	{

		EE_Read_Card(i,&rdcard);	// Read EE at address i
		if (rdcard==card_nr)		// to see if present
		{
			*address=i;			// Card address
			return(FOUND);		// Card present
		}


	}

	return(NOT_FOUND); // Card not found in table
}








