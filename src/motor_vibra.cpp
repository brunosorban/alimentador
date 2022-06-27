/*
 * File:      motor_vibra.cpp
 * Author:    Bruno A. Sorban
 * Date:      11/06/2022
 * Comments:  Controle do motor de vibração
 * Revision history:
 */

#include <Arduino.h>
#include "motor_vibra.h"

motor_vibra::motor_vibra(int pin)
{
    pino = pin;
    pinMode(pin, OUTPUT);
}

void motor_vibra::On()
{
    digitalWrite(pino, HIGH);
}

void motor_vibra::Off()
{
    digitalWrite(pino, LOW);
}