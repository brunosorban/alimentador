#include <Arduino.h>
#include "HX711.h"
 
#define DT 0
#define SCK 1
#define ESCALA_BALANCA_NUM 22.1
#define ESCALA_BALANCA_DEN 45800
 
HX711 escala;    // Relaciona a variável escala
 
void setup() {
  escala.begin (DT, SCK);
  Serial.begin(9600);
  Serial.print("Leitura da Tara:  ");
  Serial.println(escala.read());  // Aguada o termino de verificação do peso
  Serial.println("Aguarde!");
  Serial.println("Iniciando ...");
  escala.set_scale();             // Utiliza uma escala padrão de verificação
 
  escala.tare(10);                // Fixa o peso como tara
  Serial.println("Insira o item para Pesar"); 
}
 
void loop() {
  Serial.print("Valor da Leitura:  ");
  Serial.println(escala.get_value(10) * ESCALA_BALANCA_NUM / ESCALA_BALANCA_DEN,0);  // Retorna peso descontada a tara
  delay(100);
}