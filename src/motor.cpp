/* 
 * File:      motor.cpp
 * Author:    Bruno A. Sorban
 * Date:      11/06/2022
 * Comments:  Controle do motor
 * Revision history:
 */

#ifndef MOTOR_C
#define	MOTOR_C

#include <Arduino.h>
#include "motor.h"
#include "leds.h"

void forward_motor(int speed) {
    analogWrite(MOTORPIN1, speed);
    analogWrite(MOTORPIN2, 0);
}

void backward_motor(int speed) {
    analogWrite(MOTORPIN1, 0);
    delay(100);
    analogWrite(MOTORPIN2, 255);
}

void brake_motor() {
    analogWrite(MOTORPIN1, 255);
    analogWrite(MOTORPIN2, 255);
    analogWrite(LEDVERDE, 255);
    delay(500);
    analogWrite(MOTORPIN1, 0);
    analogWrite(MOTORPIN2, 0);
    analogWrite(LEDVERDE, 0);
    delay(500);
}

void sweep_motor() {
    int speed;

    for (speed = 0; speed<= 255; speed+= 1) { // goes from 0 degrees to 180 degrees
        analogWrite(MOTORPIN1, speed);
        digitalWrite(MOTORPIN2, LOW);
        delay(15);                       // waits 15 ms for the servo to reach the position
    }
    for (speed= 255; speed>= 0; speed-= 1) { // goes from 180 degrees to 0 degrees
        analogWrite(MOTORPIN1, speed);
        digitalWrite(MOTORPIN2, LOW);
        delay(15);                       // waits 15 ms for the servo to reach the position
    }
    for (speed= 0; speed<= 255; speed+= 1) { // goes from 180 degrees to 0 degrees
        digitalWrite(MOTORPIN1, LOW);
        analogWrite(MOTORPIN2, speed);
        delay(15);                       // waits 15 ms for the servo to reach the position
    }
    for (speed= 255; speed>= 0; speed-= 1) { // goes from 180 degrees to 0 degrees
        digitalWrite(MOTORPIN1, LOW);
        analogWrite(MOTORPIN2, speed);
        delay(15);                       // waits 15 ms for the servo to reach the position
    }
}

#endif	/* MOTOR_C */