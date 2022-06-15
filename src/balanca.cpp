/* 
 * File:      balanca.cpp
 * Author:    Bruno A. Sorban
 * Date:      14/06/2022
 * Comments:  Controle da balança do alimentador
 * Revision history:
 */


#include <Arduino.h>
#include "balanca.h"
#include "HX711.h"

balanca::balanca(int dt, int sck)
{
  dt = dt;
  sck = sck;
  adc_bal.begin(dt, sck);
  adc_bal.set_scale();
}

double balanca::measure() {
    medida = adc_bal.get_value(10) * ESCALA_BALANCA_NUM;
    medida = medida / ESCALA_BALANCA_DEN;
    return medida;
}