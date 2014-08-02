/*
 * fsmtable.h
 *
 *  Created on: 28/07/2014
 *      Author: Administrador
 */

#ifndef FSMTABLE_H_
#define FSMTABLE_H_

typedef	unsigned char BYTE;
typedef struct tabla_estado STATE;


void set_init_state(void);
STATE *get_state(void);
void store_state(STATE *);


/// estos protos son para probar de main solamente
// despues borrarlos y habilitar en el .C

void open_door(void);
void close_door(void);
void erase_table(void);
void blink_led(void);
void erase_card(void);
void add_card(void);
void do_nothing(void);

// EVENTS

#define  	E_REGISTERED_CARD  0x01   // Card already in EE table
#define		E_UNKOWN_CARD	0x02	 // not an supervisor nor an user card
#define		E_SUP_ERASE_ALL_CARD   0x03 // Supervisor erase all EE table card
#define		E_SUP_ERASE_CARD	0x04	 // Supervisor Erase one card from table
#define		E_SUP_ADD_CARD	0x05		 // Supervisor add one card to table
#define		E_NEW_CARD		0x06	 // A new user card ( not present in table)
#define		E_TIME_OUT		0x07	 // Time out event due to a timer expiration
#define		E_UNKNOWN_EV	0xFF	//Unknown Event





#endif /* FSMTABLE_H_ */
