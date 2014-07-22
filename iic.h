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


typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u16;
typedef signed int s16;

typedef struct {
    u8 count;
    u8 state;
    u8 address_hi;
    u8 address_lo;
    u8 *buffer;
    u8 mode;			// read / write

} i2c_data_t;



void i2c_trans(u8 size, u8 id, u8 buffer[], u16 address,u8 mode);

void i2c_init(void); // setup
u8 i2c_int(void); // data transfer interrupt
u8 i2c_eint(void); // NACK interrupt
void testi2c(void);
void WriteEE(u8 *pdata,u16 address);
void ReadEE(u8 *pdata,u16 address);




#endif /* IIC_H_ */
