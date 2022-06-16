/*
 * File:      Wifi.h
 * Author:    Thiago L. Brawerman
 * Date:      13/06/2022
 * Comments:  Controle do modulo Wi-fi
 * Revision history:
 */

#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <WiFi.h>
#include "WiFiClientSecure.h"

class Wifi
{
private:
   // WiFiClientSecure client; // Cliente seguro (para ter acesso ao HTTPS)
    String textFix = "GET path";
    const char *ssid = "SSID";              // SSID
    const char *password = "password";      // senha
    const char *server = "docs.google.com"; // Server URL
public:
    int Write();
    Wifi();
    ~Wifi();
};

#endif