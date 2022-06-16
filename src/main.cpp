#include <ESP32Servo.h>
#include <Arduino.h>
#include "leds.h"
#include "motor.h"
#include "sensor_ultrassonico.h"
#include "balanca.h"
#include <Ultrasonic.h>
#include "HX711.h"

#define VEL 255
#define SERVO_PIN 32

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
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(SERVO_PIN, 1000, 2000); // attaches the servo on pin 18 to the servo object
	// using default min/max of 1000us and 2000us
	// different servos may require different min/max settings
	// for an accurate 0 to 180 sweep

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