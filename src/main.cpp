#include <Arduino.h>
#include "leds.h"
#include "motor.h"

void setup() {
  pinMode(BEEP, OUTPUT);
  pinMode(ONBOARD_LED, OUTPUT);
  pinMode(LEDVERMELHO, OUTPUT);
  pinMode(LEDAMARELO, OUTPUT);
  pinMode(LEDVERDE, OUTPUT);
  pinMode(MOTORPIN1, OUTPUT);
  pinMode(MOTORPIN2, OUTPUT);

  blink_all(500);
  beep();
}

void loop() {

}