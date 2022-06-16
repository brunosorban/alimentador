#include <Arduino.h>
#include "leds.h"
#include "servo.h"
#include "sensor_ultrassonico.h"
#include "balanca.h"
#include <Ultrasonic.h>
#include "HX711.h"

#define SERVO_PIN 32

float dist_CM;
double peso;

// Cria os objetos LED eo Beep
leds led_vermelho(LEDVERMELHO);
leds led_amarelo(LEDAMARELO);
leds led_verde(LEDVERDE);
leds led_azul(ONBOARD_LED);
leds beep(BEEP);

// Cria objeto motor
// motor motor_fuso(MOTORPIN1, MOTORPIN2);
int pos = 0;

// Cria objeto sensor ultrassonico
sensor_ultrassonico sensor_u(PINO_TRIGGER, PINO_ECCHO);

// Cria objeto balança
balanca bal(DT_BALANCA, SCK_BALANCA);

servo servoMot(SERVO_PIN);

void setup()
{
  // Chama funções de teste
  led_vermelho.blink(200);
  led_amarelo.blink(200);
  led_verde.blink(200);
  led_azul.blink(200);
  // motor_fuso.sweep_motor();
  beep.beep();
  // servo.sweep();
  servoMot.sweep();

  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  // Inicializa Porta serial
  Serial.begin(19200);
}

void loop()
{
  // // obtem ultrassom
  // dist_CM = sensor_u.get_ultrasonic();
  // peso = bal.measure();
  // motor_fuso.forward_motor(VEL);

  // // Exibe informacoes no serial monitor
  // Serial.print("\nDistancia em cm: ");
  // Serial.print(dist_CM);
  // Serial.print("\nPeso: ");
  // Serial.print(peso);
  // Serial.print("g");
  led_vermelho.blink(200);
}
