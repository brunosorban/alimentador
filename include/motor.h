/* 
 * File:      motor.h
 * Author:    Bruno A. Sorban
 * Date:      11/06/2022
 * Comments:  Controle do motor
 * Revision history:
 */

#ifndef MOTOR_H
#define	MOTOR_H

#include <Arduino.h>

#define MOTORPIN1 32
#define MOTORPIN2 33

void forward_motor(int speed);
void backward_motor(int speed);
void brake_motor();
void sweep_motor();

#endif	/* MOTOR_H */