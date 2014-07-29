/*
 * fsm.h
 *
 *  Created on: 28/07/2014
 *      Author: Administrador
 */

#ifndef FSM_H_
#define FSM_H_

#define   NULL ((void *)0)

typedef	unsigned char BYTE;
typedef struct tabla_estado STATE;

struct tabla_estado

{
	BYTE  evento;
	STATE *prx_estado;
	void (*p_rut_accion)(void);
};

// Interfaz
STATE* fsm(STATE *p_tabla_estado,BYTE evento_actual);


// EVENTS

enum events {	E_REGISTERED_CARD,   // Card already in EE table
				E_UNKOWN_CARD,		 // not an supervisor nor an user card
				E_ERASE_ALL_CARD,    // Supervisor erase all EE table card
				E_ERASE_CARD,		 // Supervisor Erase one card from table
				E_ADD_CARD,			 // Supervisor add one card to table
				E_NEW_CARD,			 // A new user card ( not present in table)
				E_TIME_OUT,			 // Time out event due to a timer expiration
				E_UNKNOWN_EVENT		 // An Unknown event
};

#endif /* FSM_H_ */
