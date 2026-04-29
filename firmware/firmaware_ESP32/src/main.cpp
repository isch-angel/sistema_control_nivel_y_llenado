#include <Arduino.h>
// Todos estos includes pueden encontrarlos a la izquierda en la carpeta 'lib'
// Cada archivo de cabecera .h solamente contiene las definiciones de las funciones.
// Las funciones en si mismas estan en los archivos .cpp
#include "wifi_manager.h"
#include "sensors.h"
#include "pins.h"
#include "level.h"
#include "actuators.h"
#include "mqtt_manager.h"

// Umbrales para histeresis
const int UMBRAL_ENCENDIDO = 25; 
const int UMBRAL_APAGADO = 20;

unsigned long last_publish = 0;
const int publish_time = 1000;

void setup() {
    Serial.begin(115200);
    initWiFi();     // Inicia la conexion wifi
    initMqtt();     // Inicia la conexion MQTT
    initPins();     // Inicia los pines
}

void loop() {
    float d = getDistance();
    float level = getLevel_cm(d);
    int mapped_level = getLevelPercent(level);

    // Rutina de publicación (cada 1000 ms)
    if (millis() - last_publish >= publish_time) {
        publish_level(mapped_level); 
        publish_pump_state();
        last_publish = millis();
    }
    
    // Primero, verificamos si tenemos permiso para operar (sensor capacitivo y orden de llenado)
    if (!sensorCapacitivo() && getOrdenLlenado()) {
        
        // Si el nivel sube arriba de 25, encendemos
        if (mapped_level > UMBRAL_ENCENDIDO) {
            encenderActuadores();
        } 
        // Si el nivel baja de 20, apagamos
        else if (mapped_level < UMBRAL_APAGADO) {
            apagarActuadores();
        }
        // NOTA CLAVE: Si el nivel está entre 20 y 25, no hay ningún "else". 
        // Esto significa que el ESP32 simplemente no hace nada y la bomba 
        // se queda en el estado en el que ya estaba (ya sea prendida o apagada).

    } else {
        // Si el capacitivo se activa o nos quitan la orden de llenado, 
        // apagamos todo por seguridad sin importar el nivel.
        apagarActuadores();
    }
}
