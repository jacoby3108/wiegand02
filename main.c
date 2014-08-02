#include <msp430.h> 
#include "hardware.h"
#include "wiegand.h"
#include "timer.h"
#include "led.h"
#include "iic.h"
#include "EEprom.h"
#include "fsm.h"
#include "fsmtable.h"
#include "cqueue.h"

/*
 * main.c v02 test
 */

// Used Processor msp430g2553 //

unsigned char chusma1,chusma2,chusma3,chusma4;

unsigned char lamp;
unsigned char card_fc[4];    // 8 bit i.e  3 digit + \0
unsigned char card_number[6]; //16 bit i.e. 5 digit + \0

unsigned int facility_code;
// unsigned int card_nr;


unsigned char *itoa(unsigned int num, unsigned char *str, int radix);
unsigned int search_card(void);
void ProcessEvents(void);



#define FOUND 1
#define NOT_FOUND 0




int temp;
unsigned int test16=0x1234;
u8 dathi,datlo;
u8 eve;


unsigned int card_table[]= {
					24151,
					35907,
					41255,
					39447,
					30293,
					35516,
					37822,
					22729,
					37846,
					39580,
					36859,
					27102,
					27111,
					41881,
					00000		// EOT

				};

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    chusma1=BCSCTL1;
    chusma2=CALBC1_1MHZ;

    chusma3=DCOCTL;
    chusma4=CALDCO_1MHZ;



    dathi=(test16>>8)& 0xFF;
    datlo=((test16)& 0xFF);



    /// P1SEL &=~(BUTTON |GREEN | RED );

    P1IES &= ~ (WIEGAND_0 | WIEGAND_1); //Rising edge

    P1REN |= PULLUPS;  // Enable Pull up/down resistors (THIS DISABLES SMCLK WHY ???)

    P1IFG &= ~( P1_4 | P1_5); // Clear pending flags

    P1IE |= (P1_4 | P1_5);   // Enable P1 Interrupts


    init_led();
    init_wiegand();
    init_timer();
    i2c_init(); // init I2C

    led_off(RED);
    led_off(GREEN);

    QueueInit();
	set_init_state(); 	// Inicializo la FSM con el estado inicial


    _BIS_SR(GIE); // Enable General interrupts

 //testi2c();

    while(1)
    {
    	ProcessEvents();
		if (QueueStatus())
		{
			eve=PullQueue();
			store_state(fsm(get_state(),eve));      //Se lo paso a la maquina de estados
		}

	}



}


//======================================================

    void ProcessEvents(void)
    {

    unsigned int event;
    	/* Card Reader Events */

    		if(wiegand_get_status())  // if a card was detected by the card reader
    		{
    			event=ParseCard();	  // parse it
    			PushQueue(event);		 // send to event queue
    		}

    	/* Timer Events */

    		else if(Timer_Get_Status()==TIME_OUT)
    		{
    			event=E_TIME_OUT;
    			PushQueue(event);;		 // send to event queue
    		}
    }







// ============================================================

// OLD TEST Dont delete



/*
	for(;;) {
		volatile unsigned int i;	// volatile to prevent optimization

		// P1OUT ^= lamp;				// Toggle P1.0 using exclusive-OR

		i = 10000;				// SW Delay
		do i--;
		while(i != 0);

		if(wiegand_get_status())  // if a card was read
		{

			facility_code=wiegand_get_card_fc(); // get fc
			itoa(facility_code,card_fc,10); //Convert to string
			card_nr= wiegand_get_card_number(); // then get its data
			itoa(card_nr,card_number,10); //Convert to string

			if(search_card()==FOUND)			//search card on table
			open_door();

		}



	}


	return 0;
}

*/

/*
unsigned int search_card(unsigned int card_nr;)
{
int i=0;
	while (card_table[i])
	{
			if (card_table[i++]==card_nr)

				return(FOUND);

	}

	return(NOT_FOUND);
}
*/



/*******************************************************
* Code contributed by Chris Takahashi,                 *
* ctakahashi (at) users (dot) sourceforge (dot) net.   *
* See stdlib.h for licence.                            *
* $Date: 2005/08/31 11:39:47 $                         *
*******************************************************/

/*
unsigned char *itoa(unsigned int num,unsigned char *str, int radix) {
    char sign = 0;
    char temp[33];  //an int can only be 32 bits long
                    //at radix 2 (binary) the string
                    //is at most 16 + 1 null long.
    int temp_loc = 0;
    int digit;
    int str_loc = 0;

    //save sign for radix 10 conversion
    if (radix == 10 && num < 0) {
        sign = 1;
        num = -num;
    }

    //construct a backward string of the number.
    do {
        digit = (unsigned long)num % radix;
        if (digit < 10)
            temp[temp_loc++] = digit + '0';
        else
            temp[temp_loc++] = digit - 10 + 'A';
        num /= radix;
    } while (num > 0);

    //now add the sign for radix 10
    if (radix == 10 && sign) {
        temp[temp_loc] = '-';
    } else {
        temp_loc--;
    }


    //now reverse the string.
    while ( temp_loc >=0 ) {// while there are still chars
        str[str_loc++] = temp[temp_loc--];
    }
    str[str_loc] = 0; // add null termination.

    return str;
}

*/

// ======================================================================================

/*===================================
 *
 * TEST CODE for iic
 *
 *==================================
 */
u8 txdata[] = {'H', 'O', 'L', 'A', '1', '2 ', 'W', 'O', 'R', 'L', 'D'};
u8 rxdata[40] = {1, 2, 3};
int i;
void testi2c(void) {
u16 card=0x1234;

//   __delay_cycles(20000); //Just a start up delay

//	for (i=0;i<=4;i++)
//	WriteEE(&txdata[i],0x1234+i);

	//for (i=0;i<=4;i++)
    //ReadEE(&rxdata[i],0x1234+i);

	// EE_Clear();

//	EE_Write_Card(0x0000,&card);
	card=0xABCD;
	EE_Read_Card(0x0000,&card);

	//i2c_trans(MAX_CARD, 0xA0, rxdata, 0x0000,READ_M); //i2c RX 1 byte


	for(;;){

		while (!i2c_ready());	// wait for read operation complete
		i++;
	   }


}

