/*
 * cqueue.h
 *
 *  Created on: 01/08/2014
 *      Author: Administrador
 */

#ifndef CQUEUE_H_
#define CQUEUE_H_


void QueueInit(void);
unsigned char PushQueue(unsigned char data);
unsigned char PullQueue(void);
unsigned char QueueStatus(void);
#define QOVERFLOW 0xFF




#endif /* CQUEUE_H_ */
