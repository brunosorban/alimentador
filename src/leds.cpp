/* 
 * File:      leds.cpp
 * Author:    Bruno A. Sorban
 * Date:      11/06/2022
 * Comments:  Controle dos LEDS do alimentador
 * Revision history:
 */

#ifndef LEDS_C
#define	LEDS_C

#include <Arduino.h>
#include "leds.h"

leds::leds(int pin)
{
  pino = pin;
  pinMode(pino, OUTPUT);
}

void leds::blink(int time_ms) {
    digitalWrite(pino, HIGH);
    delay(time_ms);
    digitalWrite(pino, LOW);
    delay(time_ms);
}

void beep() {
  digitalWrite(BEEP, HIGH);
  delay(100);
  digitalWrite(BEEP, LOW);
  delay(100);
  digitalWrite(BEEP, HIGH);
  delay(100);
  digitalWrite(BEEP, LOW);
  }

#endif	/* LEDS_C */