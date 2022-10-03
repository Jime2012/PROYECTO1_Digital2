/* 
 * File:   TIMER0.h
 * Author: Compu Fire
 *
 * Created on 22 de julio de 2022, 7:36
 */

#ifndef TIMER0_H
#define	TIMER0_H

#include <xc.h>
#define _XTAL_FREQ 1000000
void tmr0_init (uint8_t prescaler);
void tmr0_reload (void);

#endif	/* OSCILADOR_H */


