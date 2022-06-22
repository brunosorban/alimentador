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
    //interpolação linear para a balança y = (x/m_inv) + b
    // medida = (adc_bal->get_value(10)) * ESCALA_BALANCA_NUM;
    // medida = medida / ESCALA_BALANCA_DEN;
    // medida = medida - TARA;

    //calibracao
    // double leitura_adc = 0;
    // for(int i = 0; i < 15; i++) {
    //   leitura_adc += adc_bal->get_value(10)/15;
    // }
    // Serial.printf("ADC: %.0f\n", leitura_adc);

    medida = (adc_bal->get_value(10)) / COEF_M_INV;
    medida = medida + COEF_B;
    medida -= TARA_POTE;
    return medida;
}

balanca::~balanca() 
{
    delete adc_bal;
}
