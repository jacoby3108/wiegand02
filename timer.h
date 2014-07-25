/*
 * timer.h
 *
 *  Created on: 24/05/2014
 *      Author: Administrador
 */

#ifndef TIMER_H_
#define TIMER_H_

void init_timer(void);
void Timer_Set_Delay(UINT dly); // dly in hundredth of seconds
UINT Timer_Get_Status(void);

#define TIME_OUT 1
#define TIMER_IS_RUNNING 2


#endif /* TIMER_H_ */
