#include "msp430g2553.h"
#include "hardware.h"
#include "led.h"



static u8 toggle=FALSE;
static UINT flash_counter=0;
static u8 led_mask;
static UINT flash_interval;
static u8 flash_pulses;
static u8 flash_mode=NORMAL;



void init_led(void)
{

//****************Measurement of SMCLK********************************************************
// if SHOW_SMCLK is enabled (=1) smclk signal is visible on P1.4 useful to measure system clk
// (WARNING !! if this test is enabled only scope should be connected to  P1.4  )
#define SHOW_SMCLK 0		// =1 Show System bus clk / =0 Normal I/O operation
//*****************************************************************************************

#if SHOW_SMCLK     // show SMCLK on P1.4  (WARNING !! only scope should be connected to  P1.4  )

    P1SEL|=IO_FUNCTION | P1_4 ;  // select SMCLK as primary function
    P1SEL2|=IO_FUNCTION;
    P1DIR |= P1_4;  // P1.4 as output "required"
    for(;;){}  // Endless loop

#else

    P1OUT &= ~(GREEN | RED);
    P1SEL|=IO_FUNCTION;				// Primary function as I/O port
    P1SEL2|=IO_FUNCTION;
    P1DIR |= (GREEN | RED);			// set port direction
#endif

    toggle=FALSE;
    flash_mode=NORMAL;



}

void led_on(u8 ledcolor)
{
	 toggle=FALSE;
	 P1OUT |= ledcolor;

}

void led_off(u8 ledcolor)
{

	 toggle=FALSE;
	 P1OUT &= ~ledcolor;

}

/*=========================================================================================================
 * flash_led:
 * Toggles led at interval rate (50%)
 * ledcolor: is RED or GREEN or both
 * interval: is the rate in mseg
 * count: if 0 led stays flashing until led_on or led_off  is called
 * 		   if <> 0 the led flashes count times.
 *
 *
 * Sample code:
 *
 *1 flash_led(RED,500,8);		// flashes red led 8 times at 500ms rate
 *
 *2 flash_led(RED,500);			// red led keeps flashing at 500ms rate until led_on or led_off is called
 *
 *
 *========================================================================================================
 **/


void flash_led(u8 ledcolor,UINT interval, u8 count )
{
	if(count){

	flash_mode = COUNTED;			// Flashes only "count" times
	flash_pulses=(count-1)*2;
	}

	else

	flash_mode = NORMAL;        // Keep flashing all time



	flash_interval=interval/10;  	// flash rate
	flash_counter=flash_interval;   // initialize rate counter
	led_mask=ledcolor;				// set proper mask
	toggle=TRUE;					// Enable Toggle mode

}
/*=============================================
 * isr_leds() is feed by a 10ms clock source
 *
 * It provides timing for led timed functions
 *
 *
 *============================================*/

void isr_led(void)
{
	if (toggle==TRUE)				// if Toggle mode
	{
		if(flash_counter)			// update rate counter
			flash_counter--;
		else						// once the counter reached 0
		{

			P1OUT ^= led_mask;		// Toggle led
			flash_counter=flash_interval;				// restore counter

			if(flash_mode == COUNTED )					// if mode is Counted
			{
				if(flash_pulses)						// count down until no more pulses (flashes) are left
					flash_pulses--;
				else
				{
					P1OUT &= ~led_mask;					// Ensure led remains off when flashing stops
					toggle=FALSE;						// No more flashes pending  disable toggle mode
				}
			}
		}
	}
}




