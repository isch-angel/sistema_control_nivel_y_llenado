#pragma once
#include <WiFi.h>

// Las variables se definen aquí, solo una vez
// const char* ssid     = "IZZI-Torres-2.4G";
// const char* password = "TORRES271169B";

// const char* ssid = "wifi_docentes";   // Cambiarla por wifi-uach
// const char* password = "IReD0BgdY5";

// const char* ssid = "Di que eres pobre";
// const char* password = "Soy pobre";

void initWiFi ();
void WiFiEvent(WiFiEvent_t event);

