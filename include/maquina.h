#ifndef DEFINICOES_SISTEMA
#define DEFINICOES_SISTEMA


#define true 1
#define false 0

#define NUM_ESTADOS 5
#define NUM_EVENTOS 7

//estados
#define IDLE 0
#define DEPOSICAO 1
#define DETECCAO 2
#define REGISTRO 3
#define ERRO 4

//eventos
#define NENHUM_EVENTO -1
#define HORARIO 0
#define SENSOR_PRESENCA_SIM 1
#define SENSOR_PRESENCA_NAO 2
#define TEMPORIZADOR 3
#define MASSA_SIM 4
#define MASSA_NAO 5
#define RACAO_SIM 6

//acoes
#define NENHUMA_ACAO -1
#define A01  0
#define A02  1
#define A03  2
#define A04  3
#define A05  4
#define A06  5
#define A07  6

#endif
