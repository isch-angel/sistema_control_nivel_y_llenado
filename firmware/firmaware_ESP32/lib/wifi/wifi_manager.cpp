#include <Arduino.h>
#include <WiFi.h>
#include "wifi_manager.h"
#include "mqtt_manager.h"

const char* ssid = "wifi_docentes";
const char* password = "IReD0BgdY5";

void initWiFi() {
    WiFi.onEvent(WiFiEvent);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
}

void WiFiEvent(WiFiEvent_t event) {
    switch (event) {
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            Serial.println("WiFi connected");
            Serial.println("IP address: ");
            Serial.println(WiFi.localIP());
            connectToMqtt();
            break;

        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Serial.println("WiFi desconectado, reconectando...");
            WiFi.begin(ssid, password);
            break;

        default:
            break;
    }
}