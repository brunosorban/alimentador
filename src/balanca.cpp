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
  adc_bal = new HX711;
  adc_bal->begin(dt, sck);
}

double balanca::measure() {
    medida = (adc_bal->get_value(10)) * ESCALA_BALANCA_NUM;
    medida = medida / ESCALA_BALANCA_DEN;
    medida = medida - TARA;
    return medida;
}

balanca::~balanca() 
{
    delete adc_bal;
}