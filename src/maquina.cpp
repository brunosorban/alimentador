#include <stdio.h>
#include "maquina.h"

int obterEvento();
int obterAcao(int estado, int codigo_evento);
int obterProximoEstado(int estado, int codigo_evento);
void executarAcao(int codigo_acao);

int main() {
    int codigo_evento;
    int codigo_acao;
    int estado;

    estado = IDLE;

    printf("Máquina de estados iniciada\n");

    while(1) {
        codigo_evento = obterEvento();
        if(codigo_evento != NENHUM_EVENTO) {
            codigo_acao = obterAcao(estado, codigo_evento);
            estado = obterProximoEstado(estado, codigo_evento);
            executarAcao(codigo_acao);
            printf("Estado: %d Evento: %d Acao %d\n", estado, codigo_evento, codigo_acao);
        }

    }
}

int obterEvento() {
    return 0;
}

int obterAcao(int estado, int codigo_evento) {
    return 0;
}

int obterProximoEstado(int estado, int codigo_evento) {
    return 0;
}

void executarAcao(int codigo_acao) {
    return;
}