#include <ESP32Servo.h>
#include <Arduino.h>
#include "leds.h"
#include "servo.h"
#include "sensor_ultrassonico.h"
#include "balanca.h"
#include "HX711.h"
#include "time.h"
#include "comunicacao_wifi.h"
#include "maquina.h"

/**********************************************
                   Definicoes
**********************************************/
#define DIST_TRESH 10 //distancia em cm que considera deteccao
#define SERVO_PIN 21


/**********************************************
              Criacao dos objetos
**********************************************/
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



/**********************************************
             Variaveis Globais
**********************************************/
float dist_CM;
double peso;
int num_pesagens = 2;
horarios horariosUsuario;
int horario_desejado;

//variaveis globais usadas na maquina de estados
double massa_atual;
double massa_desejada;
double massa_necessaria;

int deposicao_realizada; //determina que a deposicao foi realizada
int horario_atual; //guarda o horario atual (minutos desde as 00h)

int acao_matrizTransicao[NUM_ESTADOS][NUM_EVENTOS];
int proximoestado_matrizTransicao[NUM_ESTADOS][NUM_EVENTOS];
int estado;
int codigoAcao = NENHUMA_ACAO;
int codigoEvento = NENHUM_EVENTO;


/**********************************************
        Funcoes da Maquina de Estados
**********************************************/

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


void iniciaMaquinaEstados() {
  int i;
  int j;

  for(i = 0; i < NUM_ESTADOS; i++) {
    for(j = 0; j< NUM_EVENTOS; j++) {
      acao_matrizTransicao[i][j] = NENHUMA_ACAO;
      proximoestado_matrizTransicao[i][j] = i;
    }
  }

  //setup do idle
  proximoestado_matrizTransicao[IDLE][DETECTAR] = DETECCAO;
  acao_matrizTransicao[IDLE][DETECTAR] = A01;

  proximoestado_matrizTransicao[IDLE][ACIONAR] = DEPOSICAO;
  acao_matrizTransicao[IDLE][ACIONAR] = A02;

  proximoestado_matrizTransicao[IDLE][ATUALIZAR] = IDLE;
  acao_matrizTransicao[IDLE][ATUALIZAR] = A07;


  //setup da deposicao
  proximoestado_matrizTransicao[DEPOSICAO][DESACIONAR] = IDLE;
  acao_matrizTransicao[DEPOSICAO][DESACIONAR] = A03;
  
  proximoestado_matrizTransicao[DEPOSICAO][DETECTAR] = DETECCAO;
  acao_matrizTransicao[DEPOSICAO][DETECTAR] = A04;


  //setup da deteccao
  proximoestado_matrizTransicao[DETECCAO][REGISTRAR] = IDLE;
  acao_matrizTransicao[DETECCAO][REGISTRAR] = A05;

  proximoestado_matrizTransicao[DETECCAO][REGISTRAR_ACIONAR] = DEPOSICAO;
  acao_matrizTransicao[DETECCAO][REGISTRAR_ACIONAR] = A06;
}


int obterAcao(int estado, int codigoEvento) {
  return acao_matrizTransicao[estado][codigoEvento];
}

int obterProximoEstado(int estado, int codigoEvento) {
  return proximoestado_matrizTransicao[estado][codigoEvento];
}


void executarAcao(int codigoAcao) {
  switch(codigoAcao)
  {
    case A01: //deteccao do animal do idle
      break;

    case A02: //comeco da deposicao
      //determina a massa necessaria a ser depositada
      massa_atual = bal.measure();
      massa_necessaria = massa_desejada - massa_atual;
      deposicao_realizada = false;
      //abre a porta de deposicao
      servoMot.open();
      break;

    case A03: //fim da deposicao
      servoMot.close();
      deposicao_realizada = true;
      break;

    case A04: //detecta, sai da deposicao
      servoMot.close();
      massa_atual = bal.measure();
      
      //atualiza a massa desejada de acordo com o que ja foi depositado
      massa_desejada = massa_desejada - massa_atual;
      break;

    case A05: //registro, vai para o idle
      horario_atual = getTimeSec();
      massa_atual = bal.measure();
      sendData((String)massa_atual, (String)horario_atual);
      break;

    case A06: //registro, vai para deposicao
      horario_atual = getTimeSec();
      massa_atual = bal.measure();
      sendData((String)massa_atual, (String)horario_atual);

      //determina nova massa necessaria
      massa_necessaria = massa_desejada - massa_atual;
      
      //reabre porta
      servoMot.open();
      break;

    case A07: //atualiza variaveis internas
      horariosUsuario = readMassasHorarios();
      horario_desejado = horariosUsuario.horario1;
      massa_desejada = horariosUsuario.massa1;
      break;
  }
}


/**********************************************
            Inicio do programa
**********************************************/

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

  //inicio da maquina de estados
  iniciaMaquinaEstados();
  estado = IDLE;

  //bip para indicar final do setup
  beep.beep();
}

void loop() {
  if(Serial.available()) {
    codigoEvento = Serial.parseInt();
    codigoAcao = obterAcao(estado, codigoEvento);
    estado = obterProximoEstado(estado, codigoEvento);
    Serial.printf("Estado: %d Evento: %d Acao: %d\n", estado, codigoEvento, codigoAcao);
    executarAcao(codigoAcao);
  }
}
