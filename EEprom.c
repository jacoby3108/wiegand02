/*
 * EEprom.c
 *
 *  Created on: 31/07/2014
 *      Author: Administrador
 */


#include <msp430.h>
#include "hardware.h"
#include "iic.h"
#include "timer.h"



//====================EE SERVICES========================================

void WriteEE(u8 *pdata,u16 address)
{

	i2c_trans(1, 0xA0, pdata, address,WRITE_M); //i2c TX 1 byte
    Timer_Set_Delay(10);						//set write EE time
	while(Timer_Get_Status()!=TIME_OUT);		//Wait Write Time

}

void ReadEE(u8 *pdata,u16 address)
{

	i2c_trans(1, 0xA0, pdata, address,READ_M);  // i2c RX 1 byte
	Timer_Set_Delay(10);						//set write EE time **
	while(Timer_Get_Status()!=TIME_OUT);		//Wait Write Time **
//	while (!i2c_ready());					// wait for read operation complete (aca hay lio)
}


