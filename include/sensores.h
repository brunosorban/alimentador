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
#include <Ultrasonic.h>

//Define os pinos para o trigger e echo
#define pino_trigger 25
#define pino_echo 26

float get_ultrasonic();

#endif	/* SENSORES_H */