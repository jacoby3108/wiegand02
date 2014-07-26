/*
 * iic.c
 *
 *  Created on: 14/07/2014
 *      Author: Dany
 *
 *      Adapted from http://forum.43oh.com/topic/1772-universal-ripple-control-receiver/#p18191
 *
 *
 */


#include <msp430.h>
#include "hardware.h"
#include "iic.h"
#include "timer.h"

// i2c states
#define WRITE		0x00
#define READ		0x01
#define ADDRHI		0x02
#define ADDRLO		0x03
#define SND_DATA 	0x04
#define REPSTT		0x08


#define RX_DATA		0x09
#define RX_CMD		0x0A
#define DONE		0xFF


#define READ_M		0
#define WRITE_M		1


i2c_data_t i2c_data = {0, 0, 0, 0};
u8 rcvdata;
u8 ready=0;

u8 mode=READ_M;

void i2c_init(void) {
    P1SEL |= BIT6 + BIT7; //Set I2C pins
    P1SEL2 |= BIT6 + BIT7;
    UCB0CTL1 |= UCSWRST; //Enable SW reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC; //I2C Master, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST; //Use SMCLK, keep SW reset
    UCB0BR0 = 12; //fSCL = SMCLK/12 = ~100kHz
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST; //Clear SW reset, resume operation

}

void i2c_trans(u8 size, u8 id, u8 *buffer, u16 address, u8 mode) {

    i2c_data.count =  size; // byte counter
    i2c_data.address_hi = (address>>8)& 0xFF; // Address Hi
    i2c_data.address_lo = address& 0xFF; // Address Hi
    i2c_data.buffer = buffer; // byte counter
    i2c_data.state = ADDRHI;
    i2c_data.mode = mode;
    UCB0I2CSA = id >> 1; // Slave Address 01101000 0x68 RTC //Slave Address 01010000 0x50 EEPROM (slave address is 7 bits 1010 A2 A1 A0
    UCB0CTL1 |= UCTR + UCTXSTT; // I2C TX, start condition
    IE2 |= UCB0TXIE | UCB0RXIE; //Enable TX, RX interrupt
    UCB0I2CIE = UCNACKIE; // NACK interrupts
    ready=0;
}


//======================= I2C ISR's ====================================================

// Tx vector

u8 i2c_int(void) {

	if (i2c_data.mode == WRITE_M)			// Write mode state machine
	{
		if (i2c_data.state == ADDRHI)				// Time to send MSB of EEprom address?
		{

			UCB0TXBUF=i2c_data.address_hi;
			i2c_data.state=ADDRLO;
		}

		else if (i2c_data.state == ADDRLO)         // Time to send LSB of EEprom address?
		{

			UCB0TXBUF=i2c_data.address_lo;
			i2c_data.state=SND_DATA;
		}
		else if (i2c_data.state == SND_DATA)		// Now send all data
		{

        	UCB0TXBUF=*(i2c_data.buffer);
        	i2c_data.state=DONE;
		}
		else if(i2c_data.state == DONE)		// End of frame
		{
    			UCB0CTL1 |= UCTXSTP; 		//I2C stop condition
                while (UCB0CTL1 & UCTXSTP); //Ensure stop condition got sent
                IFG2 &= ~UCB0TXIFG; 		//Clear USCI_B0 TX int flag
                return 1; 					//Exit LPM0
		}

		return 0; /// conitune sending the frame
	}

	if (i2c_data.mode == READ_M)				//Read mode state machine
	{
		if (i2c_data.state == ADDRHI)				// Time to send MSB of EEprom address?
		{

			UCB0TXBUF=i2c_data.address_hi;
			i2c_data.state=ADDRLO;
		}

		else if (i2c_data.state == ADDRLO)         // Time to send LSB of EEprom address?
		{

			UCB0TXBUF=i2c_data.address_lo;
			i2c_data.state=RX_CMD;
		}
		else if (i2c_data.state == RX_CMD)		// Send RD command
		{

//*****		P1OUT = ((RED) | PULLUPS);  // for debug
			UCB0CTL1 &= ~UCTR; 			// I2C RX
			UCB0CTL1 |= UCTXSTT; 		// I2C repeated Start condition for RX
			i2c_data.state=RX_DATA;

		}

		else if (i2c_data.state == RX_DATA)		// Last repeated start TX Interrupt
		{

    		// repeated start was send
    	    // now incoming data is being received;

           	 i2c_data.state=DONE;

		}


		else if(i2c_data.state == DONE)				// End of frame

		{


			 	UCB0CTL1 |= UCTXSTP ; 				//I2C stop condition  ( In master rx mode the stop is always preceded by a NACK)
    	         while (UCB0CTL1 & UCTXSTP);	 	//Ensure stop condition got sent before reading the data
                IFG2 &= ~UCB0TXIFG; 				//Clear USCI_B0 TX int flag
//******        P1OUT = ((RED ^ RED) | PULLUPS);  	// for debug
                *(i2c_data.buffer)= UCB0RXBUF;   	// Read incoming data
                ready=1;						 	// Signal that data is available
                return 1; 							//Exit LPM0
		}

		return 0; /// conitune sending the frame
	}

}

// Rx vector ??

u8 i2c_eint(void) {
    if (UCB0STAT & UCNACKIFG) { // send STOP if slave sends NACK
        UCB0CTL1 |= UCTXSTP;
        UCB0STAT &= ~UCNACKIFG;
        return 1;
    }
    return 0;
}

// I2C interrupt vectors

// I2C data transfer vector
#pragma vector = USCIAB0TX_VECTOR

__interrupt void USCIAB0TX_ISR(void) {

	i2c_int();

//	if (i2c_int()) __bic_SR_register_on_exit(CPUOFF); //Exit LPM0;
}

// I2C status vector
#pragma vector = USCIAB0RX_VECTOR

__interrupt void USCIAB0RX_ISR(void) {
    if (i2c_eint()) {
        while (!(IFG2 & UCA0TXIFG)); // send error via UART
        UCA0TXBUF = '#';
    }
}


// ======================================================================================

/*===================================
 *
 * TEST CODE for iic
 *
 *==================================
 */
u8 txdata[] = {'H', 'E', 'L', 'L', 'O', ' ', 'W', 'O', 'R', 'L', 'D'};
u8 rxdata[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int i;
void testi2c(void) {


//   __delay_cycles(20000); //Just a start up delay

	for (i=0;i<=4;i++)
	WriteEE(&txdata[i],0x1234+i);

	for (i=0;i<=4;i++)
    ReadEE(&rxdata[i],0x1234+i);

    i=0;

}



//====================EE SERVICES========================================

void WriteEE(u8 *pdata,u16 address)
{

	i2c_trans(1, 0xA0, pdata, address,WRITE_M); //i2c TX 1 byte
    Timer_Set_Delay(10);						//set write EE time
	while(Timer_Get_Status()!=TIME_OUT);		//Wait Write Time

}

void ReadEE(u8 *pdata,u16 address)
{

	i2c_trans(1, 0xA0, pdata, address,READ_M); //i2c RX 1 byte

}







