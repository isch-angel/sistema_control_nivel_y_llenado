#include <Arduino.h>
#include <wifi.h>
#include "wifi_connection.h"

// const char* ssid = "IZZI-Torres-2.4G";   // No se puede conectar a redes 5G
// const char* password = "TORRES271169B";

const char* ssid = "wifi_docentes";   // Cambiarla por wifi-uach
const char* password = "IReD0BgdY5";

// Solo inicializa la conexion
void initWiFi() {
  WiFi.onEvent(WiFiEvent);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // Serial.println("Connecting to WiFi...");
}

void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case ARDUINO_EVENT_WIFI_STA_START:
      Serial.println("WiFi iniciado");
      break;

    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      Serial.println("Conectado al AP");
      break;

    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
      break;

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("WiFi desconectado, reconectando...");
      WiFi.disconnect();
      WiFi.begin(ssid, password);
      break;

    default:
      break;

  }
}