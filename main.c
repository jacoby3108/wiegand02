#include <msp430.h> 
#include "hardware.h"
#include "wiegand.h"
#include "timer.h"
#include "led.h"

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
extern unsigned char ready;

#define FOUND 1
#define NOT_FOUND 0
int temp;
unsigned int test16=0x1234;

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

    test16++;

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


    _BIS_SR(GIE); // Enable General interrupts

    testi2c();







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
 	 led_on(RED);
 //	 while(j--)
 //	 {

 		// led_on(RED);;
 		 flash_led(RED,50,0);

 		 Timer_Set_Delay(2000);	//500 ms
 		 while(Timer_Get_Status()!=TIME_OUT)
 			 ;

 		led_off(RED);

 		/// Timer_Set_Delay(500);  //500ms
 		/// while(Timer_Get_Status()!=TIME_OUT)
 		 			 ;


 //	 }


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



