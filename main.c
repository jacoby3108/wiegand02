#include <msp430.h> 
#include "hardware.h"
#include "wiegand.h"
#include "timer.h"
/*
 * main.c v02 test
 */

// Used Processor msp430g2553 //

unsigned char chusma1,chusma2,chusma3,chusma4;

unsigned char lamp;
unsigned char card_fc[4];    // 8 bit i.e  3 digit + \0
unsigned char card_number[6]; //16 bit i.e. 5 digit + \0

unsigned int facility_code;
unsigned int card_nr;


unsigned char *itoa(unsigned int num, unsigned char *str, int radix);
unsigned int search_card(void);
void open_door(void);


#define FOUND 1
#define NOT_FOUND 0
int temp;

unsigned int card_table[]= {	24151,
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
					41880,		// Posta 41881
					00000		// EOT

				};

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    chusma1=BCSCTL1;
    chusma2=CALBC1_1MHZ;

    chusma3=DCOCTL;
    chusma4=CALDCO_1MHZ;


    /// P1SEL &=~(BUTTON |GREEN | RED );

//****************Measurement of SMCLK********************************************************
// if SHOW_SMCLK is enabled (=1) smclk signal is visible on P1.4 useful to measure system clk
// (WARNING !! if this test is enabled only scope should be connected to  P1.4  )
#define SHOW_SMCLK 0		// =1 Show System bus clk / =0 Normal I/O operation
//*****************************************************************************************



#if SHOW_SMCLK     // show SMCLK on P1.4  (WARNING !! only scope should be connected to  P1.4  )

    P1SEL=IO_FUNCTION | P1_4 ;  // select SMCLK as primary function
    P1SEL2=IO_FUNCTION;
    P1DIR |= P1_4;  // P1.4 as output "required"
    for(;;){}  // Endless loop

#else
    P1SEL=IO_FUNCTION;
    P1SEL2=IO_FUNCTION;
    P1DIR |= (GREEN | RED);
#endif



    P1IES &= ~ (WIEGAND_0 | WIEGAND_1); //Rising edge


    P1REN |= PULLUPS;  // Enable Pull up/down resistors (THIS DISABLES SMCLK WHY ???)

    P1IFG &= ~(BUTTON | P1_4 | P1_5); // Clear pending flags
    P1IE |= (P1_4 | P1_5);   // Enable P1 Interrupts

    lamp=GREEN;

    P1OUT = lamp | PULLUPS;



    init_wiegand();
   init_timer();


    _BIS_SR(GIE);



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

void open_door(void)
{
 volatile int i,k;
 int j=4;
 j=4;
 P1OUT = RED | PULLUPS;
 	 while(j--)
 	 {

 		 P1OUT = RED | PULLUPS;

 		 for(k=1000;k;k--){
 			for (i=100;i;i--);
 		 }

 		 P1OUT = ((RED ^ RED) | PULLUPS);

 		 for(k=1000;k;k--){
 			for (i=100;i;i--);
 		 }


 	 }

 P1OUT = GREEN | PULLUPS;
}


unsigned int search_card(void)
{
int i=0;
	while (card_table[i])
	{
			if (card_table[i++]==card_nr)

				return(FOUND);

	}

	return(NOT_FOUND);
}


/*******************************************************
* Code contributed by Chris Takahashi,                 *
* ctakahashi (at) users (dot) sourceforge (dot) net.   *
* See stdlib.h for licence.                            *
* $Date: 2005/08/31 11:39:47 $                         *
*******************************************************/


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



