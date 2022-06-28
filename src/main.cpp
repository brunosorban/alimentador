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
#define WIFI_TIMEOUT 20

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
  Serial.println("------------------ Dados ----------------------");
  Serial.print("Massa atual:     ");
  Serial.println(massa_atual);
  Serial.print("Distancia atual: ");

  if (dist_cm < 4)
  {
    Serial.print("NO READING");
    dist_cm = 100;
  }
  else
    Serial.print(dist_cm);

  dist_cm < DIST_TRESH ? Serial.println("    PRESENTE") : Serial.println("    AUSENTE");

  vTaskDelay(100 / portTICK_PERIOD_MS);
  horario_atual = getTimeMin();
  Serial.print("Horário atual:  ");
  Serial.printf("%02d:%02d\n", (int)horario_atual / 60, horario_atual % 60);
  Serial.println("-----------------------------------------------");

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
    Serial.println("------------------ ERRO -----------------------");
    Serial.println("TEMPO DE DEPOSICAO MAXIMO ATINGIDO");
    Serial.println("-----------------------------------------------");
    ESP.restart();
    for (;;)
      ;
    break;
  }
}

String decodeEstado(int estado)
{
  switch (estado)
  {
  case IDLE:
    return "IDLE";
  case DEPOSICAO:
    return "DEPOSIÇÃO";
  case DETECCAO:
    return "DETECÇÃO";
  case ERRO:
    return "ERRO";

  default:
    return "";
  }
}
String decodeAcao(int acao)
{
  if (acao == NENHUMA_ACAO)
    return "NENHUMA_ACAO";
  return String("A0" + (String)(acao + 1));
}
String decodeEvento(int evento)
{
  switch (evento)
  {
  case NENHUM_EVENTO:
    return "NENHUM_EVENTO";
  case DETECTAR:
    return "DETECTAR";
  case ACIONAR:
    return "ACIONAR";
  case DESACIONAR:
    return "DESACIONAR";
  case REGISTRAR:
    return "REGISTRAR";
  case REGISTRAR_ACIONAR:
    return "REGISTRAR_ACIONAR";
  case ATUALIZAR:
    return "ATUALIZAR";
  case AVISAR:
    return "AVISAR";
  default:
    return "";
  }
}

void taskMaqEst(void *pv)
{
  for (;;)
  {
    horario_atual = getTimeMin();
    if (horario_atual)
    {
      if (indice_horario < 3)
      {
        Serial.printf("Próximo Horario: %02d:%02d\n", (int)horariosUsuario.horario_array[indice_horario] / 60, horariosUsuario.horario_array[indice_horario] % 60);
        Serial.printf("Próxima Quantidade ração: %.2f\n", (float)horariosUsuario.massa_array[indice_horario]);
        Serial.printf("Índice: %d\n", indice_horario);
        Serial.println("-----------------------------------------------");
      }
      else
      {
        Serial.println("\n         Rotina Concluida\n");
        Serial.println("-----------------------------------------------");
      }

      vTaskDelay(100 / portTICK_PERIOD_MS);

      codigoEvento = determinaEvento();
      
      codigoAcao = obterAcao(estado, codigoEvento);
      estado = obterProximoEstado(estado, codigoEvento);
      Serial.println("--------------- Máquina de Estados ------------");
      Serial.printf("Estado: %d  ", estado);
      Serial.println(decodeEstado(estado));
      Serial.printf("Evento: %d  ", codigoEvento);
      Serial.println(decodeEvento(codigoEvento));
      Serial.printf("Acao:   %d  ", codigoAcao);
      Serial.println(decodeAcao(codigoAcao));
      Serial.println("-----------------------------------------------");
      vTaskDelay(100 / portTICK_PERIOD_MS);
      executarAcao(codigoAcao);
      

      vTaskDelay(5000 / portTICK_PERIOD_MS);
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
  unsigned long initTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() -initTime < WIFI_TIMEOUT*1000)
  {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("\nFailed to Connect\n");
    Serial.print("Rebooting...\n");
    ESP.restart();
  } 

  Serial.println("\nWiFi conectado.");

  // configuracao do horario
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Horário GMT-3 configurado.\n");

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

  Serial.print("Horarios | Massas\n");
  for (int i = 0; i < 3; i++)
  {
    Serial.printf("  %02d:%02d  | %.2f\n",
                  (int)horariosUsuario.horario_array[i] / 60,
                  horariosUsuario.horario_array[i] % 60,
                  (float)horariosUsuario.massa_array[i]);
  }
  bal.tarar();
  // delay(10000);
  Serial.println("\nBegin...");
  Serial.println("-----------------------------------------------");
  xTaskCreate(taskMaqEst, "Task Maquina de Estados", 8000, NULL, 1, NULL);

  vTaskStartScheduler();

  for (;;)
    ;
}

void loop()
{
  // NOTHING
}
