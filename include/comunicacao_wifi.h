#ifndef COMUNICACAO_WIFI_H
#define COMUNICACAO_WIFI_H

#include "Wifi.h"
#include <HTTPClient.h>
#include <Arduino.h>

//declaracao de variaveis
extern const char * ssid;
extern const char * password;

extern String google_script_leitura;
extern String google_script_escrita;

extern const char* ntpServer;
extern const long gmtOffset_sec;
extern const int daylightOffset_sec;  

void sendData(String dado, String time);

String readData();

int getTimeSec();


#endif