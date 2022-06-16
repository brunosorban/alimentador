#include <ESP32Servo.h>
#include <Arduino.h>
#include "leds.h"
#include "servo.h"
#include "sensor_ultrassonico.h"
#include "balanca.h"
#include <Ultrasonic.h>
#include "HX711.h"

#define SERVO_PIN 21

float dist_CM;
double peso;
int num_pesagens = 2;

// Cria os objetos LED eo Beep
leds led_vermelho(LEDVERMELHO);
leds led_amarelo(LEDAMARELO);
leds led_verde(LEDVERDE);
leds led_azul(ONBOARD_LED);
leds beep(BEEP);

// Cria objeto motor
// motor motor_fuso(MOTORPIN1, MOTORPIN2);
Servo myservo;
int pos = 0;

// Cria objeto sensor ultrassonico
sensor_ultrassonico sensor_u(PINO_TRIGGER, PINO_ECCHO);

// Cria objeto balança
balanca bal(DT_BALANCA, SCK_BALANCA);

servo servoMot(SERVO_PIN);

void setup() {
  // Chama funções de teste
  led_vermelho.blink(200);
  led_amarelo.blink(200);
  led_verde.blink(200);
  led_azul.blink(200);
  // motor_fuso.sweep_motor();
  beep.beep();
  // motor_fuso.partida(VEL);

	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	// for an accurate 0 to 180 sweep
  servoMot.sweep();

  // Inicializa Porta serial
  Serial.begin(19200);
}

void loop() {
  // // obtem ultrassom
  dist_CM = sensor_u.get_ultrasonic();

  peso = 0.0;
  //calcula media de 10 leituras
  for(int i = 0; i < num_pesagens; i++) {
    peso = peso + bal.measure()/num_pesagens;
  }
  // motor_fuso.forward_motor(VEL);
  
  // // Exibe informacoes no serial monitor
  Serial.print("\nDistancia em cm: ");
  Serial.print(dist_CM);
  Serial.print("\nPeso: ");
  Serial.print(peso);
  Serial.print("g");

  servoMot.sweep();
  delay(100);
//   for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
//     // in steps of 1 degree
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(7);                       // waits 15ms for the servo to reach the position
//   }
//   for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(7);                       // waits 15ms for the servo to reach the position
//   }

}

