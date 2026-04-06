#include <Arduino.h>
#include "serial_comm.h"
#include "toggle_led.h"
#include "wifi_connection.h"
#include "sensor.h"

void setup() {
    initSerial();
    initWiFi();
    initSensors();
    
    initLed(); // Esto es una broma, ignorarlo
}

void loop() {
    Serial.print("distancia cm: ");
    Serial.println(sensorUltrasonico());
    delay(500);

    toggleLed(); // Ignorarlo tambien
}

