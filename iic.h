/*
 * iic.h
 *
 *  Created on: 14/07/2014
 *      Author: Dany
 *
 *    Adapted from http://forum.43oh.com/topic/1772-universal-ripple-control-receiver/#p18191
 *
 *
 */

#ifndef IIC_H_
#define IIC_H_



typedef struct {
    u8 count;
    u8 state;
    u8 address_hi;
    u8 address_lo;
    u8 *buffer;
    u8 mode;			// read / write

} i2c_data_t;


// Exported services
void i2c_trans(u8 size, u8 id, u8 buffer[], u16 address,u8 mode); // IIC service for Reading or writing i2c device
void i2c_init(void); // setup i2c hardware
u8 i2c_ready(void ); // Status of i2c for read transfers

// internal functions
static u8 i2c_int(void); // data transfer interrupt
static u8 i2c_eint(void); // NACK interrupt
static void testi2c(void);


//Exported constants
#define READ_M		0		// Read EEprom memory
#define WRITE_M		1		// Write EEprom memory



#endif /* IIC_H_ */
