#include <msp430.h>
#include "hardware.h"


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




void init_timer(void)
{


	   TA0CTL=TIMER_CONTROL;


	   TACCR0=10000;  // [en useg @DCO 1Mhz]i.e. 10mseg
}



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

	}


}




void TAOI_handler(void)
{
	P1OUT ^= GREEN;
}



void CC1_handler(void)
{

}



void CC2_handler(void)
{

}






