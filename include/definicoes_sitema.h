#ifndef DEFINICOES_SISTEMA
#define DEFINICOES_SISTEMA

#define true 1
#define false 0

/******************************************

           Máquina de Estados

*****************************************/

#define NUM_ESTADOS 4
#define NUM_EVENTOS 7

// estados
#define IDLE 0
#define DEPOSICAO 1
#define DETECCAO 2
#define ERRO 3

// eventos
#define NENHUM_EVENTO -1
#define DETECTAR 0
#define ACIONAR 1
#define DESACIONAR 2
#define REGISTRAR 3
#define REGISTRAR_ACIONAR 4
#define ATUALIZAR 5
#define AVISAR 6

// acoes
#define NENHUMA_ACAO -1
#define A01 0
#define A02 1
#define A03 2
#define A04 3
#define A05 4
#define A06 5
#define A07 6
#define A08 7

/******************************************

                Pinos

*****************************************/
// Pinos balança
#define SCK_BALANCA 22
#define DT_BALANCA 23

// Pino Motor Vibração
#define MOTVIB_PIN 13

// Pinos para o trigger e echo
#define PINO_TRIGGER 26
#define PINO_ECHO 25

// Pino para o servo
#define SERVO_PIN 21

#endif
