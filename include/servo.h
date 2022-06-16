/*
 * File:      servo.h
 * Author:    Thiago
 * Date:      16/06/2022
 * Comments:  Controle do Servo motor
 * Revision history:
 */

#ifndef SERVO_H
#define SERVO_H

#include <Arduino.h>
#include <ESP32Servo.h>

class servo
{
private:
    Servo *myservo;
    int vel = 255;
    int pos = 0;

public:
    servo(int pin);
    ~servo();

    void open();
    void close();
    void sweep();
    void set_speed(int desired_speed);
};

#endif /* SERVO_H */