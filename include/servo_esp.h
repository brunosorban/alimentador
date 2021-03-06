/*
 * File:      servo_esp.h
 * Author:    Bruno A. Sorban
 * Date:      21/06/2022
 * Comments:  Controle dos servo com esp32
 * Revision history:
 */

#ifndef SERVO_ESP_H
#define SERVO_ESP_H

#include <Arduino.h>
#include <ESP32Servo.h>

#define OPEN_POSITION 140
#define CLOSED_POSITION 180

class servo_esp
{
private:
    Servo myservo;

public:
    servo_esp(int pin); // funcao construtura, chamada quando a classe é criada
    void open();
    void close();
};

#endif /* SERVO_ESP_H */