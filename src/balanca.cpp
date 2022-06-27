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
  adc_bal = new HX711;
  adc_bal->begin(dt, sck);
  referencia = 35;
}

double balanca::measure()
{
  medida = (adc_bal->get_value(10)) / COEF_M_INV;
  medida = medida + COEF_B;
  medida -= referencia;
  return medida;
}

void balanca::tarar()
{
  referencia += measure();
}

balanca::~balanca()
{
  delete adc_bal;
}
