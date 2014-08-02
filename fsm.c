/*
 * fsm.c
 *
 *  Created on: 28/07/2014
 *      Author: Administrador
 */


#include "fsm.h"
#include "fsmtable.h"



STATE* fsm(STATE *p_tabla_estado,BYTE evento_actual)
{

//      printf(" >>%c<<\n ",evento_actual); //just for test (debug)
   	while (p_tabla_estado -> evento != evento_actual		//Recorre las tablas de estado
		   && p_tabla_estado -> evento != E_UNKNOWN_EV)
		++p_tabla_estado;
      (*p_tabla_estado -> p_rut_accion) ();          /*rutina de accion corresondiente*/
      p_tabla_estado=p_tabla_estado -> prx_estado;   /*siguiente estado*/


      return(p_tabla_estado);

}





