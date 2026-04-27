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

// Actualizamos la firma de la función para que reciba el nivel
void publish_level(int mapped_level); 
void publish_pump_state();
void read_serial_command();

// Umbrales para histeresis
const int UMBRAL_ENCENDIDO = 25; 
const int UMBRAL_APAGADO = 20;

unsigned long last_publish = 0;
const int publish_time = 1000;

void setup() {
    Serial.begin(115200);
    initMqtt();     // Inicia la conexion MQTT
    initWiFi();     // Inicia la conexion wifi
    initPins();     // Inicia los pines
}

void loop() {
    float d = sensorUltrasonico();
    float level_cm = tankLevel(d);
    int mapped_level = map(level_cm, 0, TANK_DEEP, 0, 100);

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

// Ahora la función recibe el int que calculamos en el loop
void publish_level(int mapped_level) {
    char payload[10];
    
    // Como mapped_level es un int, dtostrf necesita un float. 
    // Lo casteamos a (float) para que tu lógica original siga funcionando sin problemas.
    dtostrf((float)mapped_level, 1, 2, payload);  

    // Publicar nivel del tanque/contenedor
    mqttClient.publish(TOPIC_NIVEL, 0, false, payload);
}

void publish_pump_state() {
    const char* payload = actuator_s ? "1" : "0";    
    mqttClient.publish(TOPIC_BOMBA, 0, false, payload);
}