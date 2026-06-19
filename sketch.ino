#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#define LED_VERMELHO 19
#define LED_VERDE 18

const char* ssid = "Iphone de Kauan";
const char* password = "kauanteste*";

const char* serverName = "https://jsonplaceholder.typicode.com/todos/1";

void setup() {
  Serial.begin(115200);

  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);

  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_VERDE, LOW);

  Serial.println("Conectando ao Wi-Fi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi desconectado!");

    digitalWrite(LED_VERMELHO, HIGH);
    digitalWrite(LED_VERDE, LOW);

    WiFi.begin(ssid, password);

    delay(5000);
    return;
  }

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient https;

  Serial.println("Enviando requisição HTTPS...");

  if (https.begin(client, serverName)) {

    int httpCode = https.GET();

    Serial.print("Código HTTP: ");
    Serial.println(httpCode);

    if (httpCode > 0) {

      String payload = https.getString();

      Serial.println("Resposta:");
      Serial.println(payload);

      if (httpCode == HTTP_CODE_OK) {
        digitalWrite(LED_VERDE, HIGH);
        digitalWrite(LED_VERMELHO, LOW);
      } else {
        digitalWrite(LED_VERMELHO, HIGH);
        digitalWrite(LED_VERDE, LOW);
      }

    } else {
      Serial.print("Erro HTTPS: ");
      Serial.println(httpCode);

      digitalWrite(LED_VERMELHO, HIGH);
      digitalWrite(LED_VERDE, LOW);
    }

    https.end();

  } else {
    Serial.println("Falha ao iniciar conexão HTTPS.");

    digitalWrite(LED_VERMELHO, HIGH);
    digitalWrite(LED_VERDE, LOW);
  }

  delay(15000);
}
