#include <Arduino.h>
#include "leds.h"
#include "motor.h"
#include "sensores.h"
#include <Ultrasonic.h>

float dist_CM;

void setup() {
  // Configura pinos como output
  pinMode(BEEP, OUTPUT);
  pinMode(ONBOARD_LED, OUTPUT);
  pinMode(LEDVERMELHO, OUTPUT);
  pinMode(LEDAMARELO, OUTPUT);
  pinMode(LEDVERDE, OUTPUT);
  pinMode(MOTORPIN1, OUTPUT);
  pinMode(MOTORPIN2, OUTPUT);

  // Chama funções de teste
  blink_all(500);
  // sweep_motor();
  beep();

  // Inicializa Porta serial
  Serial.begin(9600);
  Serial.println("Lendo dados do sensor...");
}

void loop() {
  // obtem ultrassom
  dist_CM = get_ultrasonic();
  
  //Exibe informacoes no serial monitor
  Serial.print("\nDistancia em cm: ");
  Serial.print(dist_CM);
}