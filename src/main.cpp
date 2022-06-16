#include <Arduino.h>
#include "leds.h"
#include "motor.h"
#include "sensor_ultrassonico.h"
#include <Ultrasonic.h>
#include "Wifi.h"

float dist_CM;

// Cria os objetos LED
leds led_vermelho(LEDVERMELHO);
leds led_amarelo(LEDAMARELO);
leds led_verde(LEDVERDE);
leds led_azul(ONBOARD_LED);

// Cria objeto motor
motor motor_fuso(MOTORPIN1, MOTORPIN2);

// Cria sensor ultrassonico
sensor_ultrassonico sensor(PINO_TRIGGER, PINO_ECCHO);

void setup() {
  // Chama funções de teste
  led_vermelho.blink(500);
  led_amarelo.blink(500);
  led_verde.blink(500);
  led_azul.blink(500);
  motor_fuso.sweep_motor();
  // beep();

  // Inicializa Porta serial
  Serial.begin(9600);
}

void loop() {

  led_vermelho.blink(500);
  // obtem ultrassom
  dist_CM = sensor.get_ultrasonic();

  // Exibe informacoes no serial monitor
  Serial.print("\nDistancia em cm: ");
  Serial.print(dist_CM);
  motor_fuso.sweep_motor();
  delay(1000);
}

/*
//AKfycbyjRhrrYGSFyfo5epdLldAqU4y7oYmSmSmYGIv1fszldozsuzS6yQXY7aG26jXT5tPq

//https://script.google.com/macros/s/AKfycbyjRhrrYGSFyfo5epdLldAqU4y7oYmSmSmYGIv1fszldozsuzS6yQXY7aG26jXT5tPq/exec
//Include required libraries
#include "WiFi.h"
#include <HTTPClient.h>
#include "time.h"
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;
// WiFi credentials
const char* ssid = "Thiago's phone";         // change SSID
const char* password = "Thiago1202";    // change password
// Google script ID and required credentials
String GOOGLE_SCRIPT_ID = "AKfycbyjRhrrYGSFyfo5epdLldAqU4y7oYmSmSmYGIv1fszldozsuzS6yQXY7aG26jXT5tPq";    // change Gscript ID
int count = 0;
void setup() {
delay(1000);
Serial.begin(9600);
delay(1000);
// connect to WiFi
Serial.println();
Serial.print("Connecting to wifi: ");
Serial.println(ssid);
Serial.flush();
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}
// Init and get the time
configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}
void loop() {
 if (WiFi.status() == WL_CONNECTED) {
  static bool flag = false;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  char timeStringBuff[50]; //50 chars should be enough
  strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
  String asString(timeStringBuff);
  asString.replace(" ", "-");
  Serial.print("Time:");
  Serial.println(asString);
  String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"date=" + asString + "&sensor=" + String(count);
  Serial.print("POST data to spreadsheet:");
  Serial.println(urlFinal);
  HTTPClient http;
  http.begin(urlFinal.c_str());
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET();
  Serial.print("HTTP Status Code: ");
  Serial.println(httpCode);
  //---------------------------------------------------------------------
  //getting response from google sheet
  String payload;
  if (httpCode > 0) {
      payload = http.getString();
      Serial.println("Payload: "+payload);
  }
  //---------------------------------------------------------------------
  http.end();
}
count++;
delay(1000);
}
/*
//AKfycbx5Qx0RqhFYL1m2RVrk-OMclYaoDWF_WvjWUA49g46ENKJDQaALN8cylvcidaAbD6fY
//https://script.google.com/macros/s/AKfycbx5Qx0RqhFYL1m2RVrk-OMclYaoDWF_WvjWUA49g46ENKJDQaALN8cylvcidaAbD6fY/exec

// Include required libraries
#include "WiFi.h"
#include <HTTPClient.h>
// WiFi credentials
const char *ssid = "Thiago's Phone"; // change SSID
const char *password = "Thiago1202"; // change password
// Google script ID and required credentials
String GOOGLE_SCRIPT_ID = "AKfycbx5Qx0RqhFYL1m2RVrk-OMclYaoDWF_WvjWUA49g46ENKJDQaALN8cylvcidaAbD6fY"; // change Gscript ID
void setup()
{
  delay(1000);
  Serial.begin(115200);
  delay(1000);
  // connect to WiFi
  Serial.println();
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  Serial.flush();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
}
void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    String url = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?read";
    Serial.println("Making a request");
    http.begin(url.c_str()); // Specify the URL and certificate
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
    String payload;
    if (httpCode > 0)
    { // Check for the returning code
      payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    }
    else
    {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }
  delay(1000);
}*/