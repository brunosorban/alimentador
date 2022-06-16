/*
 * File:      Wifi.cpp
 * Author:    Thiago L. Brawerman
 * Date:      13/06/2022
 * Comments:  Controle do modulo Wi-fi
 * Revision history:
 */

#include "Wifi.h"

Wifi::Wifi()
{
    Serial.println("Modulo Wi-Fi iniciando");
    
/*
    WiFi.mode(WIFI_STA);

    delay(100);

    Serial.print("Conectando a SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    // tentativa de conectar na rede
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        // esperar 1 segundos
        delay(1000);
    }
    Serial.println("");
    Serial.print("Connectado a ");
    Serial.println(ssid);
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    client.setInsecure();*/
}
/*
Wifi::Write()
{
    if (client.connect(server, 443))
    {
        String toSend = textFix; // Atribuimos a String auxiliar
        // na nova String que sera enviada
        toSend += random(0, 501); // Adicionamos um valor aleatorio
    }
}
*/
Wifi::~Wifi()
{
}