/* 
 * File:      balanca.h
 * Author:    Bruno A. Sorban
 * Date:      14/06/2022
 * Comments:  Controle da balança do alimentador
 * Revision history:
 */

#ifndef BALANCA_H
#define	BALANCA_H

#include <Arduino.h>
#include "HX711.h"

#define SCK_BALANCA 22
#define DT_BALANCA 23
#define ESCALA_BALANCA_NUM 1
#define ESCALA_BALANCA_DEN 1
#define COEF_M_INV -1862
#define COEF_B 873


class balanca {
    private:
        double referencia;
        double medida;

    public:
        void tarar();
        double measure();
        balanca(int dt, int sck);
        ~balanca();
        HX711* adc_bal;
        
};

#endif	/* BALANCA_H */