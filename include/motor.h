/* 
 * File:      motor.h
 * Author:    Bruno A. Sorban
 * Date:      11/06/2022
 * Comments:  Controle do motor
 * Revision history:
 */

#ifndef MOTOR_H
#define	MOTOR_H

#include <Arduino.h>

#define MOTORPIN1 32
#define MOTORPIN2 33

class motor {
    private:
        int in1;
        int in2;

    public:
        motor(int pin1, int pin2); // funcao construtura, chamada quando a classe é criada
        void forward_motor(int speed);
        void backward_motor(int speed);
        void brake_motor();
        void sweep_motor();
};

#endif	/* MOTOR_H */