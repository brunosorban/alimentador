/* 
 * File:      servo_esp.cpp
 * Author:    Bruno A. Sorban
 * Date:      21/06/2022
 * Comments:  Controle dos servo com esp32
 * Revision history:
 */


#include <Arduino.h>
#include "servo_esp.h"
#include <ESP32Servo.h>

servo_esp::servo_esp(int pin)
{
    myservo.attach(pin);
}

void servo_esp::open() {
    myservo.write(OPEN_POSITION);
    vTaskDelay(100/portTICK_RATE_MS);
}

void servo_esp::close() {
    myservo.write(CLOSED_POSITION);
    vTaskDelay(100/portTICK_RATE_MS);
}