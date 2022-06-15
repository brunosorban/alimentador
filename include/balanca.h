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

#define SCK_BALANCA 0
#define DT_BALANCA 1
#define ESCALA_BALANCA_NUM 22.1
#define ESCALA_BALANCA_DEN 45800

class balanca {
    private:
        int dt;
        int sck;
        double medida;
        HX711 adc_bal;

    public:
        balanca(int dt, int sck);
        double measure();
};

#endif	/* BALANCA_H */