/*
 * File:      sensores.h
 * Author:    Bruno A. Sorban
 * Date:      12/06/2022
 * Comments:  Controle do sensor ultrassonico
 * Revision history:
 */

#ifndef SENSORES_H
#define SENSORES_H

#include <Arduino.h>
#include <Ultrasonic.h>

// Define os pinos para o trigger e echo
#define PINO_TRIGGER 25
#define PINO_ECCHO 26

class sensor_ultrassonico
{
private:
        int echo_pin;
        int trigger_pin;

public:
        sensor_ultrassonico(int echo_pin, int trigger_pin);
        ~sensor_ultrassonico();
        Ultrasonic *ultrasonic_sensor;
        float get_ultrasonic();
};

#endif /* SENSORES_H */