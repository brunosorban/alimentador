#include <Arduino.h>
#include "sensor_ultrassonico.h"
#include "balanca.h"
#include "HX711.h"
#include "comunicacao_wifi.h"
#include "definicoes_sitema.h"
#include "servo_esp.h"
#include "motor_vibra.h"

/**********************************************
                   Definicoes
**********************************************/
#define DIST_TRESH 25    // distancia em cm que considera deteccao
#define HORARIO_UPDATE 5 // minutos desde as 00h
#define TEMPO_ERRO 1     // minutos para a deposicao

/**********************************************
              Criacao dos objetos
**********************************************/
// Cria objeto balança
balanca bal(DT_BALANCA, SCK_BALANCA);

// Cria objeto servomotor
servo_esp servoMot(SERVO_PIN);

motor_vibra vibMot(MOTVIB_PIN);

sensor_ultrassonico ultraSon(PINO_ECHO, PINO_TRIGGER);

/**********************************************
             Variaveis Globais
**********************************************/
float dist_cm;
horarios horariosUsuario;
int horario_desejado;

// variaveis globais usadas na maquina de estados
double massa_atual;
double massa_consumida;
double massa_desejada;
double massa_necessaria;

int horario_atual;                   // guarda o horario atual (minutos desde as 00h)
int flag_deposicao = 0;              // se verdadeiro, ocorre deposicao
int flag_deteccao = 0;               // se verdadeiro, tinha detectado
int indice_horario = 0;              // índice que indica qual próximo horário
int horario_inicio_deposicao = 9999; // horário máximo inalcançável

int acao_matrizTransicao[NUM_ESTADOS][NUM_EVENTOS];
int proximoestado_matrizTransicao[NUM_ESTADOS][NUM_EVENTOS];
int estado;
int codigoAcao = NENHUMA_ACAO;
int codigoEvento = NENHUM_EVENTO;

int timer_atualizacao;

/**********************************************
        Funcoes da Maquina de Estados
**********************************************/

int determinaEvento()
{
  // obtencao de dados
  dist_cm = ultraSon.get_ultrasonic();
  massa_atual = bal.measure();
  Serial.print("------------------------------\n");
  Serial.print("Massa atual:");
  Serial.println(massa_atual);
  Serial.print("Distancia atual:");
  if (dist_cm < 4)
    dist_cm = 100;
  Serial.println(dist_cm);
  Serial.print("------------------------------\n");
  vTaskDelay(100 / portTICK_PERIOD_MS);
  horario_atual = getTimeMin();

  // logica para evento

  if (horario_atual < HORARIO_UPDATE)
  {
    return ATUALIZAR;
  }

  if (dist_cm < DIST_TRESH)
  {
    return DETECTAR;
  }
  if (flag_deteccao)
  {
    if (flag_deposicao)
    {
      return REGISTRAR_ACIONAR;
    }
    else
    {
      return REGISTRAR;
    }
  }
  if (indice_horario < 3)
  {
    if (massa_atual >= horariosUsuario.massa_array[indice_horario])
    {
      return DESACIONAR;
    }

    if (flag_deposicao && !flag_deteccao && (horario_atual - horario_inicio_deposicao >= TEMPO_ERRO))
    {
      return AVISAR;
    }

    if (horario_atual >= horariosUsuario.horario_array[indice_horario])
    {
      return ACIONAR;
    }
  }

  return NENHUM_EVENTO;
}

void iniciaMaquinaEstados()
{
  int i;
  int j;

  for (i = 0; i < NUM_ESTADOS; i++)
  {
    for (j = 0; j < NUM_EVENTOS; j++)
    {
      acao_matrizTransicao[i][j] = NENHUMA_ACAO;
      proximoestado_matrizTransicao[i][j] = i;
    }
  }

  // setup do idle
  proximoestado_matrizTransicao[IDLE][DETECTAR] = DETECCAO;
  acao_matrizTransicao[IDLE][DETECTAR] = A01;

  proximoestado_matrizTransicao[IDLE][ACIONAR] = DEPOSICAO;
  acao_matrizTransicao[IDLE][ACIONAR] = A02;

  proximoestado_matrizTransicao[IDLE][ATUALIZAR] = IDLE;
  acao_matrizTransicao[IDLE][ATUALIZAR] = A07;

  // setup da deposicao
  proximoestado_matrizTransicao[DEPOSICAO][DESACIONAR] = IDLE;
  acao_matrizTransicao[DEPOSICAO][DESACIONAR] = A03;

  proximoestado_matrizTransicao[DEPOSICAO][DETECTAR] = DETECCAO;
  acao_matrizTransicao[DEPOSICAO][DETECTAR] = A04;

  proximoestado_matrizTransicao[DEPOSICAO][AVISAR] = ERRO;
  acao_matrizTransicao[DEPOSICAO][AVISAR] = A08;

  // setup da deteccao
  proximoestado_matrizTransicao[DETECCAO][REGISTRAR] = IDLE;
  acao_matrizTransicao[DETECCAO][REGISTRAR] = A05;

  proximoestado_matrizTransicao[DETECCAO][REGISTRAR_ACIONAR] = DEPOSICAO;
  acao_matrizTransicao[DETECCAO][REGISTRAR_ACIONAR] = A06;
}

int obterAcao(int estado, int codigoEvento)
{
  if (codigoEvento == -1)
  {
    return NENHUMA_ACAO;
  }
  return acao_matrizTransicao[estado][codigoEvento];
}

int obterProximoEstado(int estado, int codigoEvento)
{
  if (codigoEvento == -1)
  {
    return estado;
  }
  return proximoestado_matrizTransicao[estado][codigoEvento];
}

void executarAcao(int codigoAcao)
{
  switch (codigoAcao)
  {
  case A01: // deteccao do animal do idle
    flag_deteccao = 1;
    break;

  case A02: // comeco da deposicao
    // determina a massa necessaria a ser depositada
    massa_atual = bal.measure();
    massa_necessaria = massa_desejada - massa_atual;
    // abre a porta de deposicao
    servoMot.open();
    vibMot.On();
    flag_deposicao = 1;
    horario_inicio_deposicao = getTimeMin();
    break;

  case A03: // fim da deposicao
    servoMot.close();
    vibMot.Off();
    flag_deposicao = 0;
    if (indice_horario < 3)
    {
      indice_horario++;
    }
    break;

  case A04: // detecta, sai da deposicao
    servoMot.close();
    vibMot.Off();
    massa_atual = bal.measure();

    // atualiza a massa desejada de acordo com o que ja foi depositado
    massa_desejada = massa_desejada - massa_atual;
    flag_deteccao = 1;
    break;

  case A05: // registro, vai para o idle
    horario_atual = getTimeMin();
    massa_consumida = bal.measure() - massa_atual;
    massa_atual = bal.measure();
    sendData((String)massa_consumida, (String)horario_atual);
    flag_deteccao = 0;
    break;

  case A06: // registro, vai para deposicao
    horario_atual = getTimeMin();
    massa_consumida = bal.measure() - massa_atual;
    massa_atual = bal.measure();
    sendData((String)massa_consumida, (String)horario_atual);

    // determina nova massa necessaria
    massa_necessaria = massa_desejada - massa_atual;

    horario_inicio_deposicao = getTimeMin();
    // reabre porta
    servoMot.open();
    vibMot.On();
    flag_deteccao = 0;
    break;

    // determina nova massa necessaria
    massa_necessaria = massa_desejada - massa_atual;

  case A08:
    servoMot.close();
    vibMot.Off();
    Serial.println("---------------- ERRO ----------------");
    Serial.println("TEMPO DE DEPOSICAO MAXIMO ATINGIDO");
    Serial.println("-------------------------------------");
    break;
  }
}

void taskMaqEst(void *pv)
{
  for (;;)
  {
    horario_atual = getTimeMin();
    if (horario_atual)
    {
      Serial.printf("Horario_atual: %d\n", horario_atual);
      vTaskDelay(100 / portTICK_PERIOD_MS);

      codigoEvento = determinaEvento();
      if (codigoEvento != NENHUM_EVENTO)
      {
        codigoAcao = obterAcao(estado, codigoEvento);
        estado = obterProximoEstado(estado, codigoEvento);
        Serial.printf("Estado: %d Evento: %d Acao: %d\n", estado, codigoEvento, codigoAcao);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        executarAcao(codigoAcao);
        Serial.printf("Indice: %d, Horario: %d\n", indice_horario, horariosUsuario.horario_array[indice_horario]);
        vTaskDelay(100 / portTICK_PERIOD_MS);
      }
    }
  }
}
/**********************************************
            Inicio do programa
**********************************************/

void setup()
{

  Serial.begin(115200);

  // inicio da comunicacao wifi
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

  // configuracao do horario
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Horário GMT-3 configurado.");

  // leitura inicial dos horarios programados
  horariosUsuario = readMassasHorarios();

  // inicio da maquina de estados
  iniciaMaquinaEstados();
  estado = IDLE;

  horario_atual = getTimeMin();

  while (horario_atual > horariosUsuario.horario_array[indice_horario])
  {
    indice_horario++;
  }

  Serial.print("Horarios:\n ");
  for (int i = 0; i < 3; i++)
  {
    Serial.println(horariosUsuario.horario_array[i]);
  }
  Serial.print("\nMassas:\n ");
  for (int i = 0; i < 3; i++)
  {
    Serial.println(horariosUsuario.massa_array[i]);
  }

  bal.tarar();
  Serial.println("Begin...");
  xTaskCreate(taskMaqEst, "Task Maquina de Estados", 8000, NULL, 1, NULL);

  vTaskStartScheduler();

  for (;;)
    ;
}

void loop()
{
  // NOTHING
}
