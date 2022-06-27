/*
 * File:      sensor_ultrassonico.h
 * Author:    Bruno A. Sorban
 * Date:      12/06/2022
 * Comments:  Controle do sensor ultrassonico
 * Revision history:
 */

#ifndef SENSORES_H
#define SENSORES_H

#include <Arduino.h>

class sensor_ultrassonico
{
private:
        const float SOUND_SPEED = 0.034;
        const float CM_TO_INCHES = 0.393701;

        int trg; // Pino Trigger
        int ech; // Pino Echo

public:
        float get_ultrasonic();
        sensor_ultrassonico(int echo_pin, int trigger_pin);
};

#endif /* SENSORES_H */