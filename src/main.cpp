#include <ESP32Servo.h>
#include <Arduino.h>
#include "leds.h"
#include "servo.h"
#include "sensor_ultrassonico.h"
#include "balanca.h"
#include "HX711.h"
#include "time.h"
#include "comunicacao_wifi.h"

#define DIST_TRESH 10 //distancia em cm que considera deteccao
#define SERVO_PIN 21

float dist_CM;
double peso;
int num_pesagens = 2;
int deposicao_realizada = 0;

// Cria os objetos LED eo Beep
leds led_vermelho(LEDVERMELHO);
leds led_amarelo(LEDAMARELO);
leds led_verde(LEDVERDE);
leds led_azul(ONBOARD_LED);
leds beep(BEEP);


// Cria objeto balança
balanca bal(DT_BALANCA, SCK_BALANCA);

// Cria objeto servomotor
servo servoMot(SERVO_PIN);

// Cria objeto horarios
horarios horariosUsuario;

int determinaEvento() {
  float dist_CM;
  double peso = 0;
  int num_pesagens = 2;
  int horario;

  //variaveis de verificacao de estado
  int sensor_detecta = false;
  int massa_atingida = false;
  int horario_atingido = false;

  //medicao dos sensores
  dist_CM = leituraUltrassonico();
  horario = getTimeSec();
  for(int i =0; i<num_pesagens; i++) {
    peso = peso + bal.measure()/num_pesagens;
  }

  //atribuicao de logica
  sensor_detecta = (dist_CM <= DIST_TRESH);
  massa_atingida = (peso >= horariosUsuario.massa1);
  horario_atingido = (horario >= horariosUsuario.horario1);

  Serial.printf("Distancia: %f, Peso: %f Horario: %d\n", dist_CM, peso, horario);
  Serial.printf("Deteccao: %d, Massa: %d, Horario: %d\n", sensor_detecta, massa_atingida, horario_atingido);

  return 0;

}


void setup() {

  Serial.begin(115200);

  //inicio da comunicacao wifi
  Serial.print("Conectando no WiFi: ");
  Serial.println(ssid);
  Serial.flush();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado.");

  //configuracao do horario
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Horário GMT-3 configurado.");

  //leitura inicial dos horarios programados
  horariosUsuario = readMassasHorarios();

  //setup do ultrassonico
  setupUltrassonico();

  //bip para indicar final do setup
  beep.beep();
}

void loop() {
  determinaEvento();
  servoMot.open();
  delay(1000);
  servoMot.close();
  delay(500);
  
}






// void setup() {
//   // Chama funções de teste
//   led_vermelho.blink(200);
//   led_amarelo.blink(200);
//   led_verde.blink(200);
//   led_azul.blink(200);
//   // motor_fuso.sweep_motor();
//   beep.beep();
//   // motor_fuso.partida(VEL);

// 	// // Allow allocation of all timers
// 	ESP32PWM::allocateTimer(0);
// 	ESP32PWM::allocateTimer(1);
// 	ESP32PWM::allocateTimer(2);
// 	ESP32PWM::allocateTimer(3);
// 	// for an accurate 0 to 180 sweep

//   // Inicializa Porta serial
//   Serial.begin(19200);

//   Serial.println();
//   Serial.print("Connecting to wifi: ");
//   Serial.println(ssid);
//   Serial.flush();
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("");
//   Serial.println("WiFi connected.");

//   configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
// }


// void loop() {
//   // // obtem ultrassom
//   dist_CM = sensor_u.get_ultrasonic();

//   peso = 0.0;
//   //calcula media de 10 leituras
//   for(int i = 0; i < num_pesagens; i++) {
//     peso = peso + bal.measure()/num_pesagens;
//   }

//   Serial.println(readMassasHorarios().massa3);

//   String sensor = (String)peso;
//   String tempo = "10";
//   Serial.println(sensor);
//   sendData(sensor, tempo);

//   int tempo_min = getTimeSec();
//   Serial.print("Minutos desde a meia noite: ");
//   Serial.print(tempo_min);
//   Serial.println();
  
//   // // Exibe informacoes no serial monitor
//   // Serial.print("\nDistancia em cm: ");
//   // Serial.print(dist_CM);
//   // Serial.print("\nPeso: ");
//   // Serial.print(peso);
//   // Serial.print("g");

//   servoMot.sweep();
//   delay(30000);

// }

