/* 
 * File:      sensores.cpp
 * Author:    Bruno A. Sorban
 * Date:      12/06/2022
 * Comments:  Controle do sensor ultrassonico
 * Revision history:
 */

#include <Arduino.h>
#include "sensor_ultrassonico.h"

void setupUltrassonico() {
    pinMode(PINO_TRIGGER, OUTPUT); // Sets the trigPin as an Output
    pinMode(PINO_ECHO, INPUT); // Sets the echoPin as an Input
}


int leituraUltrassonico() {
    long duration;
    float distanceCm;

    // Clears the PINO_TRIGGER
    digitalWrite(PINO_TRIGGER, LOW);
    delayMicroseconds(2);
    // Sets the PINO_TRIGGER on HIGH state for 10 micro seconds
    digitalWrite(PINO_TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(PINO_TRIGGER, LOW);

    duration = pulseIn(PINO_ECHO, HIGH);
  
    // Calculate the distance
    distanceCm = duration * SOUND_SPEED/2;

    return distanceCm; 
}

// sensor_ultrassonico::sensor_ultrassonico(int echo_pin, int trigger_pin) 
// {
//     echo_pin = echo_pin;
//     trigger_pin = trigger_pin;
    
//     ultrasonic_sensor = new Ultrasonic(trigger_pin, echo_pin);
// }

// float sensor_ultrassonico::get_ultrasonic() 
// {
//     float distance = ultrasonic_sensor->read();
//     float distanceCM = distance / 10; // obter formula de conversao
//     return distanceCM;
// }

// sensor_ultrassonico::~sensor_ultrassonico() 
// {
//     delete ultrasonic_sensor;
// }
