/* 
 * File:      leds.cpp
 * Author:    Bruno A. Sorban
 * Date:      11/06/2022
 * Comments:  Controle dos LEDS do alimentador
 * Revision history:
 */

#include <Arduino.h>
#include "motor_vibra.h"

motor_vibra::motor_vibra(int pin)
{
    pino = pin;
    pinMode(pin, OUTPUT);
}

void motor_vibra::On() {
    digitalWrite(pino, HIGH);
}

void motor_vibra::Off() {
    digitalWrite(pino, LOW);
}
	/* LEDS_C */