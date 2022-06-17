#include "comunicacao_wifi.h"

const char * ssid = "GUERREIRO_NET";
const char * password = "50825084";

String google_script_leitura = "AKfycbw-SfekyBnNRGDhMqtjtv5f6iv7ovYjpLPN2ySiLwsMIL1W3SIv_8LyXqRGtzfUjUZNuA";
String google_script_escrita = "AKfycbw-7RdeFdl6DQRVcXVVRC9Cza6fPADGxR_KC1Itf4e46lv0er385I63jP74fcNU4iTw";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -3 * 60 * 60;
const int daylightOffset_sec = 0;  

void sendData(String dado, String time ) {
  HTTPClient http;
  String url="https://script.google.com/macros/s/"+google_script_escrita+"/exec?" + "date=" + time + "&sensor=" + dado;
  //Serial.print(url);
  Serial.print("Enviando dados de sensor");
  Serial.println();
  http.begin(url.c_str()); //Specify the URL and certificate
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET();  

  String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println(httpCode);    
    }
  http.end();
}

String readData() {
  HTTPClient http;
  String url = "https://script.google.com/macros/s/" + google_script_leitura + "/exec?read";

  Serial.print("Começo de requisição\n");
  http.begin(url.c_str());
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET();
  String payload;
    if (httpCode > 0) { //Check for the returning code
        payload = http.getString();
      }
    else {
      Serial.println("Error on HTTP request");
      payload = " ";
    }
	http.end();
    return payload;
}


int getTimeSec() {
  struct tm timeinfo;
  if(getLocalTime(&timeinfo)) {
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    return timeinfo.tm_hour * 60 + timeinfo.tm_min;
  } else {
    Serial.println("Failed to obtain time");
    return 0;
  }
}

