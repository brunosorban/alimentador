/* 
 * File:      sensores.h
 * Author:    Bruno A. Sorban
 * Date:      12/06/2022
 * Comments:  Controle do sensor ultrassonico
 * Revision history:
 */

#ifndef SENSORES_H
#define	SENSORES_H

#include <Arduino.h>

//Define os pinos para o trigger e echo
#define PINO_TRIGGER 26
#define PINO_ECHO 25
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

int leituraUltrassonico();

void setupUltrassonico();

// class sensor_ultrassonico {
//     private:
//         int echo_pin;
//         int trigger_pin;
    
//     public:
//         sensor_ultrassonico(int echo_pin, int trigger_pin);
//         ~sensor_ultrassonico();
//         Ultrasonic* ultrasonic_sensor;
//         float get_ultrasonic();

// };

#endif	/* SENSORES_H */