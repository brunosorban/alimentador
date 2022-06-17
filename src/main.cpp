#include <ESP32Servo.h>
#include <Arduino.h>
#include "leds.h"
#include "servo.h"
#include "sensor_ultrassonico.h"
#include "balanca.h"
#include <Ultrasonic.h>
#include "HX711.h"
// #include "Wifi.h"
// #include <HTTPClient.h>
#include "time.h"
#include "comunicacao_wifi.h"

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

	// // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	// for an accurate 0 to 180 sweep

  // Inicializa Porta serial
  Serial.begin(19200);

  Serial.println();
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  Serial.flush();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}


void loop() {
  // // obtem ultrassom
  dist_CM = sensor_u.get_ultrasonic();

  peso = 0.0;
  //calcula media de 10 leituras
  for(int i = 0; i < num_pesagens; i++) {
    peso = peso + bal.measure()/num_pesagens;
  }


  Serial.println(readData());

  String sensor = (String)peso;
  String tempo = "10";
  Serial.println(sensor);
  sendData(sensor, tempo);

  int tempo_min = getTimeSec();
  Serial.print("Minutos desde a meia noite: ");
  Serial.print(tempo_min);
  Serial.println();
  
  // // Exibe informacoes no serial monitor
  // Serial.print("\nDistancia em cm: ");
  // Serial.print(dist_CM);
  // Serial.print("\nPeso: ");
  // Serial.print(peso);
  // Serial.print("g");

  servoMot.sweep();
  delay(10000);

}

