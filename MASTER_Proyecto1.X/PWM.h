#ifndef PWM_H
#define	PWM_H

#include <xc.h>
#include <stdint.h>

void pwm_init (uint8_t channel);
/*
La variable "channel" corresponde al canal de donde se requiera el PWM, donde se
tiene que:
channel == 1 -> CCP1
channel == 2 -> CCP2
 */

void pwm_duty_cycle (uint16_t duty_cycle);
/*
La variable "duty_cycle" corresponde al valor del ciclo de trabajo que se
necesite de la se�al PWM.
 */
#endif	/* PWM_H */


