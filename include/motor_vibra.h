/*
 * File:      motor_vibra.h
 * Author:    Bruno A. Sorban
 * Date:      11/06/2022
 * Comments:  Controle domotor de vibração
 * Revision history:
 */

#ifndef MOTOR_VIBRA_H
#define MOTOR_VIBRA_H

#include <Arduino.h>

class motor_vibra
{
private:
    int pino;

public:
    motor_vibra(int pin); // funcao construtura, chamada quando a classe é criada
    void On();
    void Off();
};

#endif