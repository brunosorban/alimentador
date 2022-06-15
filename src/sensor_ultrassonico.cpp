/*
 * File:      sensores.cpp
 * Author:    Bruno A. Sorban
 * Date:      12/06/2022
 * Comments:  Controle do sensor ultrassonico
 * Revision history:
 */

#ifndef SENSORES_C
#define SENSORES_C

#include <Arduino.h>
#include "sensor_ultrassonico.h"
#include <Ultrasonic.h>

sensor_ultrassonico::sensor_ultrassonico(int echo_pin, int trigger_pin)
{
    echo_pin = echo_pin;
    trigger_pin = trigger_pin;
    ultrasonic_sensor = new Ultrasonic(trigger_pin, echo_pin);
}

float sensor_ultrassonico::get_ultrasonic()
{
    float distance = ultrasonic_sensor->read();
    float distanceCM = distance / 10; // obter formula de conversao
    return distanceCM;
}

sensor_ultrassonico::~sensor_ultrassonico()
{
    delete ultrasonic_sensor;
}

#endif /* SENSORES_C */