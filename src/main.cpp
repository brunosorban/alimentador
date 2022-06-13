#include <Arduino.h>
#include "leds.h"
#include "motor.h"
#include "sensor_ultrassonico.h"
#include <Ultrasonic.h>

float dist_CM;

// Cria os objetos LED
leds led_vermelho(LEDVERMELHO);
leds led_amarelo(LEDAMARELO);
leds led_verde(LEDVERDE);
leds led_azul(ONBOARD_LED);

// Cria objeto motor
motor motor_fuso(MOTORPIN1, MOTORPIN2);

// Cria sensor ultrassonico
sensor_ultrassonico sensor(PINO_TRIGGER, PINO_ECCHO);

void setup() {
  // Chama funções de teste
  led_vermelho.blink(500);
  led_amarelo.blink(500);
  led_verde.blink(500);
  led_azul.blink(500);
  motor_fuso.sweep_motor();
  // beep();

  // Inicializa Porta serial
  Serial.begin(9600);
}

void loop() {
  led_vermelho.blink(500);
  // obtem ultrassom
  dist_CM = sensor.get_ultrasonic();
  
  // Exibe informacoes no serial monitor
  Serial.print("\nDistancia em cm: ");
  Serial.print(dist_CM);
  delay(1000);
}