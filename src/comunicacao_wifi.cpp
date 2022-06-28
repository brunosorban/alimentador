#include "comunicacao_wifi.h"
#include "time.h"

const char *ssid = "Thiago's phone";
const char *password = "12345678";

String google_script_leitura = "AKfycbx95y5vE8FR9Zqb0F6eATDhAlcTmFlUyF80ufQr3irM9_HXP6n-WVxAqievXl4GBZCRcw";

String google_script_escrita = "AKfycbw-7RdeFdl6DQRVcXVVRC9Cza6fPADGxR_KC1Itf4e46lv0er385I63jP74fcNU4iTw";

const char *ntpServer = "br.pool.ntp.org";
const long gmtOffset_sec = -3 * 60 * 60;
const int daylightOffset_sec = 0;

void sendData(String dado, String time)
{
  HTTPClient http;
  String url = "https://script.google.com/macros/s/" + google_script_escrita + "/exec?" + "date=" + time + "&sensor=" + dado;
  Serial.print("Enviando dados...\n");
  Serial.println("-----------------------------------------------");
  vTaskDelay(100 / portTICK_PERIOD_MS);

  http.begin(url.c_str()); // Specify the URL and certificate
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET();

  String payload;
  if (httpCode > 0)
  {
    payload = http.getString();
  }
  http.end();
}

String readData()
{
  HTTPClient http;
  String url = "https://script.google.com/macros/s/" + google_script_leitura + "/exec?read";

  Serial.print("Começo de requisição\n");
  http.begin(url.c_str());
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET();
  String payload;
  if (httpCode > 0)
  { // Check for the returning code
    payload = http.getString();
  }
  else
  {
    Serial.println("Error on HTTP request");
    payload = " ";
  }
  http.end();
  return payload;
}

horarios readMassasHorarios()
{
  HTTPClient http;
  String url = "https://script.google.com/macros/s/" + google_script_leitura + "/exec?read";

  Serial.print("Buscando horarios\n");
  http.begin(url.c_str());
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET();
  String payload;
  if (httpCode > 0)
  { // Check for the returning code
    payload = http.getString();
  }
  else
  {
    Serial.println("Error on HTTP request");
    payload = " ";
  }
  http.end();
  char json[MAX_ARRAY];

  strcpy(json, payload.c_str());

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);

  horarios horariosUsuario;

  horariosUsuario.horario1 = doc["um"][0];
  horariosUsuario.horario_array[0] = doc["um"][0];
  horariosUsuario.massa1 = doc["um"][1];
  horariosUsuario.massa_array[0] = doc["um"][1];

  horariosUsuario.horario2 = doc["dois"][0];
  horariosUsuario.horario_array[1] = doc["dois"][0];
  horariosUsuario.massa2 = doc["dois"][1];
  horariosUsuario.massa_array[1] = doc["dois"][1];

  horariosUsuario.horario3 = doc["tres"][0];
  horariosUsuario.horario_array[2] = doc["tres"][0];
  horariosUsuario.massa3 = doc["tres"][1];
  horariosUsuario.massa_array[2] = doc["tres"][1];

  horariosUsuario.horario_array[3] = 9999;
  horariosUsuario.massa_array[3] = 0;

  return horariosUsuario;
}

int getTimeMin()
{
  struct tm timeinfo;
  if (getLocalTime(&timeinfo))
  {
    return timeinfo.tm_hour * 60 + timeinfo.tm_min;
  }
  else
  {
    Serial.println("Failed to obtain time");
    return 0;
  }
}
