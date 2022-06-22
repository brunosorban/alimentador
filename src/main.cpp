#include <Arduino.h>
#include "leds.h"
#include "servo_esp.h"
#include "sensor_ultrassonico.h"
#include <Ultrasonic.h>
#include <ESP32Servo.h>

float dist_CM;
int pos = 0;    // variable to store the servo position

// Cria os objetos LED
leds led_vermelho(LEDVERMELHO);
leds led_amarelo(LEDAMARELO);
leds led_verde(LEDVERDE);
leds led_azul(ONBOARD_LED);
leds beep(BEEP);

// Cria objeto SERVO
servo_esp servo_porta(SERVO_PIN);

// Cria sensor ultrassonico
sensor_ultrassonico sensor(PINO_TRIGGER, PINO_ECCHO);

void setup() {
  // Chama funções de teste
  led_vermelho.blink(500);
  led_amarelo.blink(500);
  led_verde.blink(500);
  led_azul.blink(500);
  beep.beep();

  servo_porta.open();
  delay(1000);
  servo_porta.close();
  delay(1000);

  // Inicializa Porta serial
  Serial.begin(9600);
}

void loop() {
  led_vermelho.blink(200);
  // obtem ultrassom
  dist_CM = sensor.get_ultrasonic();

  // Exibe informacoes no serial monitor
  Serial.print("\nDistancia em cm: ");
  Serial.print(dist_CM);
  // delay(1000);
}