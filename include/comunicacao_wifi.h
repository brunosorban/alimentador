#ifndef COMUNICACAO_WIFI_H
#define COMUNICACAO_WIFI_H

#include "Wifi.h"
#include <HTTPClient.h>
#include <Arduino.h>
#include <ArduinoJson.h>

#define MAX_ARRAY 100

//declaracao de variaveis
extern const char * ssid;
extern const char * password;

extern String google_script_leitura;
extern String google_script_escrita;

extern const char* ntpServer;
extern const long gmtOffset_sec;
extern const int daylightOffset_sec;  

typedef struct {
    int horario1;
    int horario2;
    int horario3;
    int massa1;
    int massa2;
    int massa3;
} horarios;

void sendData(String dado, String time);

String readData();

horarios readMassasHorarios();

int getTimeSec();


#endif