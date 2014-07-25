#include <msp430.h>
#include "hardware.h"
#include "timer.h"

// Timer hardware definitions

void TAOI_handler(void);
void CC1_handler(void);
void CC2_handler(void);

#define TACLK  TASSEL_0
#define ACLK   TASSEL_1
#define SMCLK  TASSEL_2
#define INCLK  TASSEL_3

/*=======================



/=======================*/

#define DIV_X_1 ID_0
#define DIV_X_2	ID_1
#define DIV_X_4	ID_2
#define DIV_X_8	ID_3



#define TIM_STOP 		MC_0
#define TIM_UP	 		MC_1	/*Up to CCR0 */
#define TIM_CONTINUOUS 	MC_2
#define TIM_UP_DOWN 	MC_3





#define	CLKSRC			SMCLK
#define	DIVIDER			DIV_X_1
#define	TIM_INTR_ENABLE	TAIE
#define	TMODE			TIM_UP



#define  TIMER_CONTROL (SMCLK | DIV_X_1 | TIM_UP |TIM_INTR_ENABLE)

#define T10ms 10000		// Periodic interrupt interval

//==== Driver internal variables and definitions =====

#define TIMER_TRIGGERED 2
#define TIMER_ILDE 3

static UINT timer;
static UINT timer_status;

// ==================================================


void init_timer(void)
{


	   TA0CTL=TIMER_CONTROL; // Setup hardware timer

	   TACCR0=T10ms;  // [en useg @DCO 1Mhz]i.e. 10mseg

	   timer_status= TIMER_ILDE;  //set Software Timer status to  ILDE  (ready to use)
}

/************Timer ISR*****************/

#pragma vector=TIMER0_A1_VECTOR
__interrupt void isr_timer(void)
{

	int dummy;

	dummy=TA0IV;

	switch(dummy)
	{

		case TA0IV_TAIFG:

			 TAOI_handler();

		break;

		case TA0IV_TACCR1:

			 CC1_handler();

		break;

		case TA0IV_TACCR2:

			 CC2_handler();

		break;


		default:
		break;

	}


}




void TAOI_handler(void)		// hardware interrupt handler
{
	P1OUT ^= GREEN;

	if (timer){							  // if software timer has not expired
		timer--;						  // update count

		if(!timer)						  // if count expired ( change from  1 to 0)
			timer_status=TIMER_TRIGGERED; // signal trigger
	}


}



void CC1_handler(void)
{

}



void CC2_handler(void)
{

}

/*********End  Timer ISR *************/

/********Timer services for delay ***********/

/* ==============================================================================
 * Timer_Set_Delay() Sets software timer expire time in Milliseconds.
 * Timer resolution is 10ms
 * e.g:   Timer_Set_Delay(500);	//500 ms
 * Timer status can be requested using the service: Timer_Get_Status()at
 * any time.
 *
 * ==============================================================================*/

void Timer_Set_Delay(UINT dly) // dly in milisecons (min 100ms)
{
	timer=dly/10;
}

/*================================================================================
 * Timer_Get_Status(void) returns the software timer status
 *
 * return values are: TIME_OUT or TIMER_IS_RUNNING (names are self explanatory).
 *
 * Timer_Get_Status() can be called any time after a Timer_Set_Delay() was issued
 * This function is useful in polled schemas or in main loops.
 *
 * e.g.:     main loop
 *
 * 			 if( Timer_Get_Status() == TIME_OUT)
 *
 * 					do_timed tasks();
 *
 *  		 Continue with main loop
 *
 *
 *
 *========================================================================*/

UINT Timer_Get_Status(void)
{

	if(timer_status==TIMER_TRIGGERED)
	{

		timer_status=TIMER_ILDE;
		return (TIME_OUT);
	}

	return(TIMER_IS_RUNNING);

	return(TIMER_IS_ILDE); //???
}




/*===============================================================
 * Timer_Kill : The purpose of this service is to kill the timer
 *
 * This is done by putting the timer counter to 0
 *
 * This function return information about the timer status at the
 * moment it was called.
 *
 * if the timer was running it returns 0
 * if the timer has already expired (event was already dispatched)
 * it returns 1. This info is useful for timed events in FSM
 * When event was already dispatched (we got too late) FSM has
 * can block the event.
*/
UINT Timer_Kill(void)
{

	_BIC_SR(GIE); 		// Disable General interrupts ( do atomic !!!!!!!!!!!!!!!!!!!)

	if (timer){			//If timer <> 0 => timer is running
		timer=0;		// Therefore  Kill timer
		_BIS_SR(GIE); 	// Re Enable General interrupts
		return (0); 	//Timer was running
	}

	else
		_BIS_SR(GIE); 	// Re Enable General interrupts
		return(1);     //Timer has already expired i.e. timer event has been dispatched

}


