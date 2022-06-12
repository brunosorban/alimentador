/* 
 * File:      leds.c
 * Author:    Bruno A. Sorban
 * Date:      11/06/2022
 * Comments:  Controle dos LEDS do alimentador
 * Revision history:
 */

#ifndef LEDS_C
#define	LEDS_C

#include <Arduino.h>
#include "leds.h"

void blink_all(int time_ms) {
    digitalWrite(ONBOARD_LED, HIGH);
    digitalWrite(LEDVERMELHO, HIGH);
    digitalWrite(LEDAMARELO, HIGH);
    digitalWrite(LEDVERDE, HIGH);
    delay(time_ms);
    digitalWrite(ONBOARD_LED, LOW);
    digitalWrite(LEDVERMELHO, LOW);
    digitalWrite(LEDAMARELO, LOW);
    digitalWrite(LEDVERDE, LOW);
    delay(time_ms);
}

void beep() {
  digitalWrite(BEEP, HIGH);
  delay(200);
  digitalWrite(BEEP, LOW);
  delay(200);
  digitalWrite(BEEP, HIGH);
  delay(200);
  digitalWrite(BEEP, LOW);
  }

#endif	/* LEDS_C */