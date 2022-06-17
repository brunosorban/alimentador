#include <ESP32Servo.h>
#include <Arduino.h>
#include "leds.h"
#include "servo.h"
#include "sensor_ultrassonico.h"
#include "balanca.h"
#include <Ultrasonic.h>
#include "HX711.h"
#include "Wifi.h"
#include <HTTPClient.h>

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

const char * ssid = "GUERREIRO_NET";
const char * password = "50825084";

// const char * ssid = "ARIEL S21FE";
// const char * password = "polipoli";
String google_script_leitura = "AKfycbw-SfekyBnNRGDhMqtjtv5f6iv7ovYjpLPN2ySiLwsMIL1W3SIv_8LyXqRGtzfUjUZNuA";
String google_script_escrita = "AKfycbw-7RdeFdl6DQRVcXVVRC9Cza6fPADGxR_KC1Itf4e46lv0er385I63jP74fcNU4iTw";


Servo myservo;
int pos = 0;

// Cria objeto sensor ultrassonico
sensor_ultrassonico sensor_u(PINO_TRIGGER, PINO_ECCHO);

// Cria objeto balança
balanca bal(DT_BALANCA, SCK_BALANCA);

//servo servoMot(SERVO_PIN);


void sendData(String dado, String time ) {
  HTTPClient http;
  String url="https://script.google.com/macros/s/"+google_script_escrita+"/exec?" + "date=" + time + "&sensor=" + dado;
  //Serial.print(url);
  Serial.print("Making a request");
  Serial.println();
  http.begin(url.c_str()); //Specify the URL and certificate
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET();  

  String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println(httpCode);    
    }
  http.end();
}



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
	// ESP32PWM::allocateTimer(0);
	// ESP32PWM::allocateTimer(1);
	// ESP32PWM::allocateTimer(2);
	// ESP32PWM::allocateTimer(3);
	// for an accurate 0 to 180 sweep
 // servoMot.sweep();

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
}


void loop() {
  // // obtem ultrassom
  dist_CM = sensor_u.get_ultrasonic();

  peso = 0.0;
  //calcula media de 10 leituras
  for(int i = 0; i < num_pesagens; i++) {
    peso = peso + bal.measure()/num_pesagens;
  }

  HTTPClient http;

  String url = "https://script.google.com/macros/s/" + google_script_leitura + "/exec?read";

  Serial.print("Começo de requisição\n");
  http.begin(url.c_str());
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET();
  String payload;
    if (httpCode > 0) { //Check for the returning code
        payload = http.getString();
        
        //Serial.println(httpCode);
        Serial.println(payload);
      }
    else {
      Serial.println("Error on HTTP request");
    }
	http.end();

  String sensor = (String)peso;
  String tempo = "10";
  Serial.println(sensor);
  sendData(sensor, tempo);


  
  // // Exibe informacoes no serial monitor
  // Serial.print("\nDistancia em cm: ");
  // Serial.print(dist_CM);
  // Serial.print("\nPeso: ");
  // Serial.print(peso);
  // Serial.print("g");

  //servoMot.sweep();
  delay(3000);

}

