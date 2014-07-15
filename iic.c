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


#include "msp430g2553.h"
#include "iic.h"

// i2c states
#define WRITE	0x00
#define READ	0x01
#define ADDR16	0x02
#define ADDRTR	0x04
#define REPSTT	0x08

i2c_data_t i2c_data = {0, 0, 0, 0};

void i2c_init(void) {
    P1SEL |= BIT6 + BIT7; //Set I2C pins
    P1SEL2 |= BIT6 + BIT7;
    UCB0CTL1 |= UCSWRST; //Enable SW reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC; //I2C Master, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST; //Use SMCLK, keep SW reset
    UCB0BR0 = 12; //fSCL = SMCLK/12 = ~100kHz
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST; //Clear SW reset, resume operation
    IE2 |= UCB0TXIE | UCB0RXIE; //Enable TX, RX interrupt
    UCB0I2CIE = UCNACKIE; // NACK interrups
}

void i2c_trans(u8 size, u8 id, u8 *buffer, u16 address) {
    i2c_data.state = (id & READ) | ADDRTR; // byte counter
    if (size & 0x01) i2c_data.state |= ADDR16;
    i2c_data.count = (size >> 1) + 1; // byte counter
    i2c_data.address = address; // byte counter
    i2c_data.buffer = buffer; // byte counter
    UCB0I2CSA = id >> 1; // Slave Address 01101000 0x68 RTC //Slave Address 01010000 0x50 EEPROM (slave address is 7 bits 1010 A2 A1 A0
    UCB0CTL1 |= UCTR + UCTXSTT; // I2C TX, start condition
    __bis_SR_register(CPUOFF + GIE); // Enter LPM0 w/ interrupts
    // Remain in LPM0 until all data is transferred
}

u8 i2c_int(void) {
    if (i2c_data.state == WRITE) {
        if (i2c_data.count > 0) { //Check TX byte counter
            UCB0TXBUF = *i2c_data.buffer++; // Load TX buffer
            i2c_data.count--; //Decrement TX byte counter
        } else if (i2c_data.count == 0) { //last byte transferred
            UCB0CTL1 |= UCTXSTP; //I2C stop condition
            while (UCB0CTL1 & UCTXSTP); //Ensure stop condition got sent
            IFG2 &= ~UCB0TXIFG; //Clear USCI_B0 TX int flag
            return 1; //Exit LPM0
        }
    } else if (i2c_data.state == READ) {
        *i2c_data.buffer++ = UCB0RXBUF;
        i2c_data.count--; //Decrement RX byte counter
        if (i2c_data.count == 1) { //Check RX byte counter, 1 byte remaining
            UCB0CTL1 |= UCTXSTP; // I2C stop condition
        }
        if (i2c_data.count == 0) { //Check RX byte counter, last byte received
            while (UCB0CTL1 & UCTXSTP); // Ensure stop condition got sent
            return 1; // Exit LPM0
        }
    } else if (i2c_data.state & ADDR16) { // high byte address transmit
        UCB0TXBUF = _swap_bytes(i2c_data.address);
        i2c_data.state &= ~ADDR16;
    } else if (i2c_data.state & ADDRTR) { // low byte address transmit
        UCB0TXBUF = i2c_data.address;
        i2c_data.state &= ~ADDRTR;
        if (i2c_data.state) { // repeated Start for RX
            i2c_data.state |= REPSTT;
        }
    } else if (i2c_data.state & REPSTT) { //  repeated start required
        i2c_data.state &= ~REPSTT;
        UCB0CTL1 &= ~UCTR; // I2C RX
        UCB0CTL1 |= UCTXSTT; // I2C repeated Start condition for RX
        IFG2 &= ~UCB0TXIFG; //Clear USCI_B0 TX int flag
    }
    return 0;
}

u8 i2c_eint(void) {
    if (UCB0STAT & UCNACKIFG) { // send STOP if slave sends NACK
        UCB0CTL1 |= UCTXSTP;
        UCB0STAT &= ~UCNACKIFG;
        return 1;
    }
    return 0;
}


/*
 *
 * TEST CODE for iic and sci
 *
 *
 *
 */
u8 txdata[] = {'H', 'E', 'L', 'L', 'O', ' ', 'W', 'O', 'R', 'L', 'D'};
u8 rxdata[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int i;
void testi2c(void) {

/*
	WDTCTL = WDTPW + WDTHOLD; //Stop WDT

    BCSCTL1 = CALBC1_1MHZ; //Set DCO to 1MHz
    DCOCTL = CALDCO_1MHZ;

    // UART setup
    P1SEL = BIT1 + BIT2; //Set RXD and TXD
    P1SEL2 = BIT1 + BIT2;
    UCA0CTL1 |= UCSSEL_2 | UCSWRST; //Have USCI use SMCLK AKA 1MHz main CLK
    UCA0BR0 = 104; //Baud = 9600
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_1; //Modulation
    UCA0CTL1 &= ~UCSWRST; //Start USCI
*/
    i2c_init(); // init I2C
    __delay_cycles(20000); //Just a start up delay

 //   i2c_rx(11, 0xA0, rxdata, 0, 0); //i2c RX data

 /*   u8 i = 0;
    while (i < 11) {
        while (!(IFG2 & UCA0TXIFG));
        UCA0TXBUF = rxdata[i]; //UART TX data
        i++;
    }

*/
    i2c_tx(11, 0xA0, txdata, 0, 0x99); //i2c TX 11 bytes("HELLO WORLD")
    __delay_cycles(20000); //Allow EEPROM to write data

    i2c_rx(11, 0xA0, rxdata, 0, 0x99); //i2c RX data

    i=0;


/*   i = 0;
    while (i < 11) {
        while (!(IFG2 & UCA0TXIFG));
        UCA0TXBUF = rxdata[i]; //UART TX data
        i++;
    }
    __bis_SR_register(CPUOFF + GIE); //Wait for a reset

*/
}

// I2C data transfer vector
#pragma vector = USCIAB0TX_VECTOR

__interrupt void USCIAB0TX_ISR(void) {
    if (i2c_int()) __bic_SR_register_on_exit(CPUOFF); //Exit LPM0;
}

// I2C status vector
#pragma vector = USCIAB0RX_VECTOR

__interrupt void USCIAB0RX_ISR(void) {
    if (i2c_eint()) {
        while (!(IFG2 & UCA0TXIFG)); // send error via UART
        UCA0TXBUF = '#';
    }
}

