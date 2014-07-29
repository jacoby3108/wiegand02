/*
 * fsmtable.c
 *
 *  Created on: 28/07/2014
 *      Author: Administrador
 */



#include "fsm.h"
#include "fsmtable.h"



extern STATE S_ilde[];
extern STATE S_opening_door[];
extern STATE S_delete_card[];
extern STATE S_add_card[];




extern unsigned int leer_cont(void);


static STATE *p2state=NULL;






// prototipos

static void open_door(void);
static void close_door(void);
static void clear_table(void);
static void flash_led(void);
static void erase_card(void);
static void add_card(void);
static void do_nothing(void);

/*** tablas de estado ***/

/*** IDLE State ***/

 STATE S_ilde[]=
{
  {E_REGISTERED_CARD,S_opening_door,open_door}, // if a registered card was recieved open the door
  {E_UNKOWN_CARD,S_ilde,do_nothing},			// Unknown cards will be rejected
  {E_ERASE_ALL_CARD,S_ilde,clear_table},		// Erase all registered users
  {E_ERASE_CARD,S_delete_card,flash_led},		// Erase an existing card
  {E_ADD_CARD,S_add_card,flash_led},			// Add a new card
  {E_UNKNOWN_EVENT,S_ilde,do_nothing}   		// Unknown Event
};

/*** Opening_door State ***/

STATE S_opening_door[]=
{

	{E_TIME_OUT,S_ilde,close_door},
	{E_UNKNOWN_EVENT,S_ilde,do_nothing}   		// Unknown Event

};

/*** Delete_card ***/

STATE S_delete_card[]=
{
	{E_TIME_OUT,S_ilde,do_nothing},
	{E_REGISTERED_CARD,S_ilde,erase_card},
	{E_UNKNOWN_EVENT,S_ilde,do_nothing}   		// Unknown Event

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
	{E_UNKNOWN_EVENT,S_ilde,do_nothing}   	// Unknown Event
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


}
void close_door(void)
{


}

void clear_table(void)
{


}


void flash_led(void)
{


}


void erase_card(void)
{


}





void add_card(void)
{


}

void do_nothing(void)
{


}









