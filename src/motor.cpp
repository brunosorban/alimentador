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

motor::motor(int pin1, int pin2) {
    in1 = pin1;
    in2 = pin2;
}
void motor::forward_motor(int speed) {
    analogWrite(in1, speed);
    analogWrite(in2, 0);
}

void motor::backward_motor(int speed) {
    analogWrite(in1, 0);
    delay(100);
    analogWrite(in2, speed);
}

void motor::brake_motor() {
    analogWrite(in1, 255);
    analogWrite(in2, 255);
    delay(500);
    analogWrite(in1, 0);
    analogWrite(in2, 0);
    delay(500);
}

void motor::sweep_motor() {
    int speed;

    for (speed = 0; speed<= 255; speed+= 1) { // goes from 0 degrees to 180 degrees
        analogWrite(in1, speed);
        analogWrite(in2, 0);
        delay(10);                       // waits 15 ms for the servo to reach the position
    }
    for (speed= 255; speed>= 0; speed-= 1) { // goes from 180 degrees to 0 degrees
        analogWrite(in1, speed);
        analogWrite(in2, 0);
        delay(10);                       // waits 15 ms for the servo to reach the position
    }
    for (speed= 0; speed<= 255; speed+= 1) { // goes from 180 degrees to 0 degrees
        analogWrite(in1, 0);
        analogWrite(in2, speed);
        delay(10);                       // waits 15 ms for the servo to reach the position
    }
    for (speed= 255; speed>= 0; speed-= 1) { // goes from 180 degrees to 0 degrees
        analogWrite(in1, 0);
        analogWrite(in2, speed);
        delay(10);                       // waits 15 ms for the servo to reach the position
    }
}

void motor::partida(int desired_speed) {
    int speed;
        for (speed = 0; speed<= desired_speed; speed+= 1) { // goes from 0 degrees to 180 degrees
        analogWrite(in1, speed);
        analogWrite(in2, 0);
        delay(10);                       // waits 15 ms for the servo to reach the position
    }
}
#endif	/* MOTOR_C */