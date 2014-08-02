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

//#define E_UNKNOWN_EVENT	0xFF	 // An Unknown event


#endif /* FSM_H_ */
