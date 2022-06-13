/* 
 * File:      leds.h
 * Author:    Bruno A. Sorban
 * Date:      11/06/2022
 * Comments:  Controle dos LEDS do alimentador
 * Revision history:
 */

#ifndef LEDS_H
#define	LEDS_H

#include <Arduino.h>

#define ONBOARD_LED  2
#define LEDVERMELHO 12
#define LEDAMARELO 14
#define LEDVERDE 27
#define BEEP 13

class leds {
    private:
        int pino;

    public:
        leds(int pin); // funcao construtura, chamada quando a classe é criada
        void blink(int time_ms);
        void beep();
};

// void blink_all(int timeNMNN_ms);
// void beep();
#endif	/* LEDS_H */