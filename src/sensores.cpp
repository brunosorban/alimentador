/* 
 * File:      sensores.cpp
 * Author:    Bruno A. Sorban
 * Date:      12/06/2022
 * Comments:  Controle do sensor ultrassonico
 * Revision history:
 */

#ifndef SENSORES_C
#define	SENSORES_C

#include <Arduino.h>
#include "sensores.h"

Ultrasonic ultrasonic_sensor(pino_trigger, pino_echo);

float get_ultrasonic() {
    float distance = ultrasonic_sensor.read();
    float distanceCM = distance / 10; // obter formula de conversao
    return distanceCM;
}

#endif	/* SENSORES_C */